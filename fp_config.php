<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_settings.php';

class DPSBridgeFPConfig extends DPSBridgeSettings {
  public $response = array();
  public $version = "1.01";

  /**
   * Create new object and initialise the variables.
   *
   * Call this method to get singleton.
   *
   * @return object
   *   UserFactory
   */
  public static function Instance() {
    static $inst = NULL;
    if ($inst === NULL) {
      $inst = new DPSBridgeFPConfig();
    }
    return $inst;
  }

  /**
   * Protected constructor so others can not violate the singleton.
   */
  protected function __construct() {
    session_start();

    $preconfigured = FALSE;
    $cconfig = isset($_SESSION['dpsbridge_config']) ? $_SESSION['dpsbridge_config'] : array();
    if ($cconfig || !isset($cconfig['mode'])) {
      $preconfigured = FALSE;
    }
    else {
      $preconfigured = TRUE;
    }

    parent::__construct('config', $cconfig);

    if (!$preconfigured) {
      $this->mode = 'debug';
      $this->server_family = 'production';

      if ($this->mode == 'release') {
        $this->proxy = '';
        $this->use_ssl = TRUE;
        $this->require_ssl = TRUE;
      }
      else {
        $this->use_ssl = TRUE;
        // Proxy '127.0.0.1';!
        $this->proxy = '';
        $this->require_ssl = FALSE;
      }

      if ($this->server_family == 'production') {
        $this->host = 'dpsapi2.digitalpublishing.acrobat.com';
        $this->distributionHost = 'origin.adobe-dcfs.com';
      }
      // Staging info.
      else {
        $this->host = 'dpsapi2-stage.digitalpublishing.acrobat.com';
        $this->distributionHost = 'origin-stage.adobe-dcfs.com';
      }

      $this->user_agent = 'PHP';
      $this->consumer_key = '';
      $this->consumer_secret = '';
      $this->user_email = '';
      $this->user_password = '';
      $this->timestamp = '';
      $this->oauth_signature_method = 'HAC-SHA256';
      $this->curl_ssl_verifyhost = '';
      $this->curl_ssl_verifypeer = '';
      $this->curl_ca_info = '';
      $this->curl_capath = '';
    }

    if ($this->require_ssl && $_SERVER["HTTPS"] != "on") {
      session_write_close();
      header("HTTP/1.1 301 Moved Permanently");
      header("Location: https://" . $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"]);
      exit();
    }
  }
}
