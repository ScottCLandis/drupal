<?php

require_once dirname(__FILE__).'/fp_config.php';
$config = FPConfig::Instance();
require_once dirname(__FILE__).'/fp_helper.php';

if (!isset($_SESSION['AdobeID']) || !isset($_SESSION['Password'])) {
	echo "Please provide Adobe ID and password!";
} else {
	$fp 				= new FPHelper($_SESSION['AdobeID'],$_SESSION['Password'],$_SESSION['APIKey'],$_SESSION['APISecret']);
	$folioName 			= isset($_POST["folioName"])?$_POST["folioName"]:'';
	$magazineTitle 		= isset($_POST["magazineTitle"])?$_POST["magazineTitle"]:'';
	$folio_number 		= isset($_POST["folioNumber"])?$_POST["folioNumber"]:'';
	$folioDescription 	= isset($_POST["folioDescription"])?$_POST["folioDescription"]:'';
	$publicationDate 	= isset($_POST["publicationDate"])?date('Y-m-d\TH:i:s',strtotime($_POST['publicationDate'])):'';
	$dimension 			= isset($_POST["dimension"])?explode(' x ', $_POST["dimension"]):'';
	$defaultAssetFormat = isset($_POST["defaultAssetFormat"])?$_POST["defaultAssetFormat"]:'';
	$defaultJPEGQuality = isset($_POST["defaultJPEGQuality"])?$_POST["defaultJPEGQuality"]:'';
	$bindingRight 		= isset($_POST["bindingRight"])?$_POST["bindingRight"]:'';
	$Locked 			= isset($_POST["Locked"])?$_POST["Locked"]:'';
	$folioIntent 		= isset($_POST["folioIntent"])?$_POST["folioIntent"]:'Both';
	$targetViewer 		= isset($_POST["targetViewer"])?$_POST["targetViewer"]:'20.1.1';
	$filters 			= isset($_POST["filters"])?$_POST["filters"]:'';
	$resolutionWidth 	= $dimension[0];
	$resolutionHeight 	= $dimension[1];
	$message 			= array();

	if ($folioIntent == 'Portrait')
		$folioIntent = 'PortraitOnly';
	else if ($folioIntent == 'Landscape')
		$folioIntent = 'LandscapeOnly';
	else if ($folioIntent == 'Always')
		$folioIntent = 'Both';

	if ($folioName && $magazineTitle && $folio_number && $resolutionWidth && $resolutionHeight) {
		$params = array(
			'folioName'			=> $folioName,
			'magazineTitle'		=> $magazineTitle,
			'folioNumber'		=> $folio_number,
			'folioDescription'	=> $folioDescription,
			'publicationDate'	=> $publicationDate,
			'resolutionWidth'	=> $resolutionWidth,
			'resolutionHeight'	=> $resolutionHeight,
			'targetViewer'		=> $targetViewer,
			'folioIntent'		=> $folioIntent,
			'Filters'			=> $filters
		);
		$response = $fp->create_folio($params);
		echo $response['folioID'];
	} else {
		echo ' - Folio Creation Failed: <br/>Missing one or more of the following: Folio name, Magazine title, Resolution Width & Height!';
	}
}
