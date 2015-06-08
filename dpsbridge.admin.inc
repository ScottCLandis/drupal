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
 * General settings form for DPSNext
 */
function dpsbridge_next_admin_config_settings() {
  global $user;

  $form = array();

  $form['ops'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#description' => t('General settings for connecting to Adobe Publish.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['ops']['dpsbridge_next_authentication_endpoint']=array(
    '#type' => 'textfield',
    '#title' => t('Authentication Endpoint URL'),
    '#maxlength' => 300,
      '#default_value' => variable_get(DPSNextConfig::VARIABLE_AUTHENTICATION_ENDPOINT, DPSNextConfig::AUTHENTICATION_ENDPOINT),
    '#weight' => -1,
    '#description' => t('Authentication URL, e.g. ' . DPSNextConfig::AUTHENTICATION_ENDPOINT . '.'),
  );
  $form['ops']['dpsbridge_next_authorization_endpoint']=array(
    '#type' => 'textfield',
    '#title' => t('Authorization Endpoint URL'),
    '#maxlength' => 300,
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_AUTHORIZATION_ENDPOINT, DPSNextConfig::AUTHORIZATION_ENDPOINT),
    '#weight' => 0,
    '#description' => t('Authorization URL, e.g. ' . DPSNextConfig::AUTHORIZATION_ENDPOINT . '.'),
  );
  $form['ops']['dpsbridge_next_injestion_endpoint']=array(
    '#type' => 'textfield',
    '#title' => t('Injestion Endpoint URL'),
    '#maxlength' => 300,
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_INJESTION_ENDPOINT, DPSNextConfig::INJESTION_ENDPOINT),
    '#weight' => 1,
    '#description' => t('Injestion URL, e.g. ' . DPSNextConfig::INJESTION_ENDPOINT . '.'),
  );
  $form['ops']['dpsbridge_next_producer_endpoint']=array(
    '#type' => 'textfield',
    '#title' => t('Producer Endpoint URL'),
    '#maxlength' => 300,
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_PRODUCER_ENDPOINT, DPSNextConfig::PRODUCER_ENDPOINT),
    '#weight' => 2,
    '#description' => t('Producer URL, e.g. ' . DPSNextConfig::PRODUCER_ENDPOINT . '.'),
  );
  // Credential settings
  $form['dps_credential_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credential Settings'),
    '#description' => t('Credential Settings to connect to Adobe Publish.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['dps_credential_settings']['dpsbridge_next_client_id']=array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#maxlength' => 300,
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_CLIENT_ID, DPSNextConfig::CLIENT_ID),
    '#weight' => 1,
    '#description' => t('Client ID.'),
    '#required' => TRUE,
  );
  $form['dps_credential_settings']['dpsbridge_next_client_secret']=array(
    '#type' => 'password',
    '#title' => t('Client Secret'),
    '#maxlength' => 300,
    '#attributes' => array('value' => variable_get(DPSNextConfig::VARIABLE_CLIENT_SECRET, DPSNextConfig::CLIENT_SECRET)),
    '#weight' => 2,
    '#description' => t('Client Secret.'),
    '#required' => TRUE,
  );
  $form['dps_credential_settings']['dpsbridge_next_device_token']=array(
    '#type' => 'textarea',
    '#title' => t('Device Token'),
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_DEVICE_TOKEN, DPSNextConfig::DEVICE_TOKEN_DEFAULT),
    '#weight' => 4,
    '#description' => t('Device Token.'),
    '#required' => TRUE,
  );
  $form['dps_credential_settings']['dpsbridge_next_device_id']=array(
    '#type' => 'textfield',
    '#title' => t('Device ID'),
    '#maxlength' => 300,
    '#default_value' => variable_get(DPSNextConfig::VARIABLE_DEVICE_ID, ''),
    '#weight' => 3,
    '#description' => t('Device ID.'),
    '#required' => TRUE,
  );
  $types = node_type_get_types();
	foreach($types as $node_type) {
  	//dsm($node_type);
		if($node_type->type != 'document' && $node_type->type != 'template' && $node_type->type != 'webform' && $node_type->type != 'collection' && $node_type->type != 'folios')
		  $options[$node_type->type] = $node_type->name;
	}

  //$contentinfo = field_info_instances();
	//dsm($contentinfo['node']);
	
	$form['contenttypes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adobe Publish Content Types'),
    '#description' => t('Allowed Content Type(s) to upload to Adobe Publish.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

	$form['contenttypes']['dpsbridge_next_node_types'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Adobe Publish Content Types'),
		'#options' => $options,
		'#default_value' => variable_get(DPSNextConfig::VARIABLE_NODE_TYPES, array('article')),
    '#weight' => 2,
    '#description' => t('These content types will be used to enter data for Adobe Publish. Document, Template and Webform are not included.'),
	);
  
  // Stysheets
  $options = array();
  $options_selected = array();
  $stylesheets = variable_get(DPSNextConfig::VARIABLE_STYLESHEETS, array());
  foreach ($stylesheets as $name => $stylesheet) {
    $options[$name] = $stylesheet['title'];
    $options_selected[$name] = $stylesheet['value'];
  }
  $form['dps_stylesheets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Adobe Publish Stylesheets'),
    '#description' => t('Stylesheet(s) to be used for HTML Article.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['dps_stylesheets']['allowed_stylesheets'] = array(
    '#type' => 'checkboxes',
		'#title' => t('Available Stylesheets'),
		'#options' => $options,
		'#default_value' => $options_selected,
    '#weight' => 1,
    '#description' => t('Please select all the allowed stylesheets.')
  );
  $form['dps_stylesheets']['add_new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add A Stylesheet'),
    '#weight' => 2,
  );
  $form['dps_stylesheets']['add_new']['stylesheet_filename'] = array(
    '#type' => 'textfield',
    '#title' => t('Stylesheet Name'),
    '#maxlength' => 300,
    '#description' => t('If stylesheet name already exists, the stylesheet files will be overwritten.'),
  );
  $form['dps_stylesheets']['add_new']['stylesheet_zipfile'] = array(
    '#type' => 'managed_file',
    '#title' => t('Select zip file to upload'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('zip'),
    ),
    //'#element_validate' => array('dpsbridge_next_admin_file_validator'),
    '#description' => t('The main css file should be located at <strong>css/style.css</strong>.  Allowed extensions: zip'),
  );

  $form['#submit'][] = 'dpsbridge_next_admin_config_settings_submit';
  $form['#validate'][] = 'dpsbridge_next_admin_file_validator';
  
  return system_settings_form($form);
}

function dpsbridge_next_admin_file_validator($element, &$form_state) {
  $filename = dpsbridge_helper_format_title($form_state['values']['stylesheet_filename']);
  if (empty($form_state['values']['stylesheet_zipfile']['fid']) && !empty($filename)) {
    form_set_error('stylesheet_zipfile', t("Please upload a file."));
  }
  if (!empty($form_state['values']['stylesheet_zipfile']['fid']) && empty($filename)) {
    form_set_error('stylesheet_filename', t("Please enter a stylesheet name."));
  }
} 
/**
 * Extra submit function for settings form.
 */
function dpsbridge_next_admin_config_settings_submit($form, &$form_state) {
  $form_state['values']['dpsbridge_next_device_token'] = str_replace(' ', '', $form_state['values']['dpsbridge_next_device_token']);
  
  // General settings and credential settings
  variable_set(DPSNextConfig::VARIABLE_AUTHENTICATION_ENDPOINT, $form_state['values']['dpsbridge_next_authentication_endpoint']);
  variable_set(DPSNextConfig::VARIABLE_AUTHORIZATION_ENDPOINT, $form_state['values']['dpsbridge_next_authorization_endpoint']);
  variable_set(DPSNextConfig::VARIABLE_INJESTION_ENDPOINT, $form_state['values']['dpsbridge_next_injestion_endpoint']);
  variable_set(DPSNextConfig::VARIABLE_PRODUCER_ENDPOINT, $form_state['values']['dpsbridge_next_producer_endpoint']);
  variable_set(DPSNextConfig::VARIABLE_CLIENT_ID, $form_state['values']['dpsbridge_next_client_id']);
  variable_set(DPSNextConfig::VARIABLE_CLIENT_SECRET, $form_state['values']['dpsbridge_next_client_secret']);
  variable_set(DPSNextConfig::VARIABLE_DEVICE_TOKEN, $form_state['values']['dpsbridge_next_device_token']);
  variable_set(DPSNextConfig::VARIABLE_DEVICE_ID, $form_state['values']['dpsbridge_next_device_id']);
  
  
  // Stylesheets
  $filename = dpsbridge_helper_format_title($form_state['values']['stylesheet_filename']);
  $directory = 'public://' . '/dpsbridge' . '/styles/' . $filename;

  $stylesheets = variable_get(DPSNextConfig::VARIABLE_STYLESHEETS, array());
  $allowed_stylesheets = $form_state['values']['allowed_stylesheets'];
  foreach ($allowed_stylesheets as $name => $value) {
    if (array_key_exists($name, $stylesheets)) {
      $stylesheets[$name]['value'] = $value;
    }
  }
  variable_set(DPSNextConfig::VARIABLE_STYLESHEETS, $stylesheets);
  
  // Save uploaded stylesheet
  if ($form_state['values']['stylesheet_zipfile']['fid']) {
    // Upload file
    $file = file_load($form_state['values']['stylesheet_zipfile']);
    mkdir($directory);
    file_move($file, $directory.'/HTMLResources.zip', FILE_EXISTS_REPLACE);
    // Add to druapl variable
    $filetitle = $form_state['values']['stylesheet_filename'];
    $filename = dpsbridge_helper_format_title($filetitle);
    $stylesheet_new = array(
      $filename => array(
        'title' => $filetitle,
        'value' => $filename),
    );
    $stylesheets = array_merge($stylesheets, $stylesheet_new);
    variable_set(DPSNextConfig::VARIABLE_STYLESHEETS, $stylesheets);
  }
 
  // Content types
  $dps_node_types = $form_state['values']['dpsbridge_next_node_types'];
  variable_set(DPSNextConfig::VARIABLE_NODE_TYPES, $dps_node_types);
  
  $allowed_content_types = array();
  $not_allowed_content_types = array();
  foreach($dps_node_types as $contentype => $value) {
  	if ($value != '0') {
      $allowed_content_types[$contentype] = $value;
  	}
    else {
      $not_allowed_content_types[$contentype] = $value;
    }
  }
  
  $install_fields = dpsbridge_helper_next_install_fields();
  $install_instances = array(
    'field_adobe_publish_uploaded' => array(
      'field_name' => 'field_adobe_publish_uploaded',
      'label' => t(''),
      'type' => 'list_integer',
      'default_value' => array(1=>array('value'=>1)),
      'widget' => array(
        'type' => 'options_buttons',
        'weight' => '-50'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Upload to Adobe Publish'),
          'type' => 'list_default'),
      ),
    ),
    'field_adobe_publish_short_title' => array(
      'field_name' => 'field_adobe_publish_short_title',
      'label' => t('Adobe Publish Short Title'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Short Title'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_abstract' => array(
      'field_name' => 'field_adobe_publish_abstract',
      'label' => t('Adobe Publish Abstract'),
      'type' => 'text',
      'widget' => array(
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
      ),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Abstract'),
          'type' => 'text'),
      ),
    ),
    'field_adobe_publish_short_ab' => array(
      'field_name' => 'field_adobe_publish_short_ab',
      'label' => t('Adobe Publish Short Abstract'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Short Abstract'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_author' => array(
      'field_name' => 'field_adobe_publish_author',
      'label' => t('Adobe Publish Author'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Author'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_author_url' => array(
      'field_name' => 'field_adobe_publish_author_url',
      'label' => t('Adobe Publish Author URL'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'description' => 'Link to more information about the author. The URL needs to be in the form of http:// or https://',
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Author URL'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_browse' => array(
      'field_name' => 'field_adobe_publish_browse',
      'label' => t(''),
      'type' => 'list_integer',
      'default_value' => array(),
      'widget' => array(
        'type' => 'options_buttons',
        ),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Hide from Browse Page'),
          'type' => 'list_default'),
        ),
    ),
    'field_adobe_publish_department' => array(
      'field_name' => 'field_adobe_publish_department',
      'label' => t('Adobe Publish Depatment'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Depatment'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_category' => array(
      'field_name' => 'field_adobe_publish_category',
      'label' => t('Adobe Publish Category'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Category'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_keywords' => array(
      'field_name' => 'field_adobe_publish_keywords',
      'label' => t('Adobe Publish Keywords'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Keywords'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_int_key' => array(
      'field_name' => 'field_adobe_publish_int_key',
      'label' => t('Adobe Publish Internal Keywords'),
      'type' => 'text',
      'widget' => array(
        'type' => 'text_textfield'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Adobe Publish Internal Keywords'),
          'type' => 'text'),
        ),
    ),
    'field_adobe_publish_thumb'  => array(
      'field_name' => 'field_adobe_publish_thumb',
      'label' => t('Upload Thumbnail Image'),
      'type' => 'image',
      'widget' => array(
        'type' => 'image_image'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Thumbnail Image'),
          'type' => 'image'),
      ),
    ),
    'field_adobe_publish_style'  => array(
      'field_name' => 'field_adobe_publish_style',
      'label' => t('Select a Stylesheet to apply'),
      'type' => 'text',
      'widget' => array(
        'type' => 'options_select',
        'weight' => '-49'),
      'display' => array(
        'example_node_list' => array(
          'label' => t('Stylesheet'),
          'type' => 'list_default'),
      ),
    ),
  );
  if ($allowed_content_types) {
    /*
    // Change article reference of collection
    $collection_reference_field = field_info_field('field_collection_reference');
    //dsm($collection_reference_field);return;
    if ($collection_reference_field) {
      $collection_reference_field['settings']['target_bundles'] = $allowed_content_types;
      field_update_field($collection_reference_field);
      // update instance for collection
      //$instance_info = field_info_instance('node', 'field_collection_reference', 'collection');
      //dsm($instance_info);return;
      // field_update_instance($instance_info);
    }
    */
    // Create fields if not exists
    foreach ($install_fields as $fieldname => $field) {
      if (field_info_field($fieldname) === NULL) {
        field_create_field($field);
      }
      elseif ($fieldname == 'field_adobe_publish_style') {
        $default_style_field = field_info_field('field_adobe_publish_style'); 
        $default_style_field['settings']['allowed_values'] = dpsbridge_helper_next_get_stylesheets();
        try {
          field_update_field($default_style_field);
        }
        catch (FieldUpdateForbiddenException $e) {
          drupal_set_message(t('Failed to update stylesheets.'), 'error');
        }
      }
    }
    
    foreach ($allowed_content_types as $contentype => $value) {
      foreach ($install_instances as $fieldname => $fieldinstance) {
        $fieldinstance['entity_type'] = 'node';
        $fieldinstance['bundle'] = $contentype;
        //print_r($fieldinstance);
        if (field_info_instance('node', $fieldname, $contentype) === NULL) {
          field_create_instance($fieldinstance);
        }
        //field_delete_instance($fieldinstance);
      }
    }
  }
  /*
  else {
    // Delete fields if exists
    foreach ($install_fields as $fieldname => $field) {
      if (field_info_field($fieldname)) {
        field_delete_field($field);
      }
    }
  }
  // Delete field instances for not allowed content types
  foreach ($not_allowed_content_types as $contentype => $value) {
    foreach ($install_instances as $fieldname => $fieldinstance) {
      $fieldinstance['entity_type'] = 'node';
      $fieldinstance['bundle'] = $contentype;
      //print_r($fieldinstance);
      if (field_info_instance('node', $fieldname, $contentype)) {
        field_delete_instance($fieldinstance);
      }
    }
  }
  */
}

/**
 * Menu callback for DPSNext Article Management.
 */
function dpsbridge_next_artilce_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/js/dpsnext_view_article_fn.js');
  $output = '';
  $view = views_get_view('dpsnext_article_management');
  $display = 'page';

  // Set the display.
  $view->set_display($display);
  $output .= $view->preview($display);

  // Overlay for adding new folios.
  $output .= '<div id="dialog-new" title="Upload Selected Article to Producer">';
  //$form = drupal_get_form('dpsbridge_add_new_articles_to_folio_form');
  //$output .= drupal_render($form);
  $output .= '</div>';

  // Overlay for displaying status.
  $output .= '<div id="dialog-status" title="Notice"><div id="status"></div></div>';
  return $output;
}

/**
 * Menu callback for Folio Selection.
 */
function dpsbridge_fpselect_page() {
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/js/dpsbridge_view_select_fn.js');
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
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/js/dpsbridge_view_manage_fn.js');

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

  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/js/dpsbridge_view_credential_fn.js');
  return theme('dpsbridge_credential_page', $variables);
}
