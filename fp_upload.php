<?php

require_once dirname(__FILE__).'/fp_config.php';
$config = FPConfig::Instance();
require_once dirname(__FILE__).'/fp_helper.php';
require_once dirname(__FILE__).'/dpsbridge_helper.inc';

if (!isset($_SESSION['AdobeID']) || !isset($_SESSION['Password'])) {
	echo "Missing Adobe ID and password!";
} else {
	$fp        = new FPHelper($_SESSION['AdobeID'],$_SESSION['Password'],$_SESSION['APIKey'],$_SESSION['APISecret']);
	$folio_id   = isset($_POST['folioID'])?$_POST['folioID']:'';
	$filenames = isset($_POST['filenames'])?$_POST['filenames']:'';
	$alienated = isset($_POST['alienated'])?$_POST['alienated']:'';
	$landscape = isset($_POST['landscape'])?$_POST['landscape']:'';
	$portrait  = isset($_POST['portrait'])?$_POST['portrait']:'';
	$dimension = isset($_POST['dimension'])?$_POST['dimension']:'';
	$status    = isset($_POST['status'])?$_POST['status']:'';
	$style     = isset($_POST['style'])?$_POST['style']:'';

	$offset = 0;
	$alienatedArrayCounter = 0;
	$split  = explode(' x ', $dimension);
	$width  = $split[0];
	$height = $split[1];
	// if given a landscape image, scale and upload it as cover preview landscape image
	if ($landscape) {
		$landscape 	        = substr($landscape, stripos($landscape, 'images'));
		$landscape_temp_url = dpsbridge_helper_scale_img($landscape, $width, $height, 'landscape');
		$fp->upload_cover($folio_id, 'landscape', $landscape_temp_url);
		unlink($landscape_temp_url);
	}
	// if given a portrait image, scale and upload it as cover preview portrait image
	if ($portrait) {
		$portrait          = substr($portrait, stripos($portrait, 'images'));
		$portrait_temp_url = dpsbridge_helper_scale_img($portrait, $height, $width, 'portrait');
		$fp->upload_cover($folio_id, 'portrait', $portrait_temp_url);
		unlink($portrait_temp_url);
	}
	// attempts to upload the HTML Resources zip file
	$response = $fp->upload_htmlresources($folio_id, 'styles/'.$style.'/HTMLResources.zip');
	if ($response['status'] === 'ok') {
		echo ' - Success: HTMLResource<br/>';
	} else {
		echo ' - Failed: HTMLResource <br/>:: ';
		print_r($response);
		echo "<br/>";
	}
	// if the targeted Folio folder ID and the list of local .folio file names are given
	if ($folio_id && $filenames) {
		// if this is not the first time uploading, delete the existing HTML articles
		if ($status == 'Uploaded') {
			// calls helper to pull the article metadata from the designated Folio folder in Folio Producer
			$articles = $fp->articles($folio_id);
			$articles = $articles['articles'];
			for ($n = 0; $n < count($articles); $n++) {
				if ($articles[$n]['articleMetadata']['assetFormat'] == 'Auto') { // if article is HTML base
					// call helper to delete the article from the designated Folio folder in the Folio Producer
					$fp->delete_article($folio_id, $articles[$n]['id']);
				} else {
					// call helper to reset non-Drupal article's sort number
					$fp->update_article($folio_id, $articles[$n]['id'], array('sortOrder' => intval($n)));
				}
			}
		}
		// increment the offset if there is a cover page, for sorting purposes
		if (count($filenames) > 0 && $filenames[0] == 'Cover')
			$offset++;
		// increment the offset if there is a table of contents page, for sorting purposes
		if (count($filenames) > 1 && $filenames[1] == 'TableofContents')
			$offset++;
		// loops through the articles and upload them to the Folio Producer
		for ($i = 0; $i < count($filenames); $i++) {
			$adjustedSortOrder = ($i+1+$offset)*1000;
			if ($filenames[$i] == '') { // checks if the article is not from Drupal
				// updates the sort order of the non-Drupal article
				$fp->update_article($folio_id, $alienated[$alienatedArrayCounter], array('sortOrder' => intval($adjustedSortOrder)));
				$alienatedArrayCounter++;
				continue;
			}
			$sourcePath	= 'folio/'.dpsbridge_helper_format_title($filenames[$i]).'.folio';
			$response = $fp->upload_article($folio_id, array('sortOrder' => intval($adjustedSortOrder)), $sourcePath);
			// locking the article
			//$fp->update_article($folio_id, $response['articleInfo']['id'], array('locked' => 'true'));
			if ($response['status'] === 'ok') {
				echo "<br/> - Success: ".$filenames[$i]."<br/>";
			} else {
				echo "<br/> - Failed: ".$filenames[$i].' <br/> :: ';
				print_r($response);
				echo "<br/>";
			}
		}
	} else {
		echo " - Failed: Articles <br/> :: Missing one or more of the following: Folio ID, article name, sort order, and target viewer!";
	}
}
