<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';
$config = FPConfig::Instance();
require_once dirname(__FILE__) . '/fp_helper.php';

if(isset($_SESSION['AdobeID'])) {
	$fp = new FPHelper($_SESSION['AdobeID'],$_SESSION['Password'],$_SESSION['APIKey'],$_SESSION['APISecret']);
	$deleteSession = $fp->delete_session();
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
