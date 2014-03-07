<?php

/**
 * @file
 * dpsbridge.admin.inc
 * Defines admin config. screens and functionality for the dpsbridge module
 */

/**
 * General admin settings form for dpsbridge
 */
function dpsbridge_settings() {
  $form = array();  
  // Server information.
/*
  $form['server_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('iMIS server information'),
    '#description' => t('Information to connect to iMIS server.'),
    '#collapsible' => TRUE,
  );
  $form['server_info']['mpa_imis_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Hostname'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('iMIS Hostname (e.g. http://upgrade15.magazine.org).'),
    '#default_value' => variable_get('mpa_imis_server_url', 'http://upgrade15.magazine.org'),
  );
  $form['server_info']['mpa_imis_profile_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to get profile package from iMIS.'),
    '#default_value' => variable_get('mpa_imis_profile_method_path', '/scriptcontent/custom/token/newcftoken.cfm'),
  );
  $form['server_info']['mpa_imis_logoff_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Logoff Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to logoff from iMIS.'),
    '#default_value' => variable_get('mpa_imis_logoff_method_path', '/source/security/Member-Logoff.cfm'),
  );
  $form['server_info']['mpa_imis_event_list_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Event List Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to get event list package from iMIS.'),
    '#default_value' => variable_get('mpa_imis_event_list_method_path', '/scriptcontent/custom/xml/xml_mpaeventlist.cfm'),
  );
  $form['server_info']['mpa_imis_event_detail_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Detail Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to get event detail package from iMIS.'),
    '#default_value' => variable_get('mpa_imis_event_detail_method_path', '/scriptcontent/custom/xml/xml_mpaeventdetail.cfm'),
  );
//  $form['server_info']['mpa_imis_event_function_method_path'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Event Function Method'),
//    '#size' => 200,
//    '#required' => TRUE,
//    '#description' => t('The method used to get event function package from iMIS.'),
//    '#default_value' => variable_get('mpa_imis_event_function_method_path', '/scriptcontent/custom/xml/xml_mpaeventfunction.cfm'),
//  );
  $form['server_info']['mpa_imis_event_function_meetdesc_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Meeting Description Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to get event description from iMIS.'),
    '#default_value' => variable_get('mpa_imis_event_function_meetdesc_method_path', '/scriptcontent/custom/xml/xml_mpaeventfunctionmeetdesc.cfm'),
  );
  $form['server_info']['mpa_imis_event_contact_method_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Event Contact Method'),
    '#size' => 200,
    '#required' => TRUE,
    '#description' => t('The method used to get event contact package from iMIS.'),
    '#default_value' => variable_get('mpa_imis_event_contact_method_path', '/scriptcontent/custom/xml/xml_mpaeventcontact.cfm'),
  ); */
  return system_settings_form($form);
}


/**
 *  Menu callback for folio selection.
 */
function dpsbridge_fpselect_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_select_fn.js');
  
  $output = '';
  
  $view = views_get_view('folio_article_selection');
  $display = 'page';
  //set the display
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


function dpsbridge_fpmanage_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_manage_fn.js');
  
  $view = views_get_view('folio_management_view');
  $display = 'page';
  //set the display
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

function dpsbridge_credential_page() {
  $view = views_get_view('dpsbridge_module_config');
  $display = 'page';
  //set the display
  $view->set_display($display);
  $dpsbridge_config = $view->preview($display);
  
  $variables = array(
    'dpsbridge_config' => $dpsbridge_config,
  );
  
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge_view_credential_fn.js');
  return theme('dpsbridge_credential_page', $variables);
}