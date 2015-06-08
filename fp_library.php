<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';

/**
 * FolioProducer Library Wrapper.
 */
class DPSBridgeFPLibrary {
  public $response = array();

  /**
   * Create new object and initialise the variables.
   */
  public function __construct($config_in) {
    $this->params = array();
    $this->headers = array();
    $this->config = DPSBridgeFPConfig::Instance();
    $this->config->mergePublic($config_in);
  }

  /**
   * Generate nonce and store in config.
   */
  protected function createNonce() {
    $sequence = array_merge(range(0, 9), range('A', 'Z'), range('a', 'z'));
    $length = count($sequence);
    shuffle($sequence);
    $prefix = $this->config->timestamp;
    $this->config->nonce = md5(substr($prefix . implode('', $sequence), 0, $length));
  }

  /**
   * Get the timestamp and set in config.
   */
  protected function createTimestamp() {
    if ($this->config->timestamp == '') {
      $this->config->timestamp = round(microtime(TRUE));
    }
  }

  /**
   * Generate URL for webservice.
   */
  protected function createURL($server, $suffix = '') {
    if (strpos($server, 'http') === FALSE) {
      $url = ($this->config->use_ssl) ? 'https' : 'http';
      return $url . '://' . $server . '/webservices/' . $suffix;
    }
    return $server . '/webservices/' . $suffix;
  }

  /**
   * Generate URL for webservice.
   */
  protected function createDistributionURL($server, $suffix = '') {
    if (strpos($server, 'http') === FALSE) {
      $url = ($this->config->use_ssl) ? 'https' : 'http';
      return $url . '://' . $server . '/' . $suffix;
    }
    return $server . '/webservices/' . $suffix;
  }

  /**
   * Message to be encrypted for oauth.
   */
  protected function oAuthMessage() {
    $url = urlencode($this->createURL($this->config->host, 'sessions'));
    $params = '&oauth_consumer_key%3D' . $this->config->consumer_key . '%26oauth_signature_method%3DHMAC-SHA256' . '%26oauth_timestamp%3D' . $this->config->timestamp;
    return 'POST&' . $url . $params;
  }

  /**
   * Generate the oauth signature.
   */
  protected function oAuthSignature() {
    $message = $this->oAuthMessage();
    $hash = hash_hmac('sha256', $message, $this->config->consumer_secret . '&', FALSE);
    $bytes = pack('H*', $hash);
    $base = base64_encode($bytes);
    return urlencode($base);
  }

  /**
   * Set the properties for curl request.
   *
   * @param string $method
   *   GET, POST, DELETE.
   * @param string $url
   *   API.
   * @param {array} $params
   *   Request parameters.
   * @param string $filepath
   *   Path to file if uploading.
   * @param bool $is_download
   *   Set if using the download server.
   */
  public function request($method, $url, $params = array(), $filepath = NULL, $is_download = FALSE, $is_distribution = FALSE) {
    if (strstr($filepath, 'folio/') || strstr($filepath, 'html/') || strstr($filepath, 'styles/')) {
      $dir = strstr(realpath(__FILE__), '/sites', TRUE);
      $filepath = empty($filepath) ? NULL : $dir . '/sites/default/files' . '/dpsbridge/' . $filepath;
    }

    $this->method = $method;
    $this->headers = array(
      'Content-Type: application/json; charset=utf-8',
    );
    $ready_for_request = FALSE;

    if ($is_distribution && isset($_SESSION['dpsbridge_distributionTicket'])) {
      $ready_for_request = TRUE;
    }
    if (!$is_distribution && isset($_SESSION['dpsbridge_ticket'])) {
      $ready_for_request = TRUE;
    }
    // If no oAuth then set it up!
    if (!$ready_for_request) {
      // Echo 'athentication required...'!
      if ($is_distribution) {
        // Echo 'authenticating against distribution api....'!
        $this->url = $this->createDistributionURL($this->config->distributionHost, $url);

        $credentials = array(
          'email'  => $this->config->user_email,
          'password'  => $this->config->user_password,
        );

        $this->params = json_encode($credentials);
        $this->oauth = $this->curl(TRUE);

        return $this->oauth;
      }
      else {
        // Authenticating against fp api....';!
        $this->createTimestamp();
        $this->createNonce();
        $this->sig = $this->oAuthSignature();
        // Debug message print_r("signature=[".$this->sig."]\n");.
        $this->url = $this->createURL($this->config->host, $url);

        $credentials = array(
          'email'  => $this->config->user_email,
          'password'  => $this->config->user_password,
        );

        if (!isset($credentials['email']) || !isset($credentials['password'])) {
          throw new Exception("Email and password are required");
        }
        $this->params = json_encode($credentials);
        $this->headers[] = 'Authorization: OAuth oauth_consumer_key="' . $this->config->consumer_key . '", oauth_timestamp="' . $this->config->timestamp . '", oauth_signature_method="HMAC-SHA256", oauth_signature="' . $this->sig . '"';
        $this->oauth = $this->curl(FALSE);
        $_SESSION['dpsbridge_ticket'] = $this->oauth['ticket'];
        $_SESSION['dpsbridge_server'] = $this->oauth['server'];
        $_SESSION['dpsbridge_downloadTicket'] = $this->oauth['downloadTicket'];
        $_SESSION['dpsbridge_downloadServer'] = $this->oauth['downloadServer'];
        return $this->oauth;
      }
    }
    // OAuth present regular request.
    else {
      $this->params = json_encode($params);
      if ($is_distribution) {
        $ticket = $_SESSION['dpsbridge_distributionTicket'];
        $server = $this->config->distributionHost;
      }
      elseif ($is_download) {
        $ticket = $_SESSION['dpsbridge_downloadTicket'];
        $server = $_SESSION['dpsbridge_downloadServer'];
      }
      else {
        $ticket = $_SESSION['dpsbridge_ticket'];
        $server = $_SESSION['dpsbridge_server'];
      }
      $this->url = $this->createURL($server, $url);
      $this->headers[] = 'Authorization: AdobeAuth ticket="' . $ticket  . '"';

      if (isset($filepath)) {
        // Remove content-type.
        unset($this->headers[0]);
        $this->file = $filepath;
        $this->fileUpload();
      }
      $response = $this->curl(FALSE);
      if (isset($response['ticket'])) {
        $_SESSION['dpsbridge_ticket'] = $response['ticket'];
      }
      if (isset($response['downloadTicket'])) {
        $_SESSION['dpsbridge_downloadTicket'] = $response['downloadTicket'];
      }
      return $response;
    }
  }

  /**
   * Run the curl request using the values set in request.
   *
   * @return array
   *   Curl output
   */
  public function curl($is_distribution = FALSE) {
    $ch = curl_init();

    curl_setopt_array($ch, array(
      CURLOPT_URL => $this->url,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_USERAGENT => $this->config->user_agent,
      CURLOPT_PROXY => $this->config->proxy ? $this->config->proxy : '',
      CURLOPT_HTTPPROXYTUNNEL => $this->config->proxy ? TRUE : FALSE,
      CURLOPT_PROXYPORT => $this->config->proxy ? '8888' : '',
      CURLOPT_PROXYTYPE => $this->config->proxy ? 'HTTP' : '',
      CURLOPT_HTTPHEADER => $this->headers,

      CURLOPT_SSL_VERIFYHOST => $this->config->curl_ssl_verifyhost,
      CURLOPT_SSL_VERIFYPEER => $this->config->curl_ssl_verifypeer,
    ));

    if ($this->config->curl_capath !== FALSE) {
      curl_setopt($ch, CURLOPT_CAPATH, $this->config->curl_capath);
    }
    if ($this->config->curl_cainfo !== FALSE) {
      curl_setopt($ch, CURLOPT_CAINFO, $this->config->curl_cainfo);
    }

    switch ($this->method) {
      case 'GET':
        break;

      case 'POST':
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $this->params);
        break;

      default:
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $this->method);
        break;
    }
    $execute = curl_exec($ch);
    curl_close($ch);

    if ($is_distribution) {
      return $execute;
    }
    else {
      return json_decode($execute, TRUE);
    }
  }

  /**
   * Set multipart request.
   */
  protected function fileUpload() {
    $file = $this->file;
    if (!file_exists($file)) {
      throw new Exception("File does not exist:" . $file);
    }

    $handle = fopen($file, 'rb');
    fseek($handle, 0);
    $binary = fread($handle, filesize($file));
    fclose($handle);

    $separator = md5(microtime());
    $this->headers[] = 'Content-Type: multipart/form-data; boundary=' . $separator;

    // TODO:
    // parameters
    // HTML
    $eol = "\r\n";
    $data = '';
    $data .= '--' . $separator . $eol;
    $data .= 'Content-Disposition: form-data; name=""; filename="' . $file . '"' . $eol;
    $data .= 'Content-Type: ' . $eol;
    $data .= 'Content-Transfer-Encoding: binary' . $eol . $eol;
    $data .= $binary . $eol;
    $data .= '--' . $separator . "--" . $eol;

    $this->params = $data;
  }
}
