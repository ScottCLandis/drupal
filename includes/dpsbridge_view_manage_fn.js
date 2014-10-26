/**
 * View Manage Scripting.
 */

// Set to Folio node ID when opening the edit panel.
var fid = '',
    previewFileName   = '',
    amazonDimensions  = '',
    androidDimensions = '',
    appleDimensions   = '',
    offsetIndex = 0,
    baseURL     = "",
    pathToDir   = "";
/**
 * Pull the account type from the Folio node and credentials using account type.
 */
function acquire_account_credentials(folioNodeID, filenames) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/pull-accounts",
    type: "POST",
    data: { "folioNodeID":folioNodeID },
    success: function(output) {
      if (!output['folioMeta']['dimension']) {
        dpsbridge_helper_show_status("Please select a resolution for the folio!");
        dpsbridge_helper_delete_files(baseURL, filenames);
      } else if (output['message'] == 'ok') {
        dpsbridge_helper_update_status("Connecting to Folio Producer (" + output['account']['type'] + " account)...");
        fp_connect(output['account'], output['folioMeta'], filenames);
      } else {
        dpsbridge_helper_show_status(output['message']);
        dpsbridge_helper_delete_files(baseURL, filenames);
      }
    }
  });
}
/**
 * Convert the folders into .folio files.
 */
function generate_folio(folioNodeID, filenames) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/generate-folios",
    type: "POST",
    data: { "filenames":filenames },
    success: function(output) {
      dpsbridge_helper_update_status("Acquiring account credentials...");
      acquire_account_credentials(folioNodeID, filenames);
    }
  });
}

/**
 * Pull the metadata and generate the HTML articles.
 */
function generate_html(folioNodeID) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/generate-HTML",
    type: "POST",
    data: { "folioNodeID":folioNodeID },
    success: function(output) {
      dpsbridge_helper_update_status("Generating Folios...");
      generate_folio(folioNodeID, output);
    }
  });
}
/**
 * Auto generate an HTML article folder.
 */
function generate_selected_html(articleNodeID) {
  dpsbridge_helper_show_status('Generating HTML Stacks...');
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/generate-selected-HTML",
    type: "POST",
    data: {
      "folioNodeID"   :fid,
      "articleNodeID" :articleNodeID },
    success: function(output) {
      if (output['message'] = 'ok') {
        dpsbridge_helper_download_file(baseURL, output['destination'], output['filename'], '1');
        jQuery('#dialog-status').dialog('close');
      } else {
        dpsbridge_helper_show_status(output['message']);
      }
    }
  });
}
/**
 * Obtain the values from the selected checkboxes, and toggle the overlay.
 */
function get_selected(toggle) {
  var selectedFolio = jQuery('.views-table tr input:checked');
  if (!selectedFolio.val()) {
    dpsbridge_helper_show_status("Please select a folio first!");
  } else {
    switch(toggle) {
      case 'clone':
        dpsbridge_helper_folio_clone(baseURL, selectedFolio.val());
        break;

      case 'delete':
        dpsbridge_helper_delete_node(baseURL, selectedFolio.val());
        break;

      case 'upload':
        dpsbridge_helper_show_status('Generating HTML articles...', 600, 500);
        generate_html(selectedFolio.val());
        break;
    }
  }
}
/* ================================================= *
 * Given the account credentials and folio metadata,
 *   attempts to login to Folio Producer via the API.
 * ================================================= */
function fp_connect(accountMeta, folioMeta, filenames) {
  jQuery.ajax({
    url: pathToDir + "/fp_connect.php",
    type: "POST",
    data: {
      "AdobeID"   :accountMeta['adobeID'],
      "Password"  :accountMeta['password'],
      "APIKey"    :accountMeta['apiKey'],
      "APISecret" :accountMeta['apiSecret'] },
    success: function(output) {
      if (output == 'ok') {
        if (folioMeta['productID'] && folioMeta['status'] == 'Uploaded') {
          dpsbridge_helper_update_status("Updating \"" + folioMeta['folioName'] + "\", please wait (a while)...");
          fp_upload(accountMeta, folioMeta, filenames);
        } else {
          dpsbridge_helper_update_status("Creating \"" + folioMeta['folioName'] + "\"...");
          fp_create(accountMeta, folioMeta, filenames);
        }
      } else {
        dpsbridge_helper_show_status('Session error, please try again!');
        dpsbridge_helper_delete_files(baseURL, filenames);
        fp_logout();
      }
    }
  });
}
/* ============================================================= *
 * Given the folio metadata,
 *   attempts to create a folio in the Folio Producer via the API
 * ============================================================= */
function fp_create(accountMeta, folioMeta, filenames) {
  jQuery.ajax({
    url: pathToDir + "/fp_create.php",
    type: "POST",
    data: {
      "folioName"      :folioMeta['folioName'],
      "magazineTitle"   :folioMeta['magTitle'],
      "folioNumber"    :folioMeta['folioNum'],
      "folioDescription"  :folioMeta['folioDesc'],
      "publicationDate"  :folioMeta['pubDate'],
      "dimension"      :folioMeta['dimension'],
      "folioIntent"     :folioMeta['orientation'],
      "targetViewer"    :folioMeta['viewer'],
      "filters"      :folioMeta['filter'] },
    success: function(output) {
      dpsbridge_helper_update_status("Uploading to \"" + folioMeta['folioName'] + "\", please wait (a while)...");
      folioMeta['productID'] = output;
      fp_upload(accountMeta, folioMeta, filenames);
    }
  });
}
/* ========================================= *
 * Attempts to logout of the Folio Producer,
 *   and clean the PHP Session
 * ========================================= */
function fp_logout() {
  jQuery.ajax({
    url: pathToDir + "/fp_logout.php",
    type: "POST"
  });
}
/* =================================================== *
 * Attempts to synchronize with the Folio Producer.
 *   1. Log into the Folio Producer
 *   2. Grab all articles and its metadata
 *   3. Append any articles with assetFormat != 'Auto',
 *     aka not HTML Articles, into the existing table
 * =================================================== */
function fp_sync(folioID, folioName, credentials, articles, isAds, alienated, uploadDate) {
  dpsbridge_helper_update_status("Syncing with the Folio Producer, Please wait...");
  jQuery.ajax({
    url: pathToDir + "/fp_connect.php",
    type: "POST",
    data: {
      "folioID"   :folioID,
      "AdobeID"   :credentials['adobeID'],
      "Password"  :credentials['password'],
      "APIKey"    :credentials['apiKey'],
      "APISecret" :credentials['apiSecret'] },
    success: function(output) {
      fp_logout();
      output = JSON.parse(output);
      if (!output['status']) {
        dpsbridge_helper_show_status('Session error, please try again!');
      } else if (output['status'] == 'ok') {
        dpsbridge_helper_pull_articles(folioName, articles, isAds, alienated, uploadDate, output['articles']);
      } else {
        dpsbridge_helper_show_status(output);
      }
    }
  });
}
/* ================================================================== *
 * Given the filenames and the folio ID (on the Folio Producer side),
 *   attempts to upload the HTML Resources, Cover, and Articles.
 * ================================================================== */
function fp_upload(accountMeta, folioMeta, filenames) {
  jQuery.ajax({
    url: pathToDir + "/fp_upload.php",
    type: "POST",
    data: {
      "folioID"   :folioMeta['productID'],
      "filenames" :filenames,
      "alienated" :folioMeta['alienated'],
      "landscape" :folioMeta['landscape'],
      "portrait"  :folioMeta['portrait'],
      "dimension" :folioMeta['dimension'],
      "status"  :folioMeta['status'],
      "style"     :folioMeta['stylesheet'] },
    success: function(output) {
      dpsbridge_helper_delete_files(baseURL, filenames);
      dpsbridge_helper_update_status(output);
      if (folioMeta['status'] != 'Uploaded') {
        node_status_update(folioMeta['folioNodeID'], folioMeta['productID']);
      }
      else {
        node_status_timestamp(folioMeta['folioNodeID']);
      }
    }
  });
}
/**
 * Update the image url(s) on the current webpage using JQUERY on success.
 */
function imageUI_update() {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/pull-content",
    type: "POST",
    data: { "nodeID":fid },
    success: function(output) {
      jQuery('#portrait').attr('src', '/' + output['portrait']);
      jQuery('#landscape').attr('src', '/' + output['landscape']);
      jQuery('#image').attr('src', '/' + output['landscape']);
      dpsbridge_helper_show_status("Successfully uploaded image.");
    }
  });
}
/**
 * Save the folio ID (from Folio Producer) in the folio node (Drupal node).
 */
function node_status_update(folioNodeID, folioID) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/update-status",
    type: "POST",
    data: {
      "folioNodeID" :folioNodeID,
      "folioID"     :folioID },
    success: function(output) {
      node_status_timestamp(folioNodeID);
    }
  });
}
/**
 * Update the upload timestamp for the given folio node ID.
 */
function node_status_timestamp(folioNodeID) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/update-timestamp",
    type: "POST",
    data: { "folioNodeID" :folioNodeID },
    success: function(output) {
      fp_logout();
      dpsbridge_helper_update_status("Redirecting in 5 seconds...");
      setTimeout(function() { window.location = baseURL + "/admin/config/content/fpmanage"; }, 5000);
    }
  });
}
/* ============================================================================ *
 * Pulls all information regarding the targeted article node from the database,
 *   generates the new table row for each article node,
 *   appends it to the table body with id 'articles-wrapper'
 * ============================================================================ */
function dpsbridge_helper_pull_articles(folioName, drupalArticles, isAds, alienatedID, uploadDate, fpArticles) {
  var alienatedCount = 0, index = 1;

  // Get sort numbers in sequence.
  var sortNumbers = Array();
  var indexSort = 0;
  for (var n = 0; n < fpArticles.length; n++) {
    sortNumber = parseInt(fpArticles[n]['articleMetadata']['sortNumber']);
    if (fpArticles[n]['articleMetadata']['tags'].indexOf('DPSBridge-') >= 0
        || fpArticles[n]['articleMetadata']['tags'].indexOf('DPSBridge') < 0) {
        sortNumbers[indexSort] = sortNumber;
        indexSort ++;
    }
  }
  sortNumbers.sort();

  // Display Drupal/non-Drupal articles that were previously stored in Drupal.
  for (var i = 0; i < drupalArticles.length; i++) {
    var articleID = drupalArticles[i]['target_id'];
    jQuery.ajax({
      url: baseURL + "/dpsbridge/folio/pull-article",
      type: "POST",
      async: false,
      data: { "nodeID":articleID },
      success: function(output) {
        var html = '', checked = '';
        // Check the checkbox if the article is an ad.
        if (isAds[i] == true) {
          checked = "checked";
        }
        // Create a locked row if the content is not from Drupal.
        if (output.length === 0) {
          // Makes sure to not show non-Drupal articles, deleted from FP.
          var exist = dpsbridge_helper_check_article_by_id(fpArticles, alienatedID[alienatedCount]);
          if (exist) {
            html += "<tr id='article-row-id-" + alienatedID[alienatedCount] + "'>\n";
            html += "<td class='article-index'><input type='hidden' value='" + alienatedID[alienatedCount] + "' /><span class='ui-icon ui-icon-locked'></span></td>\n";
            html += "<td><span class='ui-icon ui-icon-arrow-4'></span></td>\n";
            html += "<td class='sortable-index' style='text-align:center'>" + index + "</td>\n";
            html += "<td><span class='ui-icon ui-icon-locked'></span></td>\n";
            html += "<td>" + exist['name'] + "</td>\n";
            html += "<td>" + exist['type'] + "</td>\n";
            html += "<td><span class='ui-icon ui-icon-locked'></span></td>\n";
            html += "<td class='is-ad'><input type='hidden' /><span class='ui-icon ui-icon-locked'></span></td>\n";
            html += "<td><span class='ui-icon ui-icon-check'></span></td>\n";
            html += "</tr>\n";
            index++;
          }
          alienatedCount++;
        }
        // Create a editable row if the content is from Drupal.
        else {
          // Check if the Drupal article is up in FP.
          var sortNumber = dpsbridge_helper_get_sort_number_by_nid(fpArticles, articleID);
          html += "<tr id='article-row-id-" + articleID + "'>\n";
          html += "<td class='article-index'><input type='checkbox' value='" + articleID + "' /></td>\n";
          html += "<td><span class='ui-icon ui-icon-arrow-4'></span></td>\n";
          html += "<td id='articleSortIndex-" + sortNumber + "' class='sortable-index' style='text-align:center'>" + index + "</td>\n";
          html += "<td><a href='/node/" + articleID + "/edit?destination=admin/config/content/fpmanage'><span class='ui-icon ui-icon-pencil'></span></a></td>\n";
          html += "<td><a href='javascript:previewOptions(" + articleID + ", \"" + output['filename'] + "\")'>" + output['title'] + "</a></td>\n";
          html += "<td>" + output['type'] + "</td>\n";
          html += "<td>" + output['modified'] + "</td>\n";
          html += "<td class='is-ad'><input type='checkbox' " + checked + "/></td>\n";
          if (output['timestamp'] <= uploadDate && exist) {
            html += "<td><span class='ui-icon ui-icon-check'></span></td>\n";
          }
          else {
            html += "<td><span class='ui-icon ui-icon-close'></span></td>\n";
          }
          html += "</tr>\n";
          index++;
        }
        jQuery('#articles-wrapper').append(html);
      }
    });
  }
  // Display all non-Drupal articles that has not been stored previously.
  for (var n = 0; n < fpArticles.length; n++) {
    // Skip if it is a Drupal article.
    if (fpArticles[n]['articleMetadata']['tags'].indexOf('DPSBridge') >= 0) {
      continue;
    }
    // Insert non-Drupal articles that hasn't been added to Drupal database.
    if (jQuery('#article-row-id-' + fpArticles[n]['id']).length == 0) {
      var sortNumber  = fpArticles[n]['articleMetadata']['sortNumber'];
      var sortIndex = sortNumbers.indexOf(sortNumber);
      html  = "<tr id='article-row-id-" + fpArticles[n]['id'] + "'>\n";
      html += "<td class='article-index'><input type='hidden' value='" + fpArticles[n]['id'] + "' /><span class='ui-icon ui-icon-locked'></span></td>\n";
      html += "<td><span class='ui-icon ui-icon-arrow-4'></span></td>\n";
      html += "<td id='articleSortIndex-" + sortNumber + "' class='sortable-index' style='text-align:center'>" + index + "</td>\n";
      html += "<td><span class='ui-icon ui-icon-locked'></span></td>\n";
      html += "<td>" + fpArticles[n]['articleMetadata']['title'] + "</td>\n";
      html += "<td>" + fpArticles[n]['articleMetadata']['assetFormat'] + "</td>\n";
      html += "<td><span class='ui-icon ui-icon-locked'></span></td>\n";
      html += "<td class='is-ad'><input type='hidden' /><span class='ui-icon ui-icon-locked'></span></td>\n";
      html += "<td><span class='ui-icon ui-icon-check'></span></td>\n";
      html += "</tr>\n";
      if (sortIndex == 0) {
        jQuery('#articles-wrapper').prepend(html);
      }
      else if (sortIndex == (sortNumbers.length - 1)) {
        jQuery('#articles-wrapper').append(html);
      }
      else {
        if (preSortNumber = sortNumbers[sortIndex - 1]) {
          if (jQuery('#articleSortIndex-' + preSortNumber).length > 0) {
            jQuery('#articleSortIndex-' + preSortNumber).parent().after(html);
            continue;
          }
        }
        if (nextSortNumber = sortNumbers[sortIndex + 1]) {
          if (jQuery('#articleSortIndex-' + nextSortNumber).length > 0) {
            jQuery('#articleSortIndex-' + nextSortNumber).parent().before(html);
            continue;
          }
        }
        jQuery('#articles-wrapper').append(html);
      }
      index++;
    }
  }
  jQuery('#articles-wrapper tr').each(function(i, value){
    jQuery(this).find('td.sortable-index').text(i + 1);
  });
  // Refreshes the stylesheets.
  jQuery('#dialog-status').dialog('close');
  jQuery('#dialog-edit-folio').dialog('option', 'title', folioName).dialog('open');
}

/**
 * Pulls all information regarding the targeted Folio node from the database.
  */
function profileUI(folioNodeID, toggle) {
  fid = folioNodeID;
  dpsbridge_helper_show_status("Pulling content, please wait...", 400, 225);
  jQuery('#articles-wrapper').empty();
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/pull-content",
    type: "POST",
    data: {
      "nodeID":folioNodeID,
      "toggle":'sync' },
    success: function(output) {
        amazonDimensions  = output['amazonDimensions'];
        androidDimensions = output['androidDimensions'];
        appleDimensions   = output['appleDimensions'];
      offsetIndex = 0;
      var landscapeImg = (output['landscape'] != '') ? '/' + output['landscape'] : 'http://placehold.it/300x400&text=Cover+Landscape';
      var portraitImg = (output['portrait'] != '') ? '/' + output['portrait'] : 'http://placehold.it/400x300&text=Cover+Portrait';
      // Insert new attributes into the current overlay.
      jQuery('#portrait').attr('src', portraitImg);
      jQuery('#landscape').attr('src', landscapeImg);
      jQuery('#image').attr('src', landscapeImg);
      jQuery('#image-ui-fid').val(folioNodeID);
      jQuery('#image-ui-fname').val(output['title']);
      // Toggle for opening the Folio edit UI.
      if (toggle == 'full') {
        jQuery("#folio-ui-fname").val(output['title']);
        jQuery("#folio-ui-pname").val(output['pubName']);
        jQuery("#folio-ui-fnumber").val(output['folioNum']);
        jQuery("#folio-ui-pdate").val(output['pubDate']);
        jQuery("#folio-ui-fdesc").val(output['folioDesc']);
        jQuery("#folio-ui-filter").val(output['filter']);
        jQuery('#folio-ui-fversion').val(output['folioVer']);
        jQuery('#folio-ui-orientation').val(output['orientation']);
        jQuery('#folio-ui-generator').val(output['autoToggle']);
        jQuery('#accounts').val(output['accounts']);
        // Updates the dimension field depending on the default account choice.
        refreshDimensions();
        jQuery('#dimension').val(output['dimension']);
        // Disable account & dimension selection if published.
        if (output['status'] == 'Uploaded') {
          jQuery('#accounts').attr('disabled', 'disabled');
          jQuery('#dimension').attr('disabled', 'disabled');
          jQuery('#folio-ui-orientation').attr('disabled', 'disabled');
        }
        // Re-enable account & dimension selection if not published.
        else {
          jQuery('#accounts').removeAttr('disabled');
          jQuery('#dimension').removeAttr('disabled');
          jQuery('#folio-ui-orientation').removeAttr('disabled');
        }
        if (landscapeImg || portraitImg) {
          offsetIndex++;
        }

        // Sync with the Folio Producer.
        if (output['pubID']) {
          fp_sync(
            output['pubID'],
            output['title'],
            output['credentials'],
            output['articles'],
            output['isAds'],
            output['alienated'],
            output['uploadDate']
          );
        } else {
          dpsbridge_helper_pull_articles(
            output['title'],
            output['articles'],
            output['isAds'],
            output['alienated'],
            output['uploadDate'],
            ''
          );
        }
        readStylesheets();
        jQuery('#pubcss').val(output['pubCSS']);
        jQuery('.overridecss').val(output['pubCSS']);
      }
      // Toggle for opening the image edit UI.
      else if (toggle == 'half') {
        jQuery('#dialog-status').dialog('close');
        jQuery('#dialog-edit-cover').dialog('option', 'title', output['title']).dialog('open');
      }
    }
  });
}
/**
 * Generate a temperary HTML article in /dpsbridge/html folder.
 */
function previewOptions(previewID, filename) {
  var pname  = jQuery("#folio-ui-pname").val(),
    pubcss = jQuery("#pubcss").val();
    previewFileName = filename;
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/preview-article",
    type: "POST",
    data: {
      "previewID"  :previewID,
      "pname"    :pname,
      "pubcss"  :pubcss },
    success: function(output) {
      jQuery('#dialog-iframe-preview-option').dialog('option', 'title', output['title']).dialog('open');
    }
  });
}
/**
 * Open a new window with the designated width and height preview dialog.
 */
function previewArticle(orientation) {
  var windows_width, windows_height, previewURL,
    preview_dimension = jQuery('#preview-dimension :selected').val(),
    preview_token = preview_dimension.split(',');
  // View landscape mode.
  if (orientation == "landscape") {
    windows_width = preview_token[0];
    windows_height = preview_token[1];
  }
  // View portrait mode.
  else {
    windows_width = preview_token[1];
    windows_height = preview_token[0];
  }
  previewURL = pathToDir + '/html/' + previewFileName + '/' + previewFileName + '.html';
  window.open(previewURL, "popup", "width=" + windows_width + ", height=" + windows_height);
}
/**
 * Read Stylesheets.
 */
function readStylesheets() {
  var styleText = '';
  jQuery.ajax({
    url: baseURL + "/dpsbridge/stylesheet/read",
    type: "POST",
    async: false,
    success: function(output) {
      for (var i = 0; i < output.length; i++) {
        if (output[i].indexOf('-') < 0) {
          styleText = output[i];
        }
        // Removes the derivatives for viewing purposes.
        else {
          styleText = output[i].split(/-/)[1];
        }
        jQuery("#pubcss").append(jQuery("<option></option>").attr("value", output[i]).text(styleText));
      }
    }
  });
}
/**
 * Depending on the selected account type, display the dimensions accordingly.
 */
function refreshDimensions() {
  var account = jQuery('#accounts :selected')
  jQuery('#dimension').empty();
  switch(account.val()) {
    case 'amazon':
      dpsbridge_helper_generate_dimensions('dimension', amazonDimensions, true);
      break;

    case 'android':
      dpsbridge_helper_generate_dimensions('dimension', androidDimensions, true);
      break;

    case 'apple':
    default:
      dpsbridge_helper_generate_dimensions('dimension', appleDimensions, true);
      break;
  }
}
/**
 * Recursively remove all HTML article folders and .folio files.
 */
function removePreviewFile() {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/preview-delete",
    type: "POST",
    data: { "filename":previewFileName }
  })
}

/**
 * Given a set of edit buttons, replace them with javascript calls.
 */
function replaceHREF(ahref, toggle) {
  for (var i = 0; i < ahref.length; i++) {
    var url = ahref[i].href;
    ahref[i].href = 'javascript:profileUI(' + url.substring(url.indexOf('/node') + 6, url.indexOf('/edit?')) + ', "' + toggle + '")';
  }
}

/* /-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\ *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * Constructing the Jquery UI Overlay Windows *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * \-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/ */
(function ($) {
    Drupal.behaviors.dpsbridge_view_credential_init = {
        attach: function() {
            baseURL = Drupal.settings.dpsbridge.base_url;
            pathToDir = baseURL + '/' + Drupal.settings.dpsbridge.path_to_dir;
        }
    }
    Drupal.behaviors.dpsbridge_view_select = {
        attach: function() {
            var fname       = $("#folio-ui-fname"),
                pname       = $("#folio-ui-pname"),
                fnumber     = $("#folio-ui-fnumber"),
                pdate       = $("#folio-ui-pdate"),
                fdesc      = $("#folio-ui-fdesc"),
                filter       = $("#folio-ui-filter");
                fversion    = $('#folio-ui-fversion'),
                orientation = $('#folio-ui-orientation'),
                autoToggle  = $('#folio-ui-generator'),
                image       = $('#image'),
                portrait    = $('#portrait'),
                landscape   = $('#landscape');
            // Dialog box functionality for editing cover images.
            $("#dialog-edit-cover").dialog({
                autoOpen:false, height:525, width:570, modal:true,
                buttons: {
                    Save: function() {
                        var form = document.getElementById('image-form');
                        var formData = new FormData(form);
                        var xhr = new XMLHttpRequest();
                        xhr.open('POST', form.getAttribute('action'), false);
                        xhr.send(formData);
                        imageUI_update();
                        $(this).dialog("close"); },
                    Cancel: function() { $(this).dialog("close"); }}
            });
            // Dialog box functionality for editing folio.
            $("#dialog-edit-folio").dialog({
                autoOpen:false, height:666, width:1100, modal:true,
                buttons: {
                    "Export Selected Articles as HTML stacks": function() {
                        var articleID  = '';
                        checkboxes = $('.article-index input');
                        for (var n = 0; n < checkboxes.length; n++) {
                          if (checkboxes[n].checked) {
                            articleID += checkboxes[n].value + ',';
                          }
                        }
                        if (articleID) {
                          generate_selected_html(articleID);
                        }
                        else {
                          dpsbridge_helper_show_status("Please select an article first!");
                        }
                    },
                    "Remove Selected Articles from Folio": function() {
                        var indexes           = $('td.sortable-index'),
                            checkboxes        = $('.article-index input');
                        for (var i = 0; i < checkboxes.length; i++) {
                          if (checkboxes[i].checked) {
                            $('#article-row-id-' + checkboxes[i].value).remove();
                          }
                        }
                        for (var n = 0; n < indexes.length; n++) {
                          indexes[n].innerHTML = n + 1;
                        }
                    },
                    "Save": function() {
                        var ads      = "",
                            articles = "",
                            indexes    = $('.article-index input'),
                            isad      = $('.is-ad input'),
                            dimension  = $('#dimension :selected'),
                            pubcss     = $('#pubcss'),
                            toccss     = $('#toccss'),
                            account    = $('#accounts :selected');
                        // Appends the current articles from folio.
                        for (var i = 0; i < indexes.length; i++) {
                          articles += indexes[i].value + ",";
                        }
                        for (var n = 0; n < isad.length; n++) {
                          ads += (isad[n].checked)?"1,":"0,";
                        }
                        // Attempts to save the metadata to the selected Folio node.
                        $.ajax({
                            url: baseURL + "/dpsbridge/folio/update",
                            type: "POST",
                            data: {
                                "fid"         :fid,
                                "fname"       :fname.val(),
                                "pname"       :pname.val(),
                                "fnumber"     :fnumber.val(),
                                "pdate"       :pdate.val(),
                                "fdesc"       :fdesc.val(),
                                "dimension"   :dimension.val(),
                                "orientation" :orientation.val(),
                                "autoToggle"  :autoToggle.val(),
                                "filter"      :filter.val(),
                                "fversion"    :fversion.val(),
                                "pubcss"      :pubcss.val(),
                                "toccss"      :toccss.val(),
                                "isads"       :ads,
                                "accounts"    :account.val(),
                                "articles"    :articles },
                            success: function(output) {
                                if (output === 'ok') {
                                    window.location = baseURL + "/admin/config/content/fpmanage";
                                }
                                else {
                                    dpsbridge_helper_show_status("Failed to update '" + fname.val() + "'<br/><br/>:: " + output, 400, 300);
                                }
                            },
                            error: function (e, status) {
                                    dpsbridge_helper_show_status(e.responseText);
                            }
                        });
                        $(this).dialog("close");
                    },
                    Cancel: function() { $(this).dialog("close"); }
                }
            });
            // Dialog box functionality for providing previewing option (landscape or portrait).
            $("#dialog-iframe-preview-option").dialog({
                    autoOpen:false, height:400, width:375, modal:true,
                    buttons: {
                        Back: function() {
                            removePreviewFile();
                            $(this).dialog("close"); }},
                    close: function() { removePreviewFile(); }
            });
            // Dialog box functionality for providing previewing option (landscape or portrait).
            $("#dialog-status").dialog({
                    autoOpen:false, modal:true,
                    buttons: {
                        Close: function() {
                            $(this).dialog("close"); }}
            });
            // Auto sort the table.
            $("#sortable-table tbody").sortable({
                    update: function(event, ui) {
                        $('#sortable-table tbody tr').each(function() {
                            $(this).children('td.sortable-index').html($(this).index() + 1); }); }
            });
            // Auto resizing the dialog boxes depending on the size of the browser.
            $(window).resize(function() {
            $("#dialog-edit-folio").dialog("option", "width", "auto");
            $("#dialog-edit-cover").dialog("option", "width", "auto");
            $("#dialog-iframe-preview-option").dialog("option", "width", "auto");
            });

            $("#jqueryui-tabs").tabs();

            var container = $('#block-system-main');
            var button_wrapper = $('<div/>').css({'float':'left', 'margin-bottom':'15px'});
            var button_clone = $('<button/>').text('Clone Selected Folio').click(function() { get_selected('clone'); });
            var button_delete = $('<button/>').text('Delete Selected Folio').click(function() { get_selected('delete'); });
            var button_upload = $('<button/>').text('Upload Selected Folio to Folio Producer').click(function() { get_selected('upload'); });
            button_wrapper.append(button_clone, button_delete, button_upload);
            container.append(button_wrapper);
            replaceHREF($('table td.views-field-edit-node a'), 'full');
            replaceHREF($('table td.views-field-edit-node-1 a'), 'half');
        }
    }
})(jQuery);
