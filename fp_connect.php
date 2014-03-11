<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

  require_once dirname(__FILE__) . '/fp_config.php';
  session_cache_limiter('private');
  $cache_limiter = session_cache_limiter();
  session_cache_expire(30);
  $cache_expire = session_cache_expire();
  $config = FPConfig::Instance();
  require_once dirname(__FILE__) . '/fp_helper.php';

  $_SESSION['AdobeID'] = htmlspecialchars(isset($_POST["AdobeID"]) ? $_POST["AdobeID"] : '');
  $_SESSION['Password'] = htmlspecialchars(isset($_POST["Password"]) ? $_POST["Password"] : '');
  $_SESSION['APIKey'] = htmlspecialchars(isset($_POST["APIKey"]) ? $_POST["APIKey"] : '');
  $_SESSION['APISecret'] = htmlspecialchars(isset($_POST["APISecret"]) ? $_POST["APISecret"] : '');
  $folio_id = htmlspecialchars(isset($_POST['folioID']) ? $_POST['folioID'] : '');
  $isTest = htmlspecialchars(isset($_POST["Test"]) ? $_POST["Test"] : '');

  $fp = new FPHelper($_SESSION['AdobeID'],$_SESSION['Password'],$_SESSION['APIKey'],$_SESSION['APISecret']);
  $config->fp = $fp;
  $config = FPConfig::Instance();
  $fp = $config->fp;
  $config->fpError = 'ok';
  $config->fulfillmentError = 'ok';

  // Create session
  if(!isset($_SESSION['ticket'])) {
    $session = $fp->create_session();
    $distributionAPI = $fp->create_distribution_session();
    $distributionInfo = new SimpleXMLElement($distributionAPI);
    $_SESSION['DistributionID'] = (string) $distributionInfo->accountId;

    if($session['status'] != 'ok') {
      if ($session['status'] === 'InvalidLogin') {
        echo "[Login Failed] The Folio Producer's username and password do not match.";
      }
      else if ($session['status'] === 'BadSig' || $session['status'] === 'InvalidMessageContent') {
        echo "[Authentication Failed] The Folio Producer's API Key and/or Secret provided was not valid.";
      }
      else {
        echo "[". $session['status'] . "] " . $session['errorDetail'];
      }
      $config->fpError = $session['status'];
    }
    else if ($folio_id) {
      $articles = $fp->articles($folio_id);
      print_r(json_encode($articles));
    }
    else {
      echo "ok";
    }
  }
