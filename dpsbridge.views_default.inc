<?php
/**
 * @file
 * Enable views for the DPSBridge module.
 */

/**
 * Gemerate the following views page (sourced @ /dpsbridge/views/).
 */
function dpsbridge_views_default_views() {
  $view = NULL;
  $files = file_scan_directory(drupal_get_path('module', 'dpsbridge') . '/views', '/\.view$/');
  foreach ($files as $absolute => $file) {
    require $absolute;
    if (isset($view)) {
      $views[$file->name] = $view;
    }
  }
  return $views;
}
