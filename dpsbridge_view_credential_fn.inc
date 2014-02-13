<script>
var apikey,            // Folio Producer API key 
	apisec,            // Folio Producer API secret
	amazon_id,         // Amazon - Folio Producer user ID
	amazon_pass,       // Amazon - Folio Producer user password
	amazon_dimension,  // Amazon - available dimensions
	android_id,        // Android - Folio Producer user ID
	android_pass,      // Android - Folio Producer user password
	android_dimension, // Android - available dimensions
	apple_id,          // Apple - Folio Producer user ID
	apple_pass,        // Apple - Folio Producer user password
	apple_dimension,   // Apple - available dimensions
	baseURL = "<?= $GLOBALS['base_url'] ?>",
  pathToDir = "<?= drupal_get_path('module', 'dpsbridge') ?>";
/* =================================================== *
 * Given the account type,
 *   call helper to connect to Folio Producer,
 *   with the credentials associated with the account.
 * =================================================== */
function connectivity(Account) {
	updateFields();
	switch (Account) {
		case 'Amazon': // test Amazon account
			dps_connect(amazon_id.val(), amazon_pass.val(), apikey.val(), apisec.val(), 'Amazon', 'true');
			break;
		case 'Android': // test Android account
			dps_connect(android_id.val(), android_pass.val(), apikey.val(), apisec.val(), 'Android', 'true');
			break;
		case 'Apple': // test Apple account
			dps_connect(apple_id.val(), apple_pass.val(), apikey.val(), apisec.val(), 'Apple', 'true');
			break;
	}
}
/* ========================================================= *
 * Helper method for testing the Folio Producer credentials.
 * ========================================================= */
function dps_connect(AdobeID, AdobePass, APIKey, APISecret, type, toggle) {
	helper_show_status('Attempting to connect to Folio Producer, please wait...');
	$.ajax({
		url: pathToDir+"/fp_connect.php",
		type: "POST",
		data: {
			"AdobeID"   :AdobeID,   // Folio Producer user ID
			"Password"  :AdobePass, // Folio Producer user password
			"APIKey"    :APIKey,    // Folio Producer API key
			"APISecret" :APISecret, // Folio Producer API secret
			"Test"		:toggle,    // toggle for stating that this is a test
		},
		success: function(output) {
			if (output === 'ok')
				helper_show_status("<strong><em>"+type+"</em></strong><br/><br/>-Folio Producer API authentication successful!<br/><br/>- Login successful!<br/><br/>- Test completed!", 350, 400);
			else
				helper_show_status("<strong><em>"+type+"</em></strong><br/><br/>:: "+output, 600, 250);
			logout();
		}
	});
}
/* =============================================== *
 * Insert/update the credentials in the user table
 * =============================================== */
function dps_credentials() {
	updateFields();
	helper_show_status('Updating credentials, please wait...');
	// call helper to convert the array of dimensions into a string, separated by commas
	var amazon_target_dimension = helper_array_to_string(amazon_dimension);
	var android_target_dimension = helper_array_to_string(android_dimension);
	var apple_target_dimension = helper_array_to_string(apple_dimension);

	$.ajax({
		url: baseURL+"/dpsbridge/dps/credentials/add",
		type: "POST",
		data: { 
			"apikey"           :apikey.val(),
			"apisec"           :apisec.val(),
			"amazon_id"        :amazon_id.val(),
			"amazon_pass"      :amazon_pass.val(),
			"amazon_dimension" :amazon_target_dimension,
			"android_id"       :android_id.val(),
			"android_pass"     :android_pass.val(),
			"android_dimension":android_target_dimension,
			"apple_id"         :apple_id.val(),
			"apple_pass"       :apple_pass.val(),
			"apple_dimension"  :apple_target_dimension,
		},
		success: function(output) {
			if (output === 'ok')
				helper_show_status('Credentials have been successfully updated!');
			else
				helper_show_status(output);
		}
	})
}
/* ================================== *
 * Generate the form table via JQuery
 * ================================== */
function generate_form_table() {
	$('.view-dpsbridge-dps-folio-module-config').append("");
}
/* ======================================== *
 * Load user credentaisl from the database,
 *   and insert into the form table.
 * ======================================== */
function generate_form_table_values() {
	$.ajax({
		url: baseURL+"/dpsbridge/dps/credentials/pull",
		type: "POST",
		success: function(output) {
			$('#apikey').val(output['apikey']);
			$('#apisec').val(output['apisec']);
			$('#amazon_id').val(output['amazon']['id']);
			$('#amazon_pass').val(output['amazon']['pass']);
			helper_generate_dimensions('amazon_dimension', output['amazon']['dimension'], TRUE);
			$('#android_id').val(output['android']['id']);
			$('#android_pass').val(output['android']['pass']);
			helper_generate_dimensions('android_dimension', output['android']['dimension'], TRUE);
			$('#apple_id').val(output['apple']['id']);
			$('#apple_pass').val(output['apple']['pass']);
			helper_generate_dimensions('apple_dimension', output['apple']['dimension'], TRUE);
		}
	})
}
/* ========================================================= *
 * Attempts to read the current available local stylesheets,
 *   The local storage folder is @ /dpsbridge/styles.
 * ========================================================= */
function generate_form_table_stylesheet() {
	var styleText = '';
	$.ajax({
		url: baseURL+"/dpsbridge/stylesheet/read",
		type: "POST",
		success: function(output) {
			for (var i = 0; i < output.length; i++) {
				if (output[i].indexOf('-') < 0)
					styleText = output[i];
				else // removes the derivatives for viewing purposes
					styleText = output[i].split(/-/)[1];
				$("#stylesheets").append("<tr id='style-"+output[i]+"'><td value='"+output[i]+"'>"+styleText+"</td></tr>");
				$("#stylesheet-delete").append("<option value='"+output[i]+"'>"+styleText+"</option>");
				$("#stylesheet-download").append("<option value='"+output[i]+"'>"+styleText+"</option>");
			}
		}
	});
}
/* ================================================== *
 * Given the targeted dimension and the account type,
 *   insert the dimension in numerical order.
 * ================================================== */
function insertDimension(accountTypeID, dimension) {
	var account = $('#'+accountTypeID+' option');
	for (var i = 0; i < account.length; i++) {
		current = account[i].value.split(' x ');
		target  = dimension.split(' x ');
		if (parseInt(target[0]) <= parseInt(current[0])) {
			$('#'+accountTypeID).eq(i).prepend('<option value="'+dimension+'">'+dimension+'</option>');
			return;
		}
	}
	$('#'+accountTypeID+':last').append('<option value="'+dimension+'">'+dimension+'</option>');
}
/* ========================================= *
 * Attempts to logout of the Folio Producer,
 *   and clean the PHP Session
 * ========================================= */
function logout() {
	$.ajax({
		url: pathToDir+"/fp_logout.php",
		type: "POST",
	})
}
/* ======================================= *
 * Depending on the selected account type,
 *   display the dimensions accordingly.
 * ======================================= */
function refreshDimensions() {
	var account = $('#account-type-delete :selected')
	$('#dimension-list').empty();
	switch(account.val()) {
		case 'amazon':
			helper_generate_dimensions('dimension-list', $('#amazon_dimension option'), false);
			break;
		case 'android':
			helper_generate_dimensions('dimension-list', $('#android_dimension option'), false);
			break;
		case 'apple':
			helper_generate_dimensions('dimension-list', $('#apple_dimension option'), false);
			break;
	}
}
/* ============================== *
 * Update the global fields, 
 *   in preparation for API calls
 * ============================== */
function updateFields() {
	apikey            = $('#apikey');
	apisec            = $('#apisec');
	amazon_id         = $('#amazon_id');
	amazon_pass       = $('#amazon_pass');
	amazon_dimension  = $('#amazon_dimension option');
	android_id        = $('#android_id');
	android_pass      = $('#android_pass');
	android_dimension = $('#android_dimension option');
	apple_id          = $('#apple_id');
	apple_pass        = $('#apple_pass');
	apple_dimension   = $('#apple_dimension option');
}
/* /-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\/-\ *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * Constructing the Jquery UI Overlay Windows *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * \-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/\-/ */
$(function() {
	// dialog box for checking credentials
	$("#dialog-option-connectivity").dialog({
		autoOpen:false, height:325, width:350, modal:true,
		buttons: {
			Close: function() {
				$(this).dialog("close"); }},
	});
	// dialog box for adding dimensions
	$("#dialog-option-dimension-add").dialog({
		autoOpen:false, height:300, width:300, modal:true,
		buttons: {
			Add: function() {
				var desiredAccount  = $('#account-type-add :selected'), // selected account type
					dimensionLength = $('#dlength'), // input length
					dimensionWidth  = $('#dwidth');  // input width
				if (dimensionLength.val() && dimensionWidth.val()) {
					// calls helper method to insert the dimension in numerical order
					insertDimension(desiredAccount.val(), dimensionLength.val()+' x '+dimensionWidth.val());
					// clean the text fields
					$('#dlength').val('');
					$('#dwidth').val('');
					$(this).dialog("close");
				} else {
					helper_show_status('Please enter desired dimension length and width.');
				}},
			Close: function() {
				$(this).dialog("close"); }},
	});
	// dialog box for deleting dimensions
	$("#dialog-option-dimension-delete").dialog({
		autoOpen:false, height:350, width:300, modal:true,
		buttons: {
			Delete: function() {
				var desiredAccount = $('#account-type-delete :selected'),
					dimensions = $('#dimension-list :selected');
				// check if all available dimensions are selected, to prevent deletion of all available dimensions
				if (dimensions.length >= $('#'+desiredAccount.val()+'_dimension option').length) {
					helper_show_status('At least 1 dimension has to exist per account type!');
				} else if (dimensions.length > 0) { // if user has selected dimension(s) < availalbe dimensions
					// delete the selected dimension(s) from the selected account type
					for (var i = 0; i < dimensions.length; i++) {
						$('#'+desiredAccount.val()+'_dimension option[value="'+dimensions[i].value+'"]').remove();
					}
					$(this).dialog("close");
				} else { // if user hasn't selected any dimensions
					helper_show_status('Please select dimension(s) to delete.');
				} },
			Close: function() {
				$(this).dialog("close"); }},
		open: function() {
			refreshDimensions();
		}
	});
	// dialog box for adding stylesheets
	$("#dialog-option-stylesheet-add").dialog({
		autoOpen:false, height:350, width:400, modal:true,
		buttons: {
			Upload: function() {
				var filename = $('#filename').val(),
					derivative = $('#derivative :selected').val();
				if (!filename) {
					helper_show_status("Please enter a filename!");
				} else if (!derivative) {
					helper_show_status("Please select the derivative of this stylesheet!");
				} else if (filename == 'Bootstrap' || filename == 'Foundation') {
					helper_show_status("Cannot override the stocked Bootstrap or Foundation stylesheet!");
				} else {
					// sends the zip file via XML-HTTP-Request
					var form = document.getElementById('stylesheet-form'),
						formData = new FormData(form),
				 		xhr = new XMLHttpRequest();
				 	xhr.open('POST', form.getAttribute('action'), false);
					xhr.send(formData);
					var response = xhr.responseText;
					response = response.replace(/\n/, '').replace(/\"/g, '');
					response = response.split(/-/);
					if (response[0] == "ok" || response[0] == "↵ok") {
						$("#stylesheets").append("<tr id='style-"+derivative+"-"+response[1]+"'><td value='"+derivative+"-"+response[1]+"'>"+response[1]+"</td></tr>");
						$("#stylesheet-delete").append("<option value='"+derivative+"-"+response[1]+"'>"+response[1]+"</option>");
						$("#stylesheet-download").append("<option value='"+derivative+"-"+response[1]+"'>"+response[1]+"</option>");
					} else {
						helper_show_status(response[0]+' '+response[1]);
					}
					$(this).dialog("close");
				} },
			Close: function() {
				$(this).dialog("close"); }},
	});
	// dialog box for deleting stylesheets
	$("#dialog-option-stylesheet-delete").dialog({
		autoOpen:false, height:250, width:350, modal:true,
		buttons: {
			Delete: function() {
				var stylesheetNum = $('#stylesheet-delete option').length,
					stylesheet = $('#stylesheet-delete :selected').val();
				if (stylesheetNum <= 1) { // must have at least 1 stylesheet
					helper_show_status("Must have at least 1 stylesheet available at all times!");
				} else if (stylesheet == 'Bootstrap' || stylesheet == 'Foundation') { // cannot delete Bootstrap or Foundation stylesheet
					helper_show_status("Cannot delete the stocked Bootstrap or Foundation stylesheet!");
				} else {
					// deletes the stylesheet from local directory
					$.ajax({ url: baseURL+"/dpsbridge/stylesheet/delete", type: "POST", data: {'filename':stylesheet} });
					// remove stylesheet from view
					$("#style-"+stylesheet).remove();
					$("#stylesheet-delete option[value="+stylesheet+"]").remove();
					$("#stylesheet-download option[value="+stylesheet+"]").remove();
				}
				$(this).dialog("close"); },
			Close: function() {
				$(this).dialog("close"); }},
	});
	// dialog box for deleting stylesheets
	$("#dialog-option-stylesheet-download").dialog({
		autoOpen:false, height:200, width:350, modal:true,
		buttons: {
			Download: function() {
				var stylesheet = $('#stylesheet-download :selected').val();
				helper_download_file(baseURL, pathToDir+'/styles/'+stylesheet+'/HTMLResources.zip', 'HTMLResources', '0');
				$(this).dialog("close"); },
			Close: function() {
				$(this).dialog("close"); }},
	});
	// dialog box for showing any messages
	$("#dialog-status").dialog({
		autoOpen:false, modal:true,
		buttons: {
			Close: function() {
				$(this).dialog("close"); }},
	});
	// initialize the tabs
	$('#jqueryui-tabs').tabs();
});
</script>
