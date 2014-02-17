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
