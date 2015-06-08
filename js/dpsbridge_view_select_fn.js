/**
 * View Select Scripting.
 */

var articles = "";
/* ----------------------------------------------------------- *
 * Attempts to obtain the values from the selected checkboxes,
 *  toggles the overlay window if checkboxes are selected
 * ----------------------------------------------------------- */
function get_selected(toggle) {
    var checkboxes = jQuery('.views-table tr input');
    var selectedArticles = "";
    for (var i = 1; i < checkboxes.length; i++) {
        if (checkboxes[i].checked) {
            selectedArticles += checkboxes[i].value + ",";
        }
    }
    // Give warning if nothing was selected.
    if (!selectedArticles) {
        dpsbridge_helper_show_status("Please select an article first!");
    }
    else {
        articles = selectedArticles;
        // Toggle the respective popup window.
        switch (toggle) {
            case "new":
                jQuery("#dialog-new").dialog("open");
                break;

            case "existing":
                jQuery("#dialog-existing").dialog("open");
                break;

            default:
                break;
        }
    }
}
/* ----------------------------------------- *
 * Constructing the Jquery UI Overlay Window
 * ----------------------------------------- */

(function ($) {
    Drupal.behaviors.dpsbridge_view_select = {
        attach: function() {
            baseURL = Drupal.settings.dpsbridge.base_url;
            var fname   = $("#fname"),
                pname   = $("#pname"),
                fnumber = $("#fnumber"),
                pdate   = $("#pdate");

            $("#dialog-new").dialog({
                autoOpen:false, height:470, width:350, modal:true,
                buttons: {
                    "Create Folio": function() {
                        dpsbridge_helper_show_status("Adding the selected Article(s) to \"" + fname.val() + "\", please wait...");
                        $.ajax({
                            url: baseURL + "/dpsbridge/folio/add",
                            type: "POST",
                            data: {
                                "fname":fname.val(),
                                "pname":pname.val(),
                                "fnumber":fnumber.val(),
                                "pdate":pdate.val(),
                                "articles":articles },
                            success: function(output) {
                                if (output === 'ok') {
                                    window.location = baseURL + "/admin/config/content/fpmanage";
                                }
                                else {
                                    dpsbridge_helper_show_status(output);
                                }
                            }
                        })
                        $(this).dialog("close"); },
                    Cancel: function() { $(this).dialog("close"); }}
            });
            $("#dialog-existing").dialog({
                autoOpen:false, height:275, width:400, modal:true,
                buttons: {
                    "Add to Folio": function() {
                        var fid = $("#fid :selected")
                        dpsbridge_helper_show_status("Adding the selected Article(s) to \"" + fid.text() + "\", please wait...");
                        $.ajax({
                            url: baseURL + "/dpsbridge/folio/append",
                            type: "POST",
                            data: {
                                "fid":fid.val(),
                                "articles":articles },
                            success: function(output) {
                                if (output === 'ok') {
                                    window.location = baseURL + "/admin/config/content/fpmanage";
                                }
                                else {
                                    dpsbridge_helper_show_status(output);
                                }
                            }
                        })
                        $(this).dialog("close"); },
                    Cancel: function() { $(this).dialog("close"); }}
            });
            $("#dialog-status").dialog({
                autoOpen:false, modal:true,
                buttons: {
                    Close: function() {
                        $(this).dialog("close"); }}
            });
            $("#jqueryui-tabs").tabs();

            var container = $('#block-system-main');
            var button_wrapper = $('<div/>').css({'float':'left', 'margin-bottom':'15px'});
            var button_exist = $('<button/>').text('Add Selected Articles to Existing Folio').click(function() { get_selected('existing') });
            button_wrapper.append(button_exist)
            var button_new = $('<button/>').text('Add Selected Articles to New Folio').click(function() { get_selected('new') });
            button_wrapper.append(button_new)
            container.append(button_wrapper);
        }
    }
})(jQuery);
