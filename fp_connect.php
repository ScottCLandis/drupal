<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';
session_cache_limiter('private');
$cache_limiter = session_cache_limiter();
session_cache_expire(30);
$cache_expire = session_cache_expire();
$config = DPSBridgeFPConfig::Instance();
require_once dirname(__FILE__) . '/fp_helper.php';

$_SESSION['dpsbridge_AdobeID'] = htmlspecialchars(isset($_POST["AdobeID"]) ? $_POST["AdobeID"] : '');
$_SESSION['dpsbridge_Password'] = htmlspecialchars(isset($_POST["Password"]) ? $_POST["Password"] : '');
$_SESSION['dpsbridge_APIKey'] = htmlspecialchars(isset($_POST["APIKey"]) ? $_POST["APIKey"] : '');
$_SESSION['dpsbridge_APISecret'] = htmlspecialchars(isset($_POST["APISecret"]) ? $_POST["APISecret"] : '');
$folio_id = htmlspecialchars(isset($_POST['folioID']) ? $_POST['folioID'] : '');
$is_test = htmlspecialchars(isset($_POST["Test"]) ? $_POST["Test"] : '');

$fp = new DPSBridgeFPHelper($_SESSION['dpsbridge_AdobeID'], $_SESSION['dpsbridge_Password'], $_SESSION['dpsbridge_APIKey'], $_SESSION['dpsbridge_APISecret']);
$config->fp = $fp;
$config = DPSBridgeFPConfig::Instance();
$fp = $config->fp;
$config->fpError = 'ok';
$config->fulfillmentError = 'ok';

// Create session.
if (!isset($_SESSION['dpsbridge_ticket'])) {
  $session = $fp->createSession();
  $distribution_api = $fp->createDistributionSession();
  $distribution_info = new SimpleXMLElement($distribution_api);
  $_SESSION['dpsbridge_DistributionID'] = (string) $distribution_info->accountId;

  if ($session['status'] != 'ok') {
    if ($session['status'] === 'InvalidLogin') {
      echo "[Login Failed] The Folio Producer's username and password do not match.";
    }
    elseif ($session['status'] === 'BadSig' || $session['status'] === 'InvalidMessageContent') {
      echo "[Authentication Failed] The Folio Producer's API Key and/or Secret provided was not valid.";
    }
    else {
      echo "[" . $session['status'] . "] " . $session['errorDetail'];
    }
    $config->fpError = $session['status'];
  }
  elseif ($folio_id) {
    $articles = $fp->articles($folio_id);
    print_r(json_encode($articles));
  }
  else {
    echo "ok";
  }
}
