/**
 * Main Scripting.
 */

/**
 * Given an array of values, convert it to string of values separated by commas.
 *
 * @param {array} dimensionArray
 *   The array list of values.
 */
function dpsbridge_helper_array_to_string(dimensionArray) {
  var dimensionString = "";
  for (var i = 0; i < dimensionArray.length; i++) {
    dimensionString += dimensionArray[i].value + ",";
  }
  return dimensionString;
}
/**
 * Check to see if targeted article name exists within the array of articles.
 *
 * @param {array} articleList
 *   Array list of articles.
 * @param {string} articleID
 *   Single article ID.
 *
 * @return {string}
 *   Name of the non-Drupal article if found, empty string otherwise.
 */
function dpsbridge_helper_check_article_by_id(articleList, articleID) {
  for (var i = 0; i < articleList.length; i++) {
    if (articleList[i]['id'] == articleID) {
      article = new Array();
      article['name'] = articleList[i]['articleMetadata']['title'];
      article['type'] = articleList[i]['articleMetadata']['assetFormat'];
    }
  }
  return '';
}
function dpsbridge_helper_get_sort_number_by_nid(articleList, nid) {
  var tag = 'DPSBridge-' + nid;
  for (var i = 0; i < articleList.length; i++) {
    if (articleList[i]['articleMetadata']['tags'].indexOf(tag) >= 0) {
        return articleList[i]['articleMetadata']['sortNumber'];
    }
  }
  return 0;
}
/**
 * Check to see if targeted article name exists within the array of articles.
 *
 * @param {array} articleList
 *   Array list of articles.
 * @param {string} article
 *   Single article name.
 *
 * @return {boolean}
 *   True if found, false otherwise.
 */
function dpsbridge_helper_check_article_by_name(articleList, article) {
  for (var i = 0; i < articleList.length; i++) {
    if (articleList[i]['name'] == article) {
      return true;
    }
  }
  return false;
}
/**
 * Generates dimension as an option tag, and appends to the designated tag.
 */
function dpsbridge_helper_generate_dimensions(id, dimensions, toggle) {
  for (var i = 0; i < dimensions.length; i++) {
    // Do nothing if array is empty.
    if (dimensions[i] == '') {
      continue;
    }
    // For inputing the dimension values on load.
    if (toggle) {
      jQuery('#' + id).append('<option value="' + dimensions[i] + '">' + dimensions[i] + '</option>');
    }
    // For the delete dimensions window.
    else {
      jQuery('#' + id).append('<option value="' + dimensions[i].value + '">' + dimensions[i].value + '</option>');
    }
  }
}
/**
 * Make AJAX call to duplicate the Folio node and refresh the page upon success.
 *
 * @param {string} baseURL
 *   The webhost base URL.
 * @param {string} folioNodeID
 *   The folio node ID.
 */
function dpsbridge_helper_folio_clone(baseURL, folioNodeID) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/clone-node",
    type: "POST",
    data: { "fid":folioNodeID },
    success: function(output) {
      if (output === 'ok') {
        window.location = baseURL + "/admin/config/content/fpmanage";
      }
      else {
        dpsbridge_helper_show_status(output);
      }
    }
  });
}
/**
 * Make AJAX call to delete the generated HTML article folders and .folio files.
 *
 * @param {string} baseURL
 *   The webhost base URL.
 * @param {array} filenames
 *   The array list of file names.
 */
function dpsbridge_helper_delete_files(baseURL, filenames) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/clean-up",
    type: "POST",
    data: { "filenames":filenames }
  });
}
/**
 * Make AJAX call to delete the node from the Drupal database.
 *
 * @param {string} baseURL
 *   The webhost base URL.
 * @param {string} nodeID
 *   The node ID.
 */
function dpsbridge_helper_delete_node(baseURL, nodeID) {
  jQuery.ajax({
    url: baseURL + "/dpsbridge/folio/delete-node",
    type: "POST",
    data: { "fid":nodeID },
    success: function(output) {
      if (output === 'ok') {
        window.location = baseURL + "/admin/config/content/fpmanage";
      }
      else {
        dpsbridge_helper_show_status(output);
      }
    }
  });
}
/**
 * Download the designated file from the server.
 *
 * @param {string} baseURL
 *   The webhost base URL.
 * @param {string} filepath
 *   The path to the targeted file.
 * @param {string} filename
 *   The name of the targeted file.
 * @param {string} toggle
 *   Trigger to delete the selected file.
 */
function dpsbridge_helper_download_file(baseURL, filepath, filename, toggle) {
  jQuery('#dialog-upload-status').dialog('close');
  jQuery('<form>').attr('method', 'post')
         .attr('action', baseURL + '/dpsbridge/folio/download-selected')
         .append('<input name="filename" value="' + filename + '" />')
         .append('<input name="destination" value="' + filepath + '" />')
         .append('<input name="toggle" value="' + toggle + '" />')
         .submit();
}
/**
 * Delete any existing contents from previous dialog box and reopen it.
 *
 * @param {string} message
 *   The message for the dialog box.
 * @param {string} width
 *   The width of the dialog box.
 * @param {string} height
 *   The height of the dialog box.
 */
function dpsbridge_helper_show_status(message, width, height) {
  width  = (width)?width:350;
  height = (height)?height:200;
  jQuery('#status').empty();
  jQuery('#status').append("<br/><p>" + message + "</p>\n");
  jQuery('#dialog-status').dialog('open').dialog("option", "width", width).dialog("option", "height", height);
}

/**
 * Append the message to the status dialog box.
 *
 * @param {string} message
 *   The message for the dialog box.
 */
function dpsbridge_helper_update_status(message) {
  jQuery('#status').append("<p>" + message + "</p>\n");
}
