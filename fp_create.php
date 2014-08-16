<?php

/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';
$config = DPSBridgeFPConfig::Instance();
require_once dirname(__FILE__) . '/fp_helper.php';

if (!isset($_SESSION['dpsbridge_AdobeID']) || !isset($_SESSION['dpsbridge_Password'])) {
  echo "Please provide Adobe ID and password!";
}
else {
  $fp = new DPSBridgeFPHelper($_SESSION['dpsbridge_AdobeID'], $_SESSION['dpsbridge_Password'], $_SESSION['dpsbridge_APIKey'], $_SESSION['dpsbridge_APISecret']);
  $folio_name = isset($_POST["folioName"]) ? $_POST["folioName"] : '';
  $magazine_title = isset($_POST["magazineTitle"]) ? $_POST["magazineTitle"] : '';
  $folio_number = isset($_POST["folioNumber"]) ? $_POST["folioNumber"] : '';
  $folio_description = isset($_POST["folioDescription"]) ? $_POST["folioDescription"] : '';
  $publication_date = isset($_POST["publicationDate"]) ? date('Y-m-d\TH:i:s', strtotime($_POST['publicationDate'])) : '';
  $dimension = isset($_POST["dimension"]) ? explode(' x ', $_POST["dimension"]) : '';
  $default_asset_format = isset($_POST["defaultAssetFormat"]) ? $_POST["defaultAssetFormat"] : '';
  $default_jpeg_quality = isset($_POST["defaultJPEGQuality"]) ? $_POST["defaultJPEGQuality"] : '';
  $binding_right = isset($_POST["bindingRight"]) ? $_POST["bindingRight"] : '';
  $locked = isset($_POST["Locked"]) ? $_POST["Locked"] : '';
  $folio_intent = isset($_POST["folioIntent"]) ? $_POST["folioIntent"] : 'Both';
  $target_viewer = isset($_POST["targetViewer"]) ? $_POST["targetViewer"] : '20.1.1';
  $filters = isset($_POST["filters"]) ? $_POST["filters"] : '';
  $resolution_width = $dimension[0];
  $resolution_height = $dimension[1];
  $message = array();

  if ($folio_intent == 'Portrait') {
    $folio_intent = 'PortraitOnly';
  }
  elseif ($folio_intent == 'Landscape') {
    $folio_intent = 'LandscapeOnly';
  }
  elseif ($folio_intent == 'Always') {
    $folio_intent = 'Both';
  }

  if ($folio_name && $magazine_title && $folio_number && $resolution_width && $resolution_height) {
    $params = array(
      'folioName'  => $folio_name,
      'magazineTitle' => $magazine_title,
      'folioNumber' => $folio_number,
      'folioDescription' => $folio_description,
      'publicationDate' => $publication_date,
      'resolutionWidth' => $resolution_width,
      'resolutionHeight' => $resolution_height,
      'targetViewer' => $target_viewer,
      'folioIntent' => $folio_intent,
      'Filters' => $filters,
    );
    $response = $fp->createFolio($params);
    echo $response['folioID'];
  }
  else {
    echo ' - Folio Creation Failed: <br/>Missing one or more of the following: Folio name, Magazine title, Resolution Width & Height!';
  }
}
