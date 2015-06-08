<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';
$config = DPSBridgeFPConfig::Instance();
require_once dirname(__FILE__) . '/fp_helper.php';

if (isset($_SESSION['dpsbridge_AdobeID'])) {
  $fp = new DPSBridgeFPHelper($_SESSION['dpsbridge_AdobeID'], $_SESSION['dpsbridge_Password'], $_SESSION['dpsbridge_APIKey'], $_SESSION['dpsbridge_APISecret']);
  $delete_session = $fp->deleteSession();
}

$_SESSION = array();

if (ini_get("session.use_cookies")) {
  $params = session_get_cookie_params();
  setcookie(session_name(), '', time() - 42000,
    $params["path"], $params["domain"],
    $params["secure"], $params["httponly"]
  );
}

session_destroy();

echo "[Session] You have been successfully logged out of Folio Producer.";
