/**
 * View Article Scripting.
 */

//var articles = "";
var baseURL   = "",
    pathToDir = "";
/**
 * Pull the account info and article's metadata
 */
function acquire_account_credentials(articleNodeID, filenames) {
  jQuery.ajax({
    url: baseURL + "/dpsnext/article/pull-accounts",
    type: "POST",
    data: { "articleNodeID":articleNodeID },
    success: function(output) {
      /*if (!output['folioMeta']['dimension']) {
        dpsbridge_helper_show_status("Please select a resolution for the folio!");
        dpsbridge_helper_delete_files(baseURL, filenames);
      }*/ 
      if (output['message'] == 'ok') {
        dpsbridge_helper_update_status("Connecting to Producer...");
        // No authentication for now
        //fp_connect(output['account'], output['folioMeta'], filenames);
        
         // Check if article is uploaded before
         if (output['articleVersion'] != '') {
           // If article was uploaded, update metadata
           dpsbridge_helper_update_status("Updating article metadata \"" + output['articleMeta']['title'] + "\" on the producer...");
           fp_update(output['account'], output['articleMeta'], filenames, output['articleVersion']);
         } else {
           // If Article was not uploaded, create metada  
           dpsbridge_helper_update_status("Creating article metadata \"" + output['articleMeta']['title'] + "\" on the producer...");
           fp_create(output['account'], output['articleMeta'], filenames);    
         }
      } else {
        dpsbridge_helper_show_status(output['message']);
        //dpsbridge_helper_delete_files(baseURL, filenames);
      }
    }
  });
}        
/**
 * Convert the folder into .article file.
 */
function generate_article(articleNodeID, filenames) {
  jQuery.ajax({
    url: baseURL + "/dpsnext/article/generate-article",
    type: "POST",
    data: { "filenames":filenames },
    success: function(output) {
      if (output == 'ok') {
         dpsbridge_helper_update_status("Preparing data to upload...");  
         acquire_account_credentials(articleNodeID, filenames);
      } else {
         dpsbridge_helper_update_status(output);    
      }    
    }
  });
}    
/**
 * Generate a HTML article folder.
 */
function generate_html(articleNodeID) {
  jQuery.ajax({
    url: baseURL + "/dpsnext/article/generate-HTML",
    type: "POST",
    data: { "articleNodeID":articleNodeID },
    success: function(output) {    
      if (output['message'] == 'ok') {
        dpsbridge_helper_update_status("Converting Article into .article file...");    
        generate_article(articleNodeID, output['files']);
      }  else {
        dpsbridge_helper_show_status(output['message']);  
      }
    }
  });
}
/**
 * -- Currently Not used ---
 * Save version id for a node
 */
function save_version_id(articleMeta, versionID) {
  jQuery.ajax({
    url: baseURL + "/dpsnext/node/set-version",
    type: "POST",
    data: { 
        "articleNodeID":articleMeta['node']['nid'],
        "versionID":versionID
    },
    success: function(output) {    
      if (output != '') {
        fp_upload(articleMeta, versionID);
       
      }  else {
        //dpsbridge_helper_show_status();  
      }
    }
  });  
}
/* ----------------------------------------------------------- *
 * Attempts to obtain the values from the selected checkboxes,
 *  toggles the overlay window if checkboxes are selected
 * ----------------------------------------------------------- */
function get_selected(toggle) {
  var selectedArticle = jQuery('.views-table tr input:checked');
  if (!selectedArticle.val()) {
    dpsbridge_helper_show_status("Please select an article first!");
  } else {
    switch(toggle) {
      /*case 'clone':
        dpsbridge_helper_folio_clone(baseURL, selectedFolio.val());
        break;

      case 'delete':
        dpsbridge_helper_delete_node(baseURL, selectedFolio.val());
        break;*/

      case 'upload':
        dpsbridge_helper_show_status('Generating HTML article...', 600, 500);
        generate_html(selectedArticle.val());
        break;
    }
  }
}
/* ============================================================= *
 * Given the article metadata,
 *   attempts to create article metadata in the Producer via the API
 * ============================================================= */
function fp_create(accountMeta, articleMeta, filenames) {
    jQuery.ajax({
      url: baseURL + "/dpsnext/article/create",
      type: "POST",
      data: {
        'articleMeta' : articleMeta
      },
      success: function(output) {
        if (output['message'] == 'ok') {
          dpsbridge_helper_update_status("Uploading .article file to the producer...");   
          fp_upload(articleMeta, output['version_id']);
        }
        else {
          dpsbridge_helper_update_status(output['message'])  
        }
      }
    });
}

/* ============================================================= *
 * Given the article metadata,
 *   attempts to update article metadata in the Producer via the API
 * ============================================================= */
function fp_update(accountMeta, articleMeta, filenames, versionID) {
    jQuery.ajax({
      url: baseURL + "/dpsnext/article/update",
      type: "POST",
      data: {
        'articleMeta' : articleMeta,
        'versionID' : versionID
      },
      success: function(output) {
        if (output['message'] == 'ok') {
          dpsbridge_helper_update_status("Uploading .article file to the producer...");  
          //save_version_id(articleMeta, output);  
          fp_upload(articleMeta, output['version_id']);  
        }
        else {
          dpsbridge_helper_update_status(output['message'])  
        }
      }
    });
}

/* ============================================================= *
 * Given the article metadata,
 *   attempts to upload .article file on the Producer via the API
 * ============================================================= */
function fp_upload(articleMeta, versionID) {
    jQuery.ajax({
      url: baseURL + "/dpsnext/article/upload",
      type: "POST",
      data: {
        'articleMeta' : articleMeta,
        'versionID' : versionID
      },
      success: function(output) {
        if (output['message'] == 'ok') {
          dpsbridge_helper_update_status("Success!");   
        }
        else {
          dpsbridge_helper_update_status(output['message'])  
        }
      }
    });
}

/* ----------------------------------------- *
 * Constructing the Jquery UI Overlay Window
 * ----------------------------------------- */

(function ($) {
    Drupal.behaviors.dpsnext_view_article = {
        attach: function() {
            baseURL = Drupal.settings.dpsbridge.base_url;
            pathToDir = baseURL + '/' + Drupal.settings.dpsbridge.path_to_dir;
            
            $("#dialog-status").dialog({
                autoOpen:false, modal:true,
                buttons: {
                    Close: function() {
                        $(this).dialog("close"); }}
            });
            $("#jqueryui-tabs").tabs();

            var container = $('#block-system-main');
            var button_wrapper = $('<div/>').css({'float':'left', 'margin-bottom':'15px'});
            var button_upload = $('<button/>').text('Upload Selected Article to Producer').click(function() { get_selected('upload') });
            button_wrapper.append(button_upload)
            container.append(button_wrapper);
        }
    }
})(jQuery);


