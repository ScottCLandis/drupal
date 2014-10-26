<?php
/**
 * @file
 * DPSBridge module helper functions.
 */

/**
 * To check if article already exists within the selected folio node.
 *
 * Loops through the existing articles in the folio node to determine if an
 * article exists within the folio.
 *
 * @param string $article_id
 *   Article node id.
 * @param object $folio_node_array
 *   Array containing a list of article node ID's.
 */
function dpsbridge_helper_check_existence($article_id, $folio_node_array) {
  for ($i = 0; $i < count($folio_node_array); $i++) {
    if ($article_id == $folio_node_array[$i]['target_id']) {
      return TRUE;
    }
  }
  return FALSE;
}
/**
 * Helper method for creating the Folio structure.
 *
 * @param string $filename
 *   Filtered Article name, used as folder and file name.
 */
function dpsbridge_helper_create_dir($filename) {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  if (!file_exists($directory)) {
    mkdir($directory);
  }
  if (!file_exists($directory . '/' . $filename)) {
    mkdir($directory . $filename);
    mkdir($directory . $filename . '/META-INF');
    $folio_xml = fopen($directory . $filename . '/Folio.xml', 'w+');
    $index_html = fopen($directory . $filename . '/' . $filename . '.html', 'w+');
    $mimetype = fopen($directory . $filename . '/mimetype', 'w+');
    $metadata = fopen($directory . $filename . '/META-INF/pkgproperties.xml', 'w+');
    $mimecontent = "application/vnd.dpsbridge.folio+zip";
    file_put_contents($directory . $filename . '/mimetype', $mimecontent);
    fclose($folio_xml);
    fclose($index_html);
    fclose($mimetype);
    fclose($metadata);
  }
}
/**
 * Helper method for saving the Adobe DPS credentials as a Drupal 7 user.
 *
 * @param string $uid
 *   Drupal user id.
 * @param string $id
 *   Adobe DPS user id.
 * @param string $pass
 *   Adobe DPS user password.
 * @param string $dimension
 *   Available dimensions per account type (not used ATM).
 * @param string $key
 *   Adobe DPS API key.
 * @param string $secret
 *   Adobe DPS API secret.
 */
function dpsbridge_helper_credential_update($uid, $id, $pass, $dimension, $key, $secret) {
  $user = user_load($uid);
  if (!$id || !$pass) {
    user_delete($uid);
  }
  elseif (empty($user)) {
    $credentials = array(
      'uid' => $uid,
      'name'  => $id,
      'mail'  => $pass,
      'pass'  => $pass,
      'theme' => $dimension,
      'status'  => 1,
      'signature' => $key,
      'signature_format' => $secret,
    );
    user_save('', $credentials);
  }
  else {
    $user->name = $id;
    $user->mail = $pass;
    $user->pass = $pass;
    $user->theme  = $dimension;
    $user->signature  = $key;
    $user->signature_format = $secret;
    user_save((object) array('uid' => $uid), (array) $user);
  }
}

/**
 * Helper method for unzipping the given stylesheet.
 *
 * Located in /dpsbridge/styles/[stylsheet name]/HTMLResources.zip,
 *   and place it in /dpsbridge/html/HTMLResources/
 *
 * @param string $stylesheet
 *   Name of the stylesheet to unzip.
 */
function dpsbridge_helper_extract_stylesheet($stylesheet) {
  $stylesheet  = str_replace(" ", "\ ", $stylesheet);
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge';
  $source      = $directory . "/styles/" . $stylesheet . "/HTMLResources.zip";
  $destination = $directory . "/html/HTMLResources";
  // Linux built-in zipper.
  $zipper      = "unzip " . $source . " -d " . $destination;
  exec($zipper);
}
/**
 * Helper method for taking a snapshot of the webpage via shrinktheweb module.
 *
 * Method to be tested thoroughly.
 *
 * @param string $url
 *   Webpage URL.
 *
 * @return string
 *   Containing the URL to the saved snapshot
 */
function dpsbridge_helper_format_thumbnail($url) {
  $formated_url = NULL;
  $img_url  = theme('shrinktheweb_image', array('url' => $url));
  $pattern = '<img src="([\w:\/.]+)" />';
  preg_match($pattern, $img_url, $formatted_url);
  if (!$formated_url) {
    return drupal_get_path("module", "dpsbridge") . "/images/icons/toc.png";
  }
  else {
    return $formated_url[1];
  }
}
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
 * Helper method for appending the articles to a folio node.
 *
 * @param string $fid
 *   Folio node ID.
 * @param object $articles
 *   List of article node ID's.
 */
function dpsbridge_helper_folio_append($fid, $articles) {
  $articles = explode(',', $articles);
  $folio = node_load($fid);
  for ($i = 0; $i < count($articles); $i++) {
    // If the array is empty but not null.
    if ($articles[$i] == '') {
      continue;
    }
    // If article already exists within the folio node.
    if (dpsbridge_helper_check_existence($articles[$i], $folio->field_folio_reference[LANGUAGE_NONE])) {
      continue;
    }
    // Appends the folio reference to the article.
    $article = node_load(intval($articles[$i]));
    if (isset($article->field_article_reference)) {
      $article->field_article_reference[LANGUAGE_NONE][]['target_id'] = $fid;
      node_save_action($article);
    }
    // Appends the article reference to the folio.
    $folio->field_folio_reference[LANGUAGE_NONE][]['target_id'] = $articles[$i];
  }
  node_save_action($folio);
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
 * Helper method for generating the HTML article folder.
 *
 * @param object $article_metadata
 *   A list of metadata fields pertaining to the article node.
 * @param object $folio_metadata
 *   A list of metadata fields pertaining to the folio node.
 * @param object $ads_toggle
 *   A list of 0's and 1's, 1 for ads and 0 for not.
 * @param object $article_list
 *   A list of article names, used for generating the ToC.
 * @param object $ds_articles
 *   A list of Display Suite articles.
 */
function dpsbridge_helper_generate_html($article_metadata, $folio_metadata, $ads_toggle, $article_list = '', $ds_articles = array()) {
  // Filter the title to create the folder & filename.
  $filename = dpsbridge_helper_format_title($article_metadata['title']);
  // Create the directory and its default files.
  dpsbridge_helper_create_dir($filename);
  // Write to the PKGProperties.xml.
  dpsbridge_helper_writepkg($filename);
  if (empty($article_metadata['node'])) {
    $article_metadata['tags'] .= "DPSBridge";
  }
  else {
    $article_metadata['tags'] .= "DPSBridge-" . $article_metadata['node']->nid;
  }

  // Write to the Folio.xml.
  dpsbridge_helper_writefolioxml(
      // The title of the article.
      $article_metadata['title'],
      // The author of the article.
      $article_metadata['author'],
      // The publication name.
      $folio_metadata['pubName'],
      // The folio number (not the same as sort number!).
      $folio_metadata['folioNum'],
      // The description of the article.
      $article_metadata['desc'],
      // The kicker of the article.
      $article_metadata['kicker'],
      // The tags of the article.
      $article_metadata['tags'],
      // Toggle for if the article is an advertisement.
      $ads_toggle,
      // The viewer version of the article (must match the parent Folio).
      $folio_metadata['folioVer'],
      // The target dimension of the article (must match the parent Folio).
      $folio_metadata['dimension'],
      // The article orientation .
      $folio_metadata['orientation']
  );
  // Checks if the given is the ToC, cover page, or an article.
  if ($article_list != '') {
    // Generate the Table of Contents with Bootstrap stylesheet.
    if ($folio_metadata['pubCSS'] == 'Bootstrap') {
      dpsbridge_helper_style_bootstrap_toc(
          $folio_metadata['pubName'],
          $article_list,
          $ds_articles
      );
    }
    // Generate the Table of Contents with Foundation stylesheet.
    else {
      dpsbridge_helper_style_foundation_toc(
          $folio_metadata['pubName'],
          $article_list,
          $ds_articles
      );
    }
  }
  elseif (isset($article_metadata['cover']) && $article_metadata['cover']) {
    // Generate the cover article with Bootstrap stylesheet.
    if ($folio_metadata['tocCSS'] == 'Bootstrap') {
      dpsbridge_helper_style_bootstrap_cover(
          // Publication name.
          $folio_metadata['pubName'],
          // Preview landscape image.
          $folio_metadata['landscape'],
          // Preview portrait image.
          $folio_metadata['portrait'],
          // Targeted dimension.
          $folio_metadata['dimension']
      );
    }
    // Generate the cover article with Foundation stylesheet.
    else {
      dpsbridge_helper_style_foundation_cover(
          $folio_metadata['pubName'],
          $folio_metadata['landscape'],
          $folio_metadata['portrait'],
          $folio_metadata['dimension']
      );
    }
  }
  elseif ($folio_metadata['pubCSS'] == 'Bootstrap') {
    // Generate the HTML article with Bootstrap stylesheet.
    dpsbridge_helper_style_bootstrap(
        // Publication name.
        $folio_metadata['pubName'],
        // Title of the article.
        $article_metadata['title'],
        // Author of the article.
        $article_metadata['author'],
        // Kicker of the article.
        $article_metadata['kicker'],
        // Body of the article.
        $article_metadata['body'],
        // Images from the article.
        $article_metadata['image'],
        // Videos from the article.
        $article_metadata['video'],
        $article_metadata['ds_content']
    );
  }
  else {
    // Generate the HTML article with Foundation stylesheet.
    dpsbridge_helper_style_foundation(
        $folio_metadata['pubName'],
        $article_metadata['title'],
        $article_metadata['author'],
        $article_metadata['kicker'],
        $article_metadata['body'],
        $article_metadata['image'],
        $article_metadata['video'],
        $article_metadata['ds_content']
    );
  }
}

/**
 * Helper method for including the css/js for each of the 3 view pages.
 *
 * @param string $toggle
 *   Includes the respective javascript depending on the view page.
 */
function dpsbridge_helper_include_goodies($toggle) {
  drupal_add_css('http://code.jquery.com/ui/1.10.3/themes/smoothness/jquery-ui.css', 'external');
  drupal_add_css(drupal_get_path('module', 'dpsbridge') . '/includes/override.css');
  drupal_add_js(drupal_get_path('module', 'dpsbridge') . '/includes/dpsbridge.js');
}
/**
 * Helper method for saving the given image url, to the predefined image path.
 *
 * @param string $filename
 *   Image URL.
 * @param string $images
 *   New name of the saved image.
 */
function dpsbridge_helper_link_img($filename, $images) {
  $parsed    = explode('/', $images);
  $img_name  = $parsed[count($parsed) - 1];
  dpsbridge_helper_save_img($images, $filename . '/' . $img_name);
  return $img_name;
}

/**
 * Helper method to pull the article node contents.
 *
 * @param string $node_id
 *   Article node ID (or any other types of node).
 *
 * @return array
 *   Containing the article metadata.
 */
function dpsbridge_helper_pull_article($node_id) {
  $container = array();
  $node      = node_load($node_id);
  if ($node->type != 'folios') {
    $container['title']     = isset($node->title) ? $node->title : '';
    $container['filename']  = isset($node->title) ? dpsbridge_helper_format_title($node->title) : '';
    $container['tags']      = isset($node->field_tags) ? $node->field_tags : '';
    $container['type']      = isset($node->type) ? $node->type : '';
    $container['modified']  = isset($node->changed) ? gmdate("m/d/Y - H:i", $node->changed) : '';
    $container['timestamp'] = isset($node->changed) ? date('Y-m-d\TH:i:s', $node->changed) : '';
  }
  return $container;
}

/**
 * Helper method for pulling folio node contents and the user credentials.
 *
 * @param string $node_id
 *   Folio node id.
 * @param string $toggle
 *   Toggle for pulling the account information.
 *
 * @return array
 *   Containing the folio metadata.
 */
function dpsbridge_helper_pull_content($node_id, $toggle = '') {

  // Load folio metadata.
  $container = array();
  $node = node_load($node_id);
  $lang = $node->language;
  $container['title'] = isset($node->title) ? $node->title : '';
  $container['pubID'] = isset($node->field_folio_product_id[$lang][0]['value']) ? $node->field_folio_product_id[$lang][0]['value'] : '';
  $container['pubName'] = isset($node->field_folio_publication_name[$lang][0]['value']) ? $node->field_folio_publication_name[$lang][0]['value'] : '';
  $container['folioNum'] = isset($node->field_folio_number[$lang][0]['value']) ? $node->field_folio_number[$lang][0]['value'] : '';
  $container['pubDate'] = isset($node->field_folio_published_date[$lang][0]['value']) ? $node->field_folio_published_date[$lang][0]['value'] : '';
  $container['folioDesc'] = isset($node->field_folio_description[$lang][0]['value']) ? $node->field_folio_description[$lang][0]['value'] : '';
  $container['folioVer'] = isset($node->field_folio_viewer_version[$lang][0]['value']) ? $node->field_folio_viewer_version[$lang][0]['value'] : '';
  $container['accounts'] = isset($node->field_folio_producer_accounts[$lang][0]['value']) ? $node->field_folio_producer_accounts[$lang][0]['value'] : '';
  $container['pubCSS'] = isset($node->field_folio_publication_css[$lang][0]['value']) ? $node->field_folio_publication_css[$lang][0]['value'] : '';
  $container['tocCSS'] = isset($node->field_folio_toc_layout[$lang][0]['value']) ? $node->field_folio_toc_layout[$lang][0]['value'] : '';
  $container['dimension'] = isset($node->field_folio_dimension[$lang][0]['value']) ? $node->field_folio_dimension[$lang][0]['value'] : '';
  $container['orientation'] = isset($node->field_folio_orientation[$lang][0]['value']) ? $node->field_folio_orientation[$lang][0]['value'] : '';
  $container['isAds'] = isset($node->field_folio_ads_filter[$lang][0]['value']) ? explode(',', $node->field_folio_ads_filter[$lang][0]['value']) : '';
  $container['filter'] = isset($node->field_folio_library_filter[$lang][0]['value']) ? $node->field_folio_library_filter[$lang][0]['value'] : '';
  $container['articles'] = isset($node->field_folio_reference[$lang]) ? $node->field_folio_reference[$lang] : '';
  $container['landscape'] = isset($node->field_folio_landscape_url[$lang][0]['value']) ? $node->field_folio_landscape_url[$lang][0]['value'] : '';
  $container['portrait'] = isset($node->field_folio_portrait_url[$lang][0]['value']) ? $node->field_folio_portrait_url[$lang][0]['value'] : '';
  $container['autoToggle'] = isset($node->field_folio_auto_generate[$lang][0]['value']) ? $node->field_folio_auto_generate[$lang][0]['value'] : '';
  $container['status'] = isset($node->field_folio_status[$lang][0]['value']) ? $node->field_folio_status[$lang][0]['value'] : '';
  $container['uploadDate'] = isset($node->field_folio_producer_timestamp[$lang][0]['value']) ? $node->field_folio_producer_timestamp[$lang][0]['value'] : '';
  $container['alienated'] = isset($node->field_folio_producer_article_id[$lang][0]['value']) ? explode(',', $node->field_folio_producer_article_id[$lang][0]['value']) : '';

  // Load user metadata.
  $amazon  = user_load('341');
  $android = user_load('342');
  $apple   = user_load('343');
  $container['amazonDimensions'] = isset($amazon->theme) ? explode(',', $amazon->theme) : explode(',', '960 x 640,');
  $container['androidDimensions'] = isset($android->theme) ? explode(',', $android->theme) : explode(',', '960 x 640,');
  $container['appleDimensions'] = isset($apple->theme) ? explode(',', $apple->theme) : explode(',', '480 x 320,1024 x 768,1136 x 640,2048 x 1536,');
  if ($toggle == 'sync') {
    switch ($container['accounts']) {
      case 'amazon':
        $container['credentials']['adobeID'] = $amazon->name;
        $container['credentials']['password'] = $amazon->mail;
        $container['credentials']['apiKey'] = $amazon->signature;
        $container['credentials']['apiSecret'] = $amazon->signature_format;
        break;

      case 'android':
        $container['credentials']['adobeID'] = $android->name;
        $container['credentials']['password'] = $android->mail;
        $container['credentials']['apiKey'] = $android->signature;
        $container['credentials']['apiSecret'] = $android->signature_format;
        break;

      case 'apple':
      default:
        $container['credentials']['adobeID'] = $apple->name;
        $container['credentials']['password'] = $apple->mail;
        $container['credentials']['apiKey'] = $apple->signature;
        $container['credentials']['apiSecret'] = $apple->signature_format;
        break;
    }
  }
  return $container;
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
function dpsbridge_helper_pull_node($node_id) {
  $container = array();
  // Loads the node from the database.
  $node = node_load($node_id);
  $container['node'] = $node;

  // Pull fields from dps node view.
  $node_display = node_view($node, 'dps');
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
 * Helper method to check for article node ref exist within the folio node.
 *
 * @param object $container
 *   List of article node references that are currently within the folio node.
 * @param string $target
 *   Pending article node reference.
 * @param string $lang
 *   Language of the folio node.
 */
function dpsbridge_helper_reference_check($container, $target, $lang) {
  for ($i = 0; $i < count($container); $i++) {
    if ($container[$lang][$i]['target_id'] == $target) {
      return TRUE;
    }
  }
  return FALSE;
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
 * Helper method for writing the Folio.xml file.
 *
 * Located in /dpsbridge/html/[filename]/Folio.xml
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
function dpsbridge_helper_writefolioxml($title, $author, $mag_title, $folio_num, $desc, $kicker, $tags, $is_ads, $version, $dimension, $orientation = 'Always') {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $date = date('Y-m-d\TH:i:s');
  $filename = dpsbridge_helper_format_title($title);
  $targeted = explode(' x ', $dimension);
  $width = $targeted[1];
  $height = $targeted[0];
  $is_ads = ($is_ads == TRUE) ? 'true' : 'false';

  // Generate the content of the Folio.xml.
  $xml  = "<?xml version='1.0' encoding='UTF-8'?>\n";
  $xml .= "<folio version='" . $version . "' id='" . $filename . "' lastUpdated='" . $date . "' date='" . $date . "'>\n";
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
  $xml .= "</folio>\n";
  file_put_contents($directory . $filename . '/Folio.xml', $xml);
}
/**
 * Helper method for writing the pkgproperties.xml file.
 *
 * Located in /dpsbridge/html/[filename]/META-INF/pkgproperties.xml
 *
 * @param string $filename
 *   Same as the article name.
 */
function dpsbridge_helper_writepkg($filename) {
  $directory   = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $current_date = date('Y-m-d\TH:i:s');
  // Generate the content of the pkgproperties.xml.
  $pkg  = "<?xml version='1.0' encoding='UTF-8'?>\n";
  $pkg .= "<pkgProperties xmlns='http://ns.adobe.com/ucf/1.0/'>\n";
  $pkg .= "<package/>\n";
  $pkg .= "<entries>\n";
  $pkg .= "<entry path='Folio.xml'>\n";
  $pkg .= "<prop key='datetime'>" . $current_date . "</prop>\n";
  $pkg .= "</entry>\n";
  $pkg .= "<entry path='toc.png'>\n";
  $pkg .= "<prop key='datetime'>" . $current_date . "</prop>\n";
  $pkg .= "</entry>\n";
  $pkg .= "<entry path='" . $filename . ".html'>\n";
  $pkg .= "<prop key='datetime'>" . $current_date . "</prop>\n";
  $pkg .= "</entry>\n";
  $pkg .= "</entries>\n";
  $pkg .= "</pkgProperties>\n";
  file_put_contents($directory . $filename . '/META-INF/pkgproperties.xml', $pkg);
}
