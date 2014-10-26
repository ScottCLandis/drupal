<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';
$config = DPSBridgeFPConfig::Instance();
require_once dirname(__FILE__) . '/fp_helper.php';
require_once dirname(__FILE__) . '/dpsbridge_helper.inc';

if (!isset($_SESSION['dpsbridge_AdobeID']) || !isset($_SESSION['dpsbridge_Password'])) {
  echo "Missing Adobe ID and password!";
}
else {
  $fp        = new DPSBridgeFPHelper($_SESSION['dpsbridge_AdobeID'], $_SESSION['dpsbridge_Password'], $_SESSION['dpsbridge_APIKey'], $_SESSION['dpsbridge_APISecret']);
  $folio_id   = isset($_POST['folioID']) ? $_POST['folioID'] : '';
  $filenames = isset($_POST['filenames']) ? $_POST['filenames'] : '';
  $alienated = isset($_POST['alienated']) ? $_POST['alienated'] : '';
  $landscape = isset($_POST['landscape']) ? $_POST['landscape'] : '';
  $portrait  = isset($_POST['portrait']) ? $_POST['portrait'] : '';
  $dimension = isset($_POST['dimension']) ? $_POST['dimension'] : '';
  $status    = isset($_POST['status']) ? $_POST['status'] : '';
  $style     = isset($_POST['style']) ? $_POST['style'] : '';

  $offset = 0;
  $alienated_array_counter = 0;
  $split  = explode(' x ', $dimension);
  $width  = $split[0];
  $height = $split[1];
  // If given a landscape image, scale and upload it,
  // as cover preview landscape image.
  if ($landscape) {
    $landscape           = substr($landscape, stripos($landscape, 'images'));
    $landscape_temp_url = dpsbridge_helper_scale_img($landscape, $width, $height, 'landscape');
    $fp->uploadCover($folio_id, 'landscape', $landscape_temp_url);
    unlink($landscape_temp_url);
  }
  // If given a portrait image, scale and upload it,
  // as cover preview portrait image.
  if ($portrait) {
    $portrait          = substr($portrait, stripos($portrait, 'images'));
    $portrait_temp_url = dpsbridge_helper_scale_img($portrait, $height, $width, 'portrait');
    $fp->uploadCover($folio_id, 'portrait', $portrait_temp_url);
    unlink($portrait_temp_url);
  }
  // Attempts to upload the HTML Resources zip file.
  $response = $fp->uploadHTMLResources($folio_id, 'styles/' . $style . '/HTMLResources.zip');
  if ($response['status'] === 'ok') {
    echo ' - Success: HTMLResource<br/>';
  }
  else {
    echo ' - Failed: HTMLResource <br/>:: ';
    print_r($response);
    echo "<br/>";
  }
  // If targeted Folio folder ID and list of local .folio file names are given.
  if ($folio_id && $filenames) {
    // If this is not the first time uploading, delete existing HTML articles.
    if ($status == 'Uploaded') {
      // Pull article metadata from designated Folio folder in Folio Producer.
      $articles = $fp->articles($folio_id);
      $articles = $articles['articles'];
      for ($n = 0; $n < count($articles); $n++) {
        // If article is HTML base?
        if (strstr($articles[$n]['articleMetadata']['tags'], 'DPSBridge')) {
          // Delete the article from Folio folder in the Folio Producer.
          $fp->deleteArticle($folio_id, $articles[$n]['id']);
        }
        else {
          // Reset non-Drupal article's sort number.
          $fp->updateArticle($folio_id, $articles[$n]['id'], array('sortOrder' => intval($n)));
        }
      }
    }
    // Increment the offset if there is a cover page, for sorting purposes.
    if (count($filenames) > 0 && $filenames[0] == 'Cover') {
      $offset++;
    }
    // Increment the offset if there is a TOC page, for sorting purposes.
    if (count($filenames) > 1 && $filenames[1] == 'TableofContents') {
      $offset++;
    }
    // Loop through the articles and upload them to the Folio Producer.
    for ($i = 0; $i < count($filenames); $i++) {
      $adjusted_sort_order = ($i + 1 + $offset) * 1000;
      // Checks if the article is not from Drupal.
      if ($filenames[$i] == '') {
        // Updates the sort order of the non-Drupal article.
        $fp->updateArticle($folio_id, $alienated[$alienated_array_counter], array('sortOrder' => intval($adjusted_sort_order)));
        $alienated_array_counter++;
        continue;
      }
      $source_path  = 'folio/' . dpsbridge_helper_format_title($filenames[$i]) . '.folio';
      $response = $fp->uploadArticle($folio_id, array('sortOrder' => intval($adjusted_sort_order)), $source_path);
      // Locking the article.
      // $fp->updateArticle($folio_id,
      // $response['articleInfo']['id'],array('locked' => 'true'));!
      if ($response['status'] === 'ok') {
        echo "<br/> - Success: " . $filenames[$i] . "<br/>";
      }
      else {
        echo "<br/> - Failed: " . $filenames[$i] . "<br/>";
        print_r($response);
        echo "<br/>";
      }
    }
  }
  else {
    echo " - Failed: Articles <br/> :: Missing one or more of the following: Folio ID, article name, sort order, and target viewer!";
  }
}
