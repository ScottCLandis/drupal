<?php
/**
 * @file
 * Defines admin configuration screens and functionality for DPSBridge module.
 */

/**
 * General admin settings form for dpsbridge.
 */
function dpsbridge_settings() {
  $form = array();
  return system_settings_form($form);
}

/**
 * Menu callback for Folio Selection.
 */
function dpsbridge_fpselect_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_select_fn.js');
  $output = '';
  $view = views_get_view('folio_article_selection');
  $display = 'page';

  // Set the display.
  $view->set_display($display);
  $output .= $view->preview($display);

  // Overlay for adding new folios.
  $output .= '<div id="dialog-new" title="Add Selected Articles to New Folio">';
  $form = drupal_get_form('dpsbridge_add_new_articles_to_folio_form');
  $output .= drupal_render($form);
  $output .= '</div>';
  // Overlay for updating existing folios.
  $output .= '<div id="dialog-existing" title="Add Selected Articles to Existing Folio" class="align-center">';
  $form = drupal_get_form('dpsbridge_add_existing_articles_to_folio_form');
  $output .= drupal_render($form);
  $output .= '</div>';
  // Overlay for displaying status.
  $output .= '<div id="dialog-status" title="Notice"><div id="status"></div></div>';
  return $output;
}

/**
 * Menu callback for Folio Management.
 */
function dpsbridge_fpmanage_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_manage_fn.js');

  $view = views_get_view('folio_management_view');
  $display = 'page';

  // Set the display.
  $view->set_display($display);
  $folio_listing = $view->preview($display);

  $form = drupal_get_form('dpsbridge_edit_existing_folio_form');
  $folio_edit_form = drupal_render($form);

  $form = drupal_get_form('dpsbridge_image_form');
  $folio_image_form = drupal_render($form);

  $variables = array(
    'folio_listing' => $folio_listing,
    'folio_edit_form' => $folio_edit_form,
    'folio_image_form' => $folio_image_form,
  );

  $output = theme('dpsbridge_fpmanage_page', $variables);
  return $output;
}

/**
 * Menu callback for Credential Page.
 */
function dpsbridge_credential_page() {
  $form = drupal_get_form('dpsbridge_module_config_form');

  $variables = array(
    'dpsbridge_config' => $form,
  );

  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_credential_fn.js');
  return theme('dpsbridge_credential_page', $variables);
}
