<?php

/**
 * Gemerate the following views page (sourced @ /adobe/views/):
 *   - Folio Selection view
 *   - Folio Management view
 *   - Adobe DPS Folio Configuration view
 */
function adobe_views_default_views() {
	$files = file_scan_directory(drupal_get_path('module', 'adobe').'/views', '/\.view$/');
	foreach ($files as $absolute => $file) {
		require $absolute;
		if (isset($view)) {
			$views[$file->name] = $view;
		}   
	}
	return $views;
}
