
/**
 * Given an array of values,
 *   convert it to string of values separated by commas
 * @param {array} dimensionArray ..the array list of values
 */
function helper_array_to_string(dimensionArray) {
	var dimensionString = "";
	for (var i = 0; i < dimensionArray.length; i++)
		dimensionString += dimensionArray[i].value+",";
	return dimensionString;
}
/**
 * Given an array of article and the targeted article ID,
 *   check to see if the targeted article name exists within the array of articles
 * @param {array} articleList ..array list of articles
 * @param {string} articleID ...single article ID
 * @return {string} name of the non-Drupal article if found, empty string otherwise
 */
function helper_check_article_by_id(articleList, articleID) {
	for (var i = 0; i < articleList.length; i++) {
		if (articleList[i]['id'] == articleID) {
			article = new Array();
			article['name'] = articleList[i]['articleMetadata']['title'];
			article['type'] = articleList[i]['articleMetadata']['assetFormat'];
		}
	}
	return '';
}
/**
 * Given an array of article and the targeted article name,
 *   check to see if the targeted article name exists within the array of articles
 * @param {array} articleList ..array list of articles
 * @param {string} article .....single article name
 * @return {boolean} true if found, false otherwise
 */
function helper_check_article_by_name(articleList, article) {
	for (var i = 0; i < articleList.length; i++)
		if (articleList[i]['name'] == article)
			return true;
	return false;
}
/* ===================================================== *
 * Given the designated tag ID and a list of dimensions,
 *   generates each dimension as an option tag,
 *   and appends each to the designated tag.
 *   Toggle is for the different type of dimension array.
 * ===================================================== */
function helper_generate_dimensions(id, dimensions, toggle) {
	for (var i = 0; i < dimensions.length; i++) {
		if (dimensions[i] == '') // do nothing if array is empty
			continue;
		if (toggle) // for inputing the dimension values on load
			$('#'+id).append('<option value="'+dimensions[i]+'">'+dimensions[i]+'</option>');
		else // for the delete dimensions window
			$('#'+id).append('<option value="'+dimensions[i].value+'">'+dimensions[i].value+'</option>');
	}
}
/**
 * Given the folio node ID, 
 *   make AJAX call to duplicate the Folio node.
 *   Refreshes the page upon success.
 * @param {string} baseURL ......the webhost base URL
 * @param {string} folioNodeID ..the folio node ID
 */
function helper_folio_clone(baseURL, folioNodeID) {
	$.ajax({
		url: baseURL+"/dpsbridge/folio/clone-node",
		type: "POST",
		data: { "fid":folioNodeID },
		success: function(output) {
			if (output === 'ok')
				window.location = baseURL+"/admin/config/content/fpmanage";
			else
				helper_show_status(output);
		}
	});
}
/**
 * Given the list of file names, 
 *   make AJAX call to delete the generated HTML article folders and .folio files.
 * @param {string} baseURL ...the webhost base URL
 * @param {array} filenames ..the array list of file names
 */
function helper_delete_files(baseURL, filenames) {
	$.ajax({
		url: baseURL+"/dpsbridge/folio/clean-up",
		type: "POST",
		data: { "filenames":filenames }
	});
}
/**
 * Given the node ID, 
 *   make AJAX call to delete the node from the Drupal database.
 * @param {string} baseURL ..the webhost base URL
 * @param {string} nodeID ...the node ID
 */
function helper_delete_node(baseURL, nodeID) {
	$.ajax({
		url: baseURL+"/dpsbridge/folio/delete-node",
		type: "POST",
		data: { "fid":nodeID },
		success: function(output) {
			if (output === 'ok')
				window.location = baseURL+"/admin/config/content/fpmanage";
			else
				helper_show_status(output);
		}
	});
}
/**
 * Download the designated file from the server.
 * @param {string} baseURL ...the webhost base URL
 * @param {string} filepath ..the path to the targeted file
 * @param {string} filename ..the name of the targeted file
 * @param {string} toggle ....trigger to delete the selected file
 */
function helper_download_file(baseURL, filepath, filename, toggle) {
	$('#dialog-upload-status').dialog('close');
	$('<form>').attr('method', 'post')
			   .attr('action', baseURL+'/dpsbridge/folio/download-selected')
			   .append('<input name="filename" value="'+filename+'" />')
			   .append('<input name="destination" value="'+filepath+'" />')
			   .append('<input name="toggle" value="'+toggle+'" />')
			   .submit();
}
/**
 * Given the message, width (optional), and height (optional),
 *   delete any existing contents from previous dialog box
 *   and re-open it with the given message.
 * @param {string} message ..the message for the dialog box
 * @param {string} width ....the width of the dialog box
 * @param {string} height ...the height of the dialog box
 */
function helper_show_status(message, width, height) {
	width  = (width)?width:350;
	height = (height)?height:200;
	$('#status').empty();
	$('#status').append("<br/><p>"+message+"</p>\n");
	$('#dialog-status').dialog('open').dialog("option", "width", width).dialog("option", "height", height);
}

/**
 * Given the message (string), 
 *   append to the status dialog box.
 * @param {string} message ..the message for the dialog box
 */
function helper_update_status(message) {
	$('#status').append("<p>"+message+"</p>\n");
}

