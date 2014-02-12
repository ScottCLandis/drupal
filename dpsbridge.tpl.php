<?php

$fpselect   = FALSE;
$fpmanage   = FALSE;
$fpcredent  = FALSE;

/* --------------------------------------------------------------- *
 * Includes the respective PHP files depending on the current page *
 * --------------------------------------------------------------- */
if (views_get_page_view()) {
	$url = "$_SERVER[REQUEST_URI]";
	// If on the Folio Article Selection page.
	if (strpos($url, "fpselect")) {
		if (user_access('folio_article_permission')) {
			$fpselect = TRUE;
			helper_include_goodies('select');
			$fp_nodes_all = node_load_multiple(array(), array('type' => 'folios'));
			$fp_nodes_selected = array();
			require_once drupal_get_path('module', 'dpsbridge').'/dpsbridge_view_select.inc';
		} else {
			drupal_goto('dpsbridge/views/access-denied');
		}
	}
	// If on the Folio Management page.
	else if (strpos($url, "fpmanage")) {
		if (user_access('folio_management_permission')) {
			$fpmanage = TRUE;
			helper_include_goodies('manage');
			require_once drupal_get_path('module', 'dpsbridge').'/dpsbridge_view_manage.inc';
		} else {
			drupal_goto('dpsbridge/views/access-denied');
		}
	}
	// If on the Adobe DPS Folio Module Config page.
	else if (strpos($url, "fpconfig")) {
		if (user_access('folio_config_permission')) {
			$fpcredent = TRUE;
			helper_include_goodies('config');
			require_once drupal_get_path('module', 'dpsbridge').'/dpsbridge_view_credential.inc';
		} else {
			drupal_goto('dpsbridge/views/access-denied');
		}
	}
}
/* ------------------------------------------------------------ *
 * Add and/or remove buttons depending on the respective pages, *
 *   after the page has been loaded                             *
 * ------------------------------------------------------------ */
if ($fpmanage || $fpselect || $fpcredent) {
	echo "<script>\n";
	echo "$(window).load(function() {\n";
		if ($fpmanage) {
			echo "var container = $('#block-system-main');\n";
			echo "var button_wrapper = $('<div/>').css({'float':'left', 'margin-bottom':'15px'});\n";
			echo "var button_clone = $('<button/>').text('Clone Selected Folio').click(function() { get_selected('clone'); });\n";
			echo "var button_delete = $('<button/>').text('Delete Selected Folio').click(function() { get_selected('delete'); });\n";
			echo "var button_upload = $('<button/>').text('Upload Selected Folio to Folio Producer').click(function() { get_selected('upload'); });\n";
			echo "button_wrapper.append(button_clone, button_delete, button_upload);\n";
			echo "container.append(button_wrapper);\n";
			echo "replaceHREF($('table td.views-field-edit-node a'), 'full');\n";
			echo "replaceHREF($('table td.views-field-edit-node-1 a'), 'half');\n";
		} else if ($fpselect) {	
			echo "var container = $('#block-system-main');\n";
			echo "var button_wrapper = $('<div/>').css({'float':'left', 'margin-bottom':'15px'});\n";
			if ($fp_nodes_all) {
				echo "var button_exist = $('<button/>').text('Add Selected Articles to Existing Folio').click(function() { get_selected('existing') });\n";
				echo "button_wrapper.append(button_exist)\n";
			}
			echo "var button_new = $('<button/>').text('Add Selected Articles to New Folio').click(function() { get_selected('new') });\n";
			echo "button_wrapper.append(button_new)\n";
			echo "container.append(button_wrapper);\n";
		} else if ($fpcredent) {
			echo "generate_form_table_values();\n";
			echo "generate_form_table_stylesheet();\n";
		}
	echo "});\n";
	echo "</script>\n";
}

?>

<button onclick="window.location='<?= $GLOBALS['base_url'] ?>/fpselect'">Folio Article Selection</button>
<button onclick="window.location='<?= $GLOBALS['base_url'] ?>/fpmanage'">Folio Management View</button>
<button onclick="window.location='<?= $GLOBALS['base_url'] ?>/fpconfig'">Adobe DPS Folio Module Config</button>

