<?php
/**
 * @file
 * DPSBridge module helper functions.
 */

/**
 * Helper method for formatting the given video url to embedded video url.
 *
 * @param string $url
 *   Video URL.
 *
 * @return string
 *   Containing the embedded video URL.
 */
function dpsbridge_helper_format_url($url) {
  $pattern = '/v=([\w]+)(&)*/';
  preg_match($pattern, $url, $formated_url);
  return 'http://youtube.com/embed/' . $formated_url[1];
}
/**
 * Helper method for removing any special chars from filename (article title).
 *
 * @param string $filename
 *   Article title, used as the folder name, folio name, and .html filename.
 * @param string $replacement
 *   Optional replacement char for the special chars.
 *
 * @return string
 *   Containing the formatted filename without any special characters.
 */
function dpsbridge_helper_format_title($filename, $replacement = '') {
  $noises = array('.', '?', ' ', ':', '/', ',', '\'');
  $formatted = str_replace($noises, $replacement, $filename);
  return $formatted;
}
/**
 * Helper method for pulling the article node contents.
 *
 * @param string $node_id
 *   Article node id.
 *
 * @return array
 *   Containing the article metadata.
 */
function dpsbridge_helper_pull_node($node_id, $node_metadata = NULL) {
  $container = array();
  // Loads the node from the database.
  $node = node_load($node_id);
  $container['node'] = $node;

  // Pull fields from dps node view.
  if ($node_metadata) {
    $node_display = node_view($node_metadata, 'dps');
  } 
  else {
    $node_display = node_view($node, 'dps');
  }
  
  $container['ds_content'] = isset($node_display) ? $node_display : array();

  // Loads the image urls from the database.
  $images = array();
  $img_list = "";
  $tags = "";
  // Checks if there is only 1 or more than 1 images.
  if (isset($node->field_image[LANGUAGE_NONE])) {
    $img_list = $node->field_image[LANGUAGE_NONE];
  }
  elseif (isset($node->field_images[LANGUAGE_NONE])) {
    $img_list = $node->field_images[LANGUAGE_NONE];
  }
  // Store images in an array.
  if ($img_list) {
    for ($i = 0; $i < count($img_list); $i++) {
      array_push($images, file_create_url($img_list[$i]['uri']));
    }
  }
  // Translate tag ID into tag string.
  if (isset($node->field_tags[LANGUAGE_NONE])) {
    for ($i = 0; $i < count($node->field_tags[LANGUAGE_NONE]); $i++) {
      if (isset($node->field_tags[LANGUAGE_NONE][$i]['tid'])) {
        $tags .= (taxonomy_term_load($node->field_tags[LANGUAGE_NONE][$i]['tid'])->name) . ', ';
      }
    }
  }
  $container['author'] = isset($node->field_folio_byline[LANGUAGE_NONE][0]['value']) ? $node->field_folio_byline[LANGUAGE_NONE][0]['value'] : $node->name;
  $container['title'] = isset($node->title) ? $node->title : '';
  $container['desc'] = isset($node->field_intro) ? $node->field_intro[LANGUAGE_NONE][0]['value'] : '';
  $container['kicker'] = '';
  $container['tags'] = ($tags != FALSE) ? substr($tags, 0, -2) : '';
  $container['title'] = isset($node->title) ? $node->title : '';
  $container['body'] = isset($node->body) ? $node->body : '';
  $container['image'] = isset($images[0]) ? $images : '';
  $container['video'] = isset($node->field_video[LANGUAGE_NONE][0]['video_url']) ? $node->field_video[LANGUAGE_NONE][0]['video_url'] : '';
  // Loads the taxonomy term.
  if (isset($node->field_topic[LANGUAGE_NONE][0]['tid'])) {
    $tid = $node->field_topic[LANGUAGE_NONE][0]['tid'];
    $taxonomy = taxonomy_term_load($tid);
    $container['kicker'] = $taxonomy->name;
  }
  return $container;
}
/**
 * Helper method to remove all files & subfolders within a directory.
 *
 * @param string $directory
 *   Name of the directory to be deleted.
 */
function dpsbridge_helper_rrmdir($directory) {
  foreach (glob($directory . '/*') as $item) {
    if (is_dir($item)) {
      dpsbridge_helper_rrmdir($item);
    }
    else {
      unlink($item);
    }
  }
  rmdir($directory);
}
/**
 * Helper method for storing the image in the generated HTML article folder.
 *
 * @param string $img_url
 *   URL to the source image.
 * @param string $filename
 *   Path to save the image (@ /dpsbridge/html/[filename]/).
 */
function dpsbridge_helper_save_img($img_url, $filename) {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $img = file_get_contents($img_url);
  file_put_contents($directory . $filename, $img);
}
/**
 * Helper method for scaling the image and saving it in a temp directory.
 *
 * @param string $img_url
 *   The targeted image url (local).
 * @param string $newwidth
 *   Specify the new image width.
 * @param string $newheight
 *   Specify the new image height.
 * @param string $orientation
 *   Specify if the image is landscape or portrait.
 *
 * @return string
 *   Url of the scaled image.
 */
function dpsbridge_helper_scale_img($img_url, $newwidth, $newheight, $orientation) {
  header('Content-Type: image/jpeg');
  // Get the current sizes.
  list($width, $height) = getimagesize($img_url);
  $thumb = imagecreatetruecolor($newwidth, $newheight);
  // Resizes a .png image file.
  if (exif_imagetype($img_url) == IMAGETYPE_PNG) {
    $source = imagecreatefrompng($img_url);
    imagecopyresized($thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);
    imagepng($thumb, dirname(__FILE__) . '/images/' . $orientation . '/temp.png');
    return 'images/' . $orientation . '/temp.png';
  }
  // Resizes a .jpeg image file.
  elseif (exif_imagetype($img_url) == IMAGETYPE_JPEG) {
    $source = imagecreatefromjpeg($img_url);
    imagecopyresized($thumb, $source, 0, 0, 0, 0, $newwidth, $newheight, $width, $height);
    imagejpeg($thumb, dirname(__FILE__) . '/images/' . $orientation . '/temp.jpg');
    return 'images/' . $orientation . '/temp.jpg';
  }
}
/**
 * Helper method to get stylesheets.
 */
function dpsbridge_helper_next_get_stylesheets($default = array(), $show_all = TRUE) {
  $allowed_stylesheets = $default;
  $allowed_stylesheets_config = variable_get(DPSNextConfig::VARIABLE_STYLESHEETS, array());
  foreach ($allowed_stylesheets_config as $name => $stylesheet) {
    if (!$show_all) {
      // Show only allowed stylesheets
      if ($stylesheet['value'] !== 0) {
       $allowed_stylesheets[$name] = $stylesheet['title'];
      }
    }
    else {
      $allowed_stylesheets[$name] = $stylesheet['title'];
    }
  }

  return $allowed_stylesheets;
}
/**
 * Helper method to get allowed content types.
 */
function dpsbridge_helper_next_get_node_types($default = array('collection')) {
  $allowed_content_types = $default;
  $allowed_content_types_config = variable_get(DPSNextConfig::VARIABLE_NODE_TYPES, array('article'));
  foreach ($allowed_content_types_config as $value) {
    if ($value != '0') {
      $allowed_content_types[] = $value;
    }
  }
  
  return $allowed_content_types;
}
/**
 * Helper method to get extra Adobe Publish fields for allowed content types.
 */
function dpsbridge_helper_next_install_fields() {
  $install_fields = array(
    'field_adobe_publish_uploaded' => array(
      'field_name' => 'field_adobe_publish_uploaded',
      'label' => t('Upload to Adobe Publish'),
      'type' => 'list_integer',
      'settings' => array(
        'allowed_values' => array(
          1 => t('Upload to Adobe Publish'),
        ),
      ),  
    'cardinality' => '-1'),
    'field_adobe_publish_short_title' => array(
      'field_name' => 'field_adobe_publish_short_title',
      'label' => t('Adobe Publish Short Title'),
      'type' => 'text'),
    'field_adobe_publish_abstract' => array(
      'field_name' => 'field_adobe_publish_abstract',
      'label' => t('Adobe Publish Abstract'),
      'type' => 'text_long'),
    'field_adobe_publish_short_ab' => array(
      'field_name' => 'field_adobe_publish_short_ab',
      'label' => t('Adobe Publish Short Abstract'),
      'type' => 'text'),
    'field_adobe_publish_author' => array(
      'field_name' => 'field_adobe_publish_author',
      'label' => t('Adobe Publish Author'),
      'type' => 'text'),
    'field_adobe_publish_author_url' => array(
      'field_name' => 'field_adobe_publish_author_url',
      'label' => t('Adobe Publish Author URL'),
      'type' => 'text'),
    'field_adobe_publish_browse' => array(
      'field_name' => 'field_adobe_publish_browse',
      'label' => t('Hide from Browse Page'),
      'type' => 'list_integer',
      'settings' => array(
        'allowed_values' => array(
          1 => t('Hide from Browse Page'),
        ),
      ),  
      'cardinality' => '-1'),
    'field_adobe_publish_department' => array(
      'field_name' => 'field_adobe_publish_department',
      'label' => t('Adobe Publish Department'),
      'type' => 'text'), 
    'field_adobe_publish_category' => array(
      'field_name' => 'field_adobe_publish_category',
      'label' => t('Adobe Publish Category'),
      'type' => 'text'),
    'field_adobe_publish_keywords' => array(
      'field_name' => 'field_adobe_publish_keywords',
      'label' => t('Keywords'),
      'type' => 'text'),
    'field_adobe_publish_int_key' => array(
      'field_name' => 'field_adobe_publish_int_key',
      'label' => t('Internal Keywords'),
      'type' => 'text'),
    'field_adobe_publish_thumb' => array(
      'field_name' => 'field_adobe_publish_thumb',
      'label' => t('Thumbnail Image'),
      'type' => 'image'),
  );
  $stylesheets = dpsbridge_helper_next_get_stylesheets();
  $install_fields['field_adobe_publish_style'] = array(
    'field_name' => 'field_adobe_publish_style',
    'label' => t('Adobe Publish Stylesheet'),
    'type' => 'list_text',
    'settings' => array(
      'allowed_values' => $stylesheets,
      'allowed_values_function' => '',
    ),
  );
      
  return $install_fields;
}
/**
 * Helper method to create/update article metadata on remote producer. Upload background/thumbnail
 * images if uploaded. Create metadata if entity not exists or update metadata if exists.
 * 
 * @param type $node
 *   Node to be added
 */
function dpsbridge_helper_next_create_article($node) {
  $title = $node->title;
  $nid = $node->nid;
  $container = array();

  $entity_name = dpsbridge_helper_next_get_entityname($nid, $title);
  
  $short_title = isset($node->field_adobe_publish_short_title[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_short_title[LANGUAGE_NONE][0]['value'] : '';
  $abstract = isset($node->field_adobe_publish_abstract[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_abstract[LANGUAGE_NONE][0]['value'] : '';
  $short_abstract = isset($node->field_adobe_publish_short_ab[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_short_ab[LANGUAGE_NONE][0]['value'] : '';
  $category = isset($node->field_adobe_publish_category[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_category[LANGUAGE_NONE][0]['value'] : '';
  $author = isset($node->field_adobe_publish_author[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_author[LANGUAGE_NONE][0]['value'] : '';
  $author_url = isset($node->field_adobe_publish_author_url[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_author_url[LANGUAGE_NONE][0]['value'] : '';
  $hide_from_browse = isset($node->field_adobe_publish_browse[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_browse[LANGUAGE_NONE][0]['value'] : '';
  $department = isset($node->field_adobe_publish_department[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_department[LANGUAGE_NONE][0]['value'] : '';
  $keywords = isset($node->field_adobe_publish_keywords[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_keywords[LANGUAGE_NONE][0]['value'] : '';
  $internal_keywords = isset($node->field_adobe_publish_int_key[LANGUAGE_NONE][0]['value']) ? $node->field_adobe_publish_int_key[LANGUAGE_NONE][0]['value'] : '';
  //$importance = isset($node->field_collection_importance[LANGUAGE_NONE][0]['value']) ? $node->field_collection_importance[LANGUAGE_NONE][0]['value'] : '';
  $hide_from_browse = ($hide_from_browse == '1') ? TRUE : FALSE;
 
  $keywords = dpsbridge_helper_next_get_keywords($nid, $keywords);
  $internal_keywords = dpsbridge_helper_next_get_keywords($nid, $internal_keywords);
  
  $variables = array();
  $request_params = DPSNextUtil::getRequestParams($variables);
  $credentials = $request_params['credentials'];
  $parameters = $request_params['parameters'];
  $endpoints = $request_params['endpoints'];
  $metadata_post = array(
    'title' => $title,
    'abstract' => $abstract,
    'shortTitle' => $short_title,
    'shortAbstract' => $short_abstract,
    'department' => $department,
    'category' => $category,
    'author' => $author,
    'authorUrl' => $author_url,
    'keywords' => $keywords,
    'internalKeywords' => $internal_keywords,
    'hideFromBrowsePage' => $hide_from_browse,
    // 'importance' => $importance,
  ); 
  // initialize article one object
  $articleOneObj = new DPSNextArticle($credentials,
                 $parameters,
                 $endpoints);
  $articleOneObj->setEntityName($entity_name);
  
  // Authentication
  $is_authenticated = $articleOneObj->isAuthenticated();
  if ($is_authenticated) {
    // Authorization and store pulication/project path (This should be done with UI instead)
    $articleOneObj->getPermissions();
    $entity_exists = TRUE;
    try {
      $articleOneObj->getMetadata();
    } catch (DPSNextException $ex) {
      if ($ex->getCode() == '404') {
        // Entity not found
        $entity_exists = FALSE;
      }
      else {
        // Other error
        drupal_set_message($ex->getMessage(), 'error');
        watchdog('dpsbridge', $ex->getMessage());
        exit();
      }
    }
    $articleOneObj->create($metadata_post, $entity_exists);
    // Upload thumbnail image if uploaded
    $file_exists = FALSE;
    
    $field_thumb_image = field_get_items('node', $node, 'field_adobe_publish_thumb');
    if (isset($field_thumb_image[0]['fid']) && !empty($field_thumb_image[0]['fid'])) {
      $file_exists = TRUE;
      $img_path = file_load($field_thumb_image[0]['fid'])->uri;
      $thumb_image_url = drupal_realpath($img_path);
      $articleOneObj->uploadImage($thumb_image_url, 'thumbnail');
    }
    
    if ($file_exists) {
      // update collection with reference to the background & thumbnail image
      $articleOneObj->getMetadata();
      $articleOneObj->update();
      $articleOneObj->seal();
    }
   
    //node_save($node);
    
    // Generate html article
    $article_node = dpsbridge_helper_pull_node($node->nid, $node);
    dpsbridge_helper_next_generate_html($article_node, $node, FALSE);
    
    // Zip up the HTML article folder.
    // Locations of targeted HTML article folder to designated .article file.
    $filename    = dpsbridge_helper_format_title($node->title);
    $directory   = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge';
    $source      = $directory . '/html/' . $filename;
    $destination = $directory . '/folio/' . $filename . ".article";
    
    // initialize manifest builder object
    $manifestBuilder = new DPSNextManifest();
    // add the path to the article directory
    $manifestBuilder->addDir($source);
    // zip the article directory contents
    $manifestBuilder->buildArticle($destination);
    $article_zipfile_path = DPSNextUtil::get_article_file_path($node->title);

    $articleOneObj->uploadArticle($article_zipfile_path);
    // request and store the latest article one version
    $articleOneObj->getMetadata();
    // request for article manifest, to verify the uploaded .article contents
    //$articleOneObj->getManifest();
    
    $version_id = $articleOneObj->version;
    // request and store the latest article one contentVersion
    //$articleOneObj->getMetadata();
  }
  if ($version_id) {
    dpsbridge_helper_next_set_version($nid, $version_id);
    $container['message'] = 'ok';
    $container['version_id'] = $version_id;
  } 
  else {
    $container['message'] = '[Error] Failed to create metadata!';
  }
  
  return $container;
}
/**
 * Helper method to create/update collection metadata on remote producer. Upload background/thumbnail
 * images if uploaded. Create metadata if entity not exists or update metadata if exists.
 * 
 * @param type $node
 *   Node to be added
 */
function dpsbridge_helper_next_create_collection($node) {
  $container = array();
  if ($node->type == 'collection') {
    $title = $node->title;
    $nid = $node->nid;
    $container = array();

    $entity_name = dpsbridge_helper_next_get_entityname($nid, $title);
    //$keywords = dpsbridge_helper_next_get_keywords($article_metadata['node']['nid'], $article_metadata['tags']);
    $short_title = isset($node->field_collection_short_title[LANGUAGE_NONE][0]['value']) ? $node->field_collection_short_title[LANGUAGE_NONE][0]['value'] : '';
    $abstract = isset($node->field_collection_abstract[LANGUAGE_NONE][0]['value']) ? $node->field_collection_abstract[LANGUAGE_NONE][0]['value'] : '';
    $short_abstract = isset($node->field_collection_short_abstract[LANGUAGE_NONE][0]['value']) ? $node->field_collection_short_abstract[LANGUAGE_NONE][0]['value'] : '';
    $category = isset($node->field_collection_category[LANGUAGE_NONE][0]['value']) ? $node->field_collection_category[LANGUAGE_NONE][0]['value'] : '';
    $keywords = isset($node->field_collection_keywords[LANGUAGE_NONE][0]['value']) ? $node->field_collection_keywords[LANGUAGE_NONE][0]['value'] : '';
    $internal_keywords = isset($node->field_collection_int_keywords[LANGUAGE_NONE][0]['value']) ? $node->field_collection_int_keywords[LANGUAGE_NONE][0]['value'] : '';
    $reading_position = isset($node->field_collection_reading_pos[LANGUAGE_NONE][0]['value']) ? $node->field_collection_reading_pos[LANGUAGE_NONE][0]['value'] : '';
    $opento = isset($node->field_collection_opento[LANGUAGE_NONE][0]['value']) ? $node->field_collection_opento[LANGUAGE_NONE][0]['value'] : '';
    $importance = isset($node->field_collection_importance[LANGUAGE_NONE][0]['value']) ? $node->field_collection_importance[LANGUAGE_NONE][0]['value'] : '';
    $department = isset($node->field_collection_department[LANGUAGE_NONE][0]['value']) ? $node->field_collection_department[LANGUAGE_NONE][0]['value'] : '';
    $keywords = dpsbridge_helper_next_get_keywords($nid, $keywords);
    $internal_keywords = dpsbridge_helper_next_get_keywords($nid, $internal_keywords);
    
    $variables = array();
    $request_params = DPSNextUtil::getRequestParams($variables);
    $credentials = $request_params['credentials'];
    $parameters = $request_params['parameters'];
    $endpoints = $request_params['endpoints'];
    $collection_metadata_post = array(
      'title' => $title,
      'abstract' => $abstract,
      'shortTitle' => $short_title,
      'shortAbstract' => $short_abstract,
      'department' => $department,
      'category' => $category,
      'keywords' => $keywords,
      'internalKeywords' => $internal_keywords,
      'readingPosition' => $reading_position,
      'openTo' => $opento,
      'importance' => $importance,
    ); 
    // initialize article one object
    $articleOneObj = new DPSNextCollection($credentials,
                   $parameters,
                   $endpoints);
    $articleOneObj->setEntityName($entity_name);
    
    // Authentication
    $is_authenticated = $articleOneObj->isAuthenticated();
    if ($is_authenticated) {
      // Authorization and store pulication/project path (This should be done with UI instead)
      $articleOneObj->getPermissions();
      $entity_exists = TRUE;
      try {
        $articleOneObj->getMetadata();
      } catch (DPSNextException $ex) {
        if ($ex->getCode() == '404') {
          // Entity not found
          $entity_exists = FALSE;
        }
        else {
          // Other error
          drupal_set_message($ex->getMessage(), 'error');
          watchdog('dpsbridge', $ex->getMessage());
          exit();
        }
      }
    
      $articleOneObj->create($collection_metadata_post, $entity_exists);
      // Upload thumbnail image if uploaded
      $file_exists = FALSE;
      $field_thumb_image = field_get_items('node', $node, 'field_collection_image_thumb');
      if (isset($field_thumb_image[0]['fid']) && !empty($field_thumb_image[0]['fid'])) {
        $file_exists = TRUE;
        $img_path = file_load($field_thumb_image[0]['fid'])->uri;
        $thumb_image_url = drupal_realpath($img_path);
        $articleOneObj->uploadImage($thumb_image_url, 'thumbnail');
      }
      // Upload background image if uploaded
      $field_back_image = field_get_items('node', $node, 'field_collection_image_back');
      if (isset($field_back_image[0]['fid']) && !empty($field_back_image[0]['fid'])) {
        $file_exists = TRUE;
        $img_path = file_load($field_back_image[0]['fid'])->uri;
        $back_image_url = drupal_realpath($img_path);
        $articleOneObj->uploadImage($back_image_url, 'background');
      }
      if ($file_exists) {
        // update collection with reference to the background & thumbnail image
        $articleOneObj->getMetadata();
        $articleOneObj->update();
        $articleOneObj->seal();
      }
      $version_id = $articleOneObj->version;
      // request and store the latest article one contentVersion
      //$articleOneObj->getMetadata();
    }
    if ($version_id) {
      dpsbridge_helper_next_set_version($nid, $version_id);
      $container['message'] = 'ok';
      $container['version_id'] = $version_id;
    } 
    else {
      $container['message'] = '[Error] Failed to create metadata!';
    }
  }
  return $container;
}


/**
 * Helper method to get entity name.
 * 
 * @param int $node_id
 *   The node id.
 * @param string $title
 *   The title of entity
 */
function dpsbridge_helper_next_get_entityname($node_id, $title) {
  // Removed article title from entity name to make sure entity name is unique. 
  // This will fix the bug that it updated metadata of a wrong entity if entity's title is changed.
  //$entity_name = 'dpsbridge_' . $node_id . '_' . dpsbridge_helper_format_title($title);
  $node = node_load($node_id);
  $created = '';
  if ($node) {
    $created = '_' . $node->created;
  }

  $entity_name = 'dpsbridge_' . $node_id . $created;
  return $entity_name;
}
/**
 * Helper method to get keywords array for a given entity
 * 
 * @param int $node_id 
 *   The node id
 * @param string $tags
 *   The tags string. E.g. "tag 1, tag2, tag3" 
 * 
 */
function dpsbridge_helper_next_get_keywords($node_id, $tags = '') {
  $article_keywords = !isset($node_id) ? array() : array("DPSBridge-" . $node_id);
  if ($tags != '') {
    $keywords = explode(',', $tags); 
    if (is_array($keywords)) {
      foreach ($keywords as $k) {
        $article_keywords[] = trim($k);
      }
    }  
  }
  return $article_keywords;
}

/**
 * Helper method for setting version id of a node.
 * 
 * @param $node_id
 *   The node id.
 * @param $version_id
 *   The version id.
 */
function dpsbridge_helper_next_set_version($node_id, $version_id) {
  $existed = dpsbridge_helper_next_get_version($node_id);
  if ($existed === FALSE) {
    // Insert
    $node_id = db_insert('dpsbridge_node_versions')
    ->fields(array(
      'timestamp' => REQUEST_TIME,
      'version_id' => $version_id,
      'nid' => $node_id,
    ))
    ->execute();
  } 
  else {
    // Update
    $node_id = db_update('dpsbridge_node_versions')
    ->fields(array(
      'timestamp' => REQUEST_TIME,
      'version_id' => $version_id,
    ))
    ->condition('nid', $node_id)   
    ->execute();
  }
  
  return $node_id;
}
/**
 * Helper method to get version id of a node
 * @param $node_id
 *   The node id. 
 */
function dpsbridge_helper_next_get_version($node_id) {
  $query = db_select('dpsbridge_node_versions', 'dnv')
      ->fields('dnv', array('version_id'))
      ->condition('nid', $node_id);
  $result = $query->execute();
  $version_id = $result->fetchField();
  
  return $version_id;
}
/**
 * Helper method for creating the Article structure for DPSNext.
 *
 * @param string $filename
 *   Filtered Article name, used as folder and file name.
 */
function dpsbridge_helper_next_create_dir($filename) {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  if (!file_exists($directory)) {
    mkdir($directory);
  }
  if (!file_exists($directory . '/' . $filename)) {
    mkdir($directory . $filename);
    $manifest_xml = fopen($directory . $filename . '/manifest.xml', 'w+');
    $index_html = fopen($directory . $filename . '/index.html', 'w+');

    fclose($manifest_xml);
    fclose($index_html);
  }
}
/**
 * Helper method for writing DPSNext based Article.xml file.
 *
 * Located in /dpsbridge/html/[filename]/Article.xml
 *
 * @param string $title
 *   The article title, used as the filename after removing special characters.
 * @param string $author
 *   The article author.
 * @param string $mag_title
 *   The magazine title.
 * @param string $folio_num
 *   The folio number.
 * @param string $desc
 *   The folio description.
 * @param string $kicker
 *   The folio kicker.
 * @param string $tags
 *   The folio tags.
 * @param string $is_ads
 *   Toggle to indicate if the article is an advertisement.
 * @param string $version
 *   The targeted folio version.
 * @param string $dimension
 *   The targeted folio dimension.
 * @param string $orientation
 *   The article orientation, default to both orientations.
 */
function dpsbridge_helper_next_writearticlexml($title, $author, $mag_title, $folio_num, $desc, $kicker, $tags, $is_ads, $version, $dimension, $orientation = 'Always') {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $date = date('Y-m-d\TH:i:s');
  $filename = dpsbridge_helper_format_title($title);
  $targeted = explode(' x ', $dimension);
  $width = $targeted[1];
  $height = $targeted[0];
  $is_ads = ($is_ads == TRUE) ? 'true' : 'false';

  // Generate the content of the Folio.xml.
  $xml  = "<?xml version='1.0' encoding='UTF-8'?>\n";
  $xml .= "<article version='" . $version . "' id='" . $filename . "' lastUpdated='" . $date . "' date='" . $date . "'>\n";
  $xml .= "<metadata>\n";
  $xml .= "<description>" . $desc . "</description>\n";
  $xml .= "<magazineTitle>" . $mag_title . "</magazineTitle>\n";
  $xml .= "<folioNumber>" . $folio_num . "</folioNumber>\n";
  $xml .= "</metadata>\n";
  $xml .= "<targetDimensions>\n";
  $xml .= "<targetDimension wideDimension='" . $height . "' narrowDimension='" . $width . "'/>";
  $xml .= "</targetDimensions>\n";
  $xml .= "<contentStacks>\n";
  $xml .= "<contentStack lastUpdated='" . $date . "' id='" . $filename . "' smoothScrolling='always' layout='vertical' alwaysDisplayOverlays='false'>\n";
  $xml .= "<content>\n";
  $xml .= "<previews>\n";
  $xml .= "<toc>toc.png</toc>\n";
  $xml .= "</previews>\n";
  $xml .= "<regions>\n";
  $xml .= "<region>\n";
  $xml .= "<portraitBounds>\n";
  $xml .= "<rectangle x='0' y='0' width='" . $width . "' height='" . $height . "'/>\n";
  $xml .= "</portraitBounds>\n";
  $xml .= "<landscapeBounds>\n";
  $xml .= "<rectangle x='0' y='0' width='" . $height . "' height='" . $width . "'/>\n";
  $xml .= "</landscapeBounds>\n";
  $xml .= "<metadata lastUpdated='2012-10-31T15:01:07Z'>\n";
  $xml .= "<title>" . $title . "</title>\n";
  $xml .= "<description>" . $desc . "</description>\n";
  $xml .= "<author>" . $author . "</author>\n";
  $xml .= "<tags>" . $tags . "</tags>\n";
  $xml .= "<kicker>" . $kicker . "</kicker>\n";
  $xml .= "<isAdvertisement>" . $is_ads . "</isAdvertisement>\n";
  $xml .= "<orientation>" . strtolower($orientation) . "</orientation>\n";
  /*$xml .= "<smoothScrolling>Always</smoothScrolling>\n";*/
  $xml .= "</metadata>\n";
  $xml .= "</region>\n";
  $xml .= "</regions>\n";
  $xml .= "<assets>\n";
  $xml .= "<asset landscape='false'>\n";
  $xml .= "<assetRendition type='web' paginated='false' source='" . $filename . ".html' includesOverlays='true' width='" . $width . "' height='" . $height . "' role='content'/>\n";
  $xml .= "</asset>\n";
  $xml .= "<asset landscape='true'>\n";
  $xml .= "<assetRendition type='web' paginated='false' source='" . $filename . ".html' includesOverlays='true' width='" . $height . "' height='" . $width . "' role='content'/>\n";
  $xml .= "</asset>\n";
  $xml .= "</assets>\n";
  $xml .= "</content>\n";
  $xml .= "</contentStack>\n";
  $xml .= "</contentStacks>\n";
  $xml .= "</article>\n";
  file_put_contents($directory . $filename . '/Article.xml', $xml);
}
/**
 * Helper method for writing the pkgproperties.xml file for DPSNext.
 *
 * Located in /dpsbridge/html/[filename]/META-INF/pkgproperties.xml
 *
 * @param string $filename
 *   Same as the article name.
 */
function dpsbridge_helper_next_writepkg($filename) {
  $directory   = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $current_date = date('Y-m-d\TH:i:s');
  // Generate the content of the pkgproperties.xml.
  $pkg  = "<?xml version='1.0' encoding='UTF-8'?>\n";
  $pkg .= "<pkgProperties xmlns='http://ns.adobe.com/ucf/1.0/'>\n";
  $pkg .= "<package/>\n";
  $pkg .= "<entries>\n";
  $pkg .= "<entry path='Article.xml'>\n";
  $pkg .= "<prop key='datetime'>" . $current_date . "</prop>\n";
  $pkg .= "</entry>\n";
  $pkg .= "<entry path='" . $filename . ".html'>\n";
  $pkg .= "<prop key='datetime'>" . $current_date . "</prop>\n";
  $pkg .= "</entry>\n";
  $pkg .= "</entries>\n";
  $pkg .= "</pkgProperties>\n";
  file_put_contents($directory . $filename . '/META-INF/pkgproperties.xml', $pkg);
}
/**
 * Helper method for writing the manifest.xml file for DPSNext.
 *
 * Located in /dpsbridge/html/[filename]/manifest.xml
 *
 * @param string $filename
 *   Same as the article name.
 */
function dpsbridge_helper_next_writemanifest($filename) {
  $directory   = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge';
  $source      = $directory . '/html/' . $filename;
  $manifestBuilder = new DPSNextManifest();
  // add the path to the article directory
  $manifestBuilder->addDir($source);
  // iterate the article directory and generate the manifest
  $manifestBuilder->generateXML();
  // write the manifest into XML file
  $manifestBuilder->writeXML();
}
/**
 * Helper method to extract stylesheet to html articl
 * @param type $stylesheet
 */
function dpsbridge_helper_next_extract_stylesheet($stylesheet, $filename) {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge';
  $source      = $directory . "/styles/" . $stylesheet . "/HTMLResources.zip";
  $destination = $directory . "/html/{$filename}/";
  // Linux built-in zipper.
  $zipper      = "unzip -o " . $source . " -d " . $destination;
  exec($zipper);
}
/**
 * Helper method for generating DPSNext based HTML article folder.
 *
 * @param object $article_metadata
 *   A list of metadata fields pertaining to the article node.
 * @param object $node_metadata
 *   A list of metadata fields posted in node form
 * @param object $ads_toggle
 *   A list of 0's and 1's, 1 for ads and 0 for not.
 * @param object $article_list
 *   A list of article names, used for generating the ToC.
 * @param object $ds_articles
 *   A list of Display Suite articles.
 */
function dpsbridge_helper_next_generate_html($article_metadata, $node_metadata, $ads_toggle, $article_list = '', $ds_articles = array()) {
  // Filter the title to create the folder & filename.
  $filename = dpsbridge_helper_format_title($node_metadata->title);
  
  $current_dir = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge';
  if (file_exists($current_dir . '/html/' . $filename)) {
    dpsbridge_helper_rrmdir($current_dir . '/html/' . $filename);
  }
  
  // Create the directory and its default files.
  dpsbridge_helper_next_create_dir($filename);
  // Write to the PKGProperties.xml.
  //dpsbridge_helper_next_writepkg($filename);
  
  if (empty($article_metadata['node'])) {
    $article_metadata['tags'] .= "DPSBridge";
  }
  else {
    $article_metadata['tags'] .= "DPSBridge-" . $node_metadata->nid;
  }

  // Import Stylesheets
  if ($node_metadata->field_adobe_publish_style[LANGUAGE_NONE][0]['value'] != '') {
    dpsbridge_helper_next_extract_stylesheet($node_metadata->field_adobe_publish_style[LANGUAGE_NONE][0]['value'], $filename);
  }
  
  // Generate the HTML article with Foundation stylesheet.
  dpsbridge_helper_next_style_foundation(
      '',
      $node_metadata->title,
      $article_metadata['author'],
      $article_metadata['kicker'],
      $article_metadata['body'],
      $article_metadata['image'],
      $article_metadata['video'],
      $article_metadata['ds_content']
  );
  
  dpsbridge_helper_next_writemanifest($filename);
}