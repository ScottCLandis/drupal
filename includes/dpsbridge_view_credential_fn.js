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
	baseURL = "",
        pathToDir = "";
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
	dpsbridge_helper_show_status('Attempting to connect to Folio Producer, please wait...');
	jQuery.ajax({
		url: pathToDir+"/fp_connect.php",
		type: "POST",
		data: {
			"AdobeID"   :AdobeID,   // Folio Producer user ID
			"Password"  :AdobePass, // Folio Producer user password
			"APIKey"    :APIKey,    // Folio Producer API key
			"APISecret" :APISecret, // Folio Producer API secret
			"Test"		:toggle    // toggle for stating that this is a test
		},
		success: function(output) {
			if (output === 'ok')
				dpsbridge_helper_show_status("<strong><em>"+type+"</em></strong><br/><br/>-Folio Producer API authentication successful!<br/><br/>- Login successful!<br/><br/>- Test completed!", 350, 400);
			else
				dpsbridge_helper_show_status("<strong><em>"+type+"</em></strong><br/><br/>:: "+output, 600, 250);
			logout();
		}
	});
}
/* =============================================== *
 * Insert/update the credentials in the user table
 * =============================================== */
function dps_credentials() {
	updateFields();
	dpsbridge_helper_show_status('Updating credentials, please wait...');
	// call helper to convert the array of dimensions into a string, separated by commas
	var amazon_target_dimension = dpsbridge_helper_array_to_string(amazon_dimension);
	var android_target_dimension = dpsbridge_helper_array_to_string(android_dimension);
	var apple_target_dimension = dpsbridge_helper_array_to_string(apple_dimension);

	jQuery.ajax({
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
			"apple_dimension"  :apple_target_dimension
		},
		success: function(output) {
			if (output === 'ok')
				dpsbridge_helper_show_status('Credentials have been successfully updated!');
			else
				dpsbridge_helper_show_status(output);
		}
	})
}
/* ================================== *
 * Generate the form table via JQuery
 * ================================== */
function generate_form_table() {
	jQuery('.view-dpsbridge-dps-folio-module-config').append("");
}
/* ======================================== *
 * Load user credentaisl from the database,
 *   and insert into the form table.
 * ======================================== */
function generate_form_table_values() {
    if (!jQuery('#edit-credential-table').hasClass('processed')) {
        jQuery('#edit-credential-table').addClass('processed');
	jQuery.ajax({
		url: baseURL+"/dpsbridge/dps/credentials/pull",
		type: "POST",
		success: function(output) {
			jQuery('#apikey').val(output['apikey']);
			jQuery('#apisec').val(output['apisec']);
			jQuery('#amazon_id').val(output['amazon']['id']);
			jQuery('#amazon_pass').val(output['amazon']['pass']);
			dpsbridge_helper_generate_dimensions('amazon_dimension', output['amazon']['dimension'], true);
			jQuery('#android_id').val(output['android']['id']);
			jQuery('#android_pass').val(output['android']['pass']);
			dpsbridge_helper_generate_dimensions('android_dimension', output['android']['dimension'], true);
			jQuery('#apple_id').val(output['apple']['id']);
			jQuery('#apple_pass').val(output['apple']['pass']);
			dpsbridge_helper_generate_dimensions('apple_dimension', output['apple']['dimension'], true);
		}
	});
    }
}
/* ========================================================= *
 * Attempts to read the current available local stylesheets,
 *   The local storage folder is @ /dpsbridge/styles.
 * ========================================================= */
function generate_form_table_stylesheet() {
    var styleText = '';
    if (!jQuery('#stylesheets').hasClass('processed')) {
        jQuery('#stylesheets').addClass('processed');
	jQuery.ajax({
		url: baseURL+"/dpsbridge/stylesheet/read",
		type: "POST",
		success: function(output) {
			for (var i = 0; i < output.length; i++) {
				if (output[i].indexOf('-') < 0)
					styleText = output[i];
				else // removes the derivatives for viewing purposes
					styleText = output[i].split(/-/)[1];
				jQuery("#stylesheets").append("<tr id='style-"+output[i]+"'><td value='"+output[i]+"'>"+styleText+"</td></tr>");
				jQuery("#stylesheet-delete").append("<option value='"+output[i]+"'>"+styleText+"</option>");
				jQuery("#stylesheet-download").append("<option value='"+output[i]+"'>"+styleText+"</option>");
			}
		}
	});
    }
}
/* ================================================== *
 * Given the targeted dimension and the account type,
 *   insert the dimension in numerical order.
 * ================================================== */
function insertDimension(accountTypeID, dimension) {
	var account = jQuery('#'+accountTypeID+' option');
	for (var i = 0; i < account.length; i++) {
		current = account[i].value.split(' x ');
		target  = dimension.split(' x ');
		if (parseInt(target[0]) <= parseInt(current[0])) {
			jQuery('#'+accountTypeID).eq(i).prepend('<option value="'+dimension+'">'+dimension+'</option>');
			return;
		}
	}
	jQuery('#'+accountTypeID+':last').append('<option value="'+dimension+'">'+dimension+'</option>');
}
/* ========================================= *
 * Attempts to logout of the Folio Producer,
 *   and clean the PHP Session
 * ========================================= */
function logout() {
	jQuery.ajax({
		url: pathToDir+"/fp_logout.php",
		type: "POST"
	})
}
/* ======================================= *
 * Depending on the selected account type,
 *   display the dimensions accordingly.
 * ======================================= */
function refreshDimensions() {
	var account = jQuery('#account-type-delete :selected')
	jQuery('#dimension-list').empty();
	switch(account.val()) {
		case 'amazon':
			dpsbridge_helper_generate_dimensions('dimension-list', jQuery('#amazon_dimension option'), false);
			break;
		case 'android':
			dpsbridge_helper_generate_dimensions('dimension-list', jQuery('#android_dimension option'), false);
			break;
		case 'apple':
			dpsbridge_helper_generate_dimensions('dimension-list', jQuery('#apple_dimension option'), false);
			break;
	}
}
/* ============================== *
 * Update the global fields, 
 *   in preparation for API calls
 * ============================== */
function updateFields() {
	apikey            = jQuery('#apikey');
	apisec            = jQuery('#apisec');
	amazon_id         = jQuery('#amazon_id');
	amazon_pass       = jQuery('#amazon_pass');
	amazon_dimension  = jQuery('#amazon_dimension option');
	android_id        = jQuery('#android_id');
	android_pass      = jQuery('#android_pass');
	android_dimension = jQuery('#android_dimension option');
	apple_id          = jQuery('#apple_id');
	apple_pass        = jQuery('#apple_pass');
	apple_dimension   = jQuery('#apple_dimension option');
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
            pathToDir = '/' + Drupal.settings.dpsbridge.path_to_dir;
        }
    }
    Drupal.behaviors.dpsbridge_view_credential = {
        attach: function() {
	// dialog box for checking credentials
	$("#dialog-option-connectivity").dialog({
		autoOpen:false, height:325, width:350, modal:true,
		buttons: {
			Close: function() {
				$(this).dialog("close"); }}
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
					dpsbridge_helper_show_status('Please enter desired dimension length and width.');
				}},
			Close: function() {
				$(this).dialog("close"); }}
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
					dpsbridge_helper_show_status('At least 1 dimension has to exist per account type!');
				} else if (dimensions.length > 0) { // if user has selected dimension(s) < availalbe dimensions
					// delete the selected dimension(s) from the selected account type
					for (var i = 0; i < dimensions.length; i++) {
						$('#'+desiredAccount.val()+'_dimension option[value="'+dimensions[i].value+'"]').remove();
					}
					$(this).dialog("close");
				} else { // if user hasn't selected any dimensions
					dpsbridge_helper_show_status('Please select dimension(s) to delete.');
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
					dpsbridge_helper_show_status("Please enter a filename!");
				} else if (!derivative) {
					dpsbridge_helper_show_status("Please select the derivative of this stylesheet!");
				} else if (filename == 'Bootstrap' || filename == 'Foundation') {
					dpsbridge_helper_show_status("Cannot override the stocked Bootstrap or Foundation stylesheet!");
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
						dpsbridge_helper_show_status(response[0]+' '+response[1]);
					}
					$(this).dialog("close");
				} },
			Close: function() {
				$(this).dialog("close"); }}
	});
	// dialog box for deleting stylesheets
	$("#dialog-option-stylesheet-delete").dialog({
		autoOpen:false, height:250, width:350, modal:true,
		buttons: {
			Delete: function() {
				var stylesheetNum = $('#stylesheet-delete option').length,
					stylesheet = $('#stylesheet-delete :selected').val();
				if (stylesheetNum <= 1) { // must have at least 1 stylesheet
					dpsbridge_helper_show_status("Must have at least 1 stylesheet available at all times!");
				} else if (stylesheet == 'Bootstrap' || stylesheet == 'Foundation') { // cannot delete Bootstrap or Foundation stylesheet
					dpsbridge_helper_show_status("Cannot delete the stocked Bootstrap or Foundation stylesheet!");
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
				$(this).dialog("close"); }}
	});
	// dialog box for deleting stylesheets
	$("#dialog-option-stylesheet-download").dialog({
		autoOpen:false, height:200, width:350, modal:true,
		buttons: {
			Download: function() {
				var stylesheet = $('#stylesheet-download :selected').val();
				dpsbridge_helper_download_file(baseURL, pathToDir+'/styles/'+stylesheet+'/HTMLResources.zip', 'HTMLResources', '0');
				$(this).dialog("close"); },
			Close: function() {
				$(this).dialog("close"); }}
	});
	// dialog box for showing any messages
	$("#dialog-status").dialog({
		autoOpen:false, modal:true,
		buttons: {
			Close: function() {
				$(this).dialog("close"); }}
	});
	// initialize the tabs
	$('#jqueryui-tabs').tabs();
        generate_form_table_values();
	generate_form_table_stylesheet();
        }
    }
})(jQuery);
