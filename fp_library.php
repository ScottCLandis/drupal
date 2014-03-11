<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_config.php';

/**
* FolioProducer Library Wrapper
* R25
* @dbeaton
*/
class FPLibrary
{
  var $response = array();

  /**
   * Create new object and initialise the variables
   * @param array $config stores parameters
   */
  public function __construct($config_in) {
    $this->params = array();
    $this->headers = array();
    $this->config = FPConfig::Instance();
    
    $this->config->mergePublic($config_in);
  }
  /**
   * Generate nonce and store in config
   */
  private function create_nonce() {
    $sequence = array_merge(range(0,9), range('A','Z'), range('a','z'));
    $length = count($sequence);
    shuffle($sequence);
    $prefix = $this->config->timestamp;
    $this->config->nonce = md5(substr($prefix . implode('',$sequence),0,$length ));
  }
  /**
   * Get the timestamp and set in config
   */
  private function create_timestamp() {
    if($this->config->timestamp == '') {
      $this->config->timestamp = round(microtime(true));
    }
  }
  /**
   * Generate URL for webservice
   */
  private function create_url($server, $suffix = '') {
    if (strpos($server, 'http') === false) {
      $url = ($this->config->use_ssl) ? 'https' : 'http';
      return $url . '://' . $server . '/webservices/' . $suffix;
    }
    return $server . '/webservices/' . $suffix;
  }
  private function create_distributionurl($server ,$suffix = '') {
    if (strpos($server,'http') === false) {
      $url = ($this->config->use_ssl) ? 'https' : 'http';
      return $url . '://' . $server . '/'. $suffix;
    }
    return $server . '/webservices/' . $suffix;
  }
  /**
   * Message to be encrypted for oauth
   */
  private function oauth_message() {
    $url = urlencode($this->create_url($this->config->host, 'sessions'));
    $params = '&oauth_consumer_key%3D' . $this->config->consumer_key .
         '%26oauth_signature_method%3DHMAC-SHA256' .
        '%26oauth_timestamp%3D' .  $this->config->timestamp;
    return 'POST&' . $url . $params;  
  }
  /**
   * Generate the oauth signature
   */
  private function oauth_signature() {
    $message = $this->oauth_message();
    $hash = hash_hmac('sha256', $message, $this->config->consumer_secret . '&', false);
    $bytes = pack('H*', $hash);
    $base = base64_encode($bytes);
    return urlencode($base);
  }
  /**
   * Set the properties for curl request
   * @param  [type]  $method      GET, POST, DELETE
   * @param  [type]  $url         API
   * @param  array   $params      request parameters
   * @param  [type]  $filepath    path to file if uploading
   * @param  boolean $is_download set if using the download server
   */
  public function request($method, $url, $params=array(), $filepath=null, $is_download=false, $is_distribution=false) {
    $this->method = $method;
    $this->headers = array(
      'Content-Type: application/json; charset=utf-8',
    );
    $ready_for_request = false;
    
    if($is_distribution && isset($_SESSION['distributionTicket'])) {
      $ready_for_request = true;
    }
    if(!$is_distribution && isset($_SESSION['ticket'])) {
      $ready_for_request = true;
    }
    // If no oAuth then set it up
    if (!$ready_for_request) { 
    //  echo 'athentication required...';
      
      if($is_distribution) {
      //  echo 'authenticating against distribution api....';
        $this->url = $this->create_distributionurl($this->config->distributionHost, $url);
        
//          echo '</pre>';
            
        $credentials = array(
          'email'  => $this->config->user_email,
          'password'  => $this->config->user_password
        );

        $this->params = json_encode($credentials);
        $this->oauth = $this->curl(true);
/*        echo '<pre>';
        print_r($this->oauth);
        echo '</pre>';
*/
        return $this->oauth;
      } 
      else {
//        echo 'authenticating against fp api....';
        $this->create_timestamp();
        $this->create_nonce();
        $this->sig = $this->oauth_signature();
        //print_r("signature=[".$this->sig."]\n");
        $this->url = $this->create_url($this->config->host, $url);
        
        $credentials = array(
          'email'  => $this->config->user_email,
          'password'  => $this->config->user_password,
        );
        
        if (!isset($credentials['email']) || !isset($credentials['password'] )) {
          throw new Exception("Email and password are required");
        }
        $this->params = json_encode($credentials);
        $this->headers[] = 'Authorization: OAuth oauth_consumer_key="' . $this->config->consumer_key . '", oauth_timestamp="' . $this->config->timestamp . '", oauth_signature_method="HMAC-SHA256", oauth_signature="' . $this->sig . '"';
  
      //  echo 'calling curl now...';
        $this->oauth = $this->curl(false);
      //  echo '<pre>';
      //  print_r($this->oauth);
      //  echo '</pre>';
        $_SESSION['ticket'] = $this->oauth['ticket'];
      //  echo 'ticket-->'.$_SESSION['ticket'];
        $_SESSION['server'] = $this->oauth['server'];
        $_SESSION['downloadTicket'] = $this->oauth['downloadTicket'];
        $_SESSION['downloadServer'] = $this->oauth['downloadServer'];
        return $this->oauth;
      }
    }
    else {
      
      //echo 'regular request';
      $this->params = json_encode($params);

      if($is_distribution) {
      //  echo '[distribution]';
        $ticket= $_SESSION['distributionTicket'];
        $server= $this->config->distributionHost;
      }
      else if ($is_download) {
      //  echo '[download]';
        $ticket = $_SESSION['downloadTicket'];
        $server = $_SESSION['downloadServer'];
      }
      else {
      //  echo '[FP API]';
        $ticket = $_SESSION['ticket'];
        $server = $_SESSION['server'];
      }
    //  echo 'server='.$server;
      $this->url = $this->create_url($server,$url);
  
      $this->headers[] = 'Authorization: AdobeAuth ticket="' . $ticket  . '"';
      
      if (isset($filepath)) {
        unset($this->headers[0]); // remove content-type
        $this->file = $filepath;
        $this->file_upload();
      }
      $response = $this->curl(false);
      if(isset($response['ticket'])) { 
        $_SESSION['ticket'] = $response['ticket'];
      }
      if(isset($response['downloadTicket'])) { 
        $_SESSION['downloadTicket'] = $response['downloadTicket'];
      }
      return $response;
    }
  }
  /**
   * Run the curl request using the values set in request()
   * @return array Curl output
   */
  public function curl($is_distribution=false) {
    $ch = curl_init();

    //print_r($this->url);
    curl_setopt_array($ch, array(
      CURLOPT_URL => $this->url,
      CURLOPT_RETURNTRANSFER => true,  
      CURLOPT_USERAGENT => $this->config->user_agent,
      CURLOPT_PROXY => $this->config->proxy ? $this->config->proxy : '',
      CURLOPT_HTTPPROXYTUNNEL => $this->config->proxy ? true : false,
      CURLOPT_PROXYPORT => $this->config->proxy ? '8888' : '',
      CURLOPT_PROXYTYPE => $this->config->proxy ? 'HTTP' : '',
      CURLOPT_HTTPHEADER => $this->headers,
      
      CURLOPT_SSL_VERIFYHOST => $this->config->curl_ssl_verifyhost ,
      CURLOPT_SSL_VERIFYPEER => $this->config->curl_ssl_verifypeer,       
    ));

    if ($this->config->curl_capath !== false) {
      curl_setopt($ch, CURLOPT_CAPATH, $this->config->curl_capath );
    }
    if ($this->config->curl_cainfo !== false) {
      curl_setopt($ch, CURLOPT_CAINFO, $this->config->curl_cainfo );
    }

    switch ($this->method) {
      case 'GET' :

        break;
      case 'POST' :
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $this->params);  
        break;
      default :
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, $this->method );
        break;
    }
    $execute = curl_exec($ch);
    curl_close($ch);
    
    if($is_distribution) {
      return $execute;
    }
    else {
      return json_decode( $execute ,true );
    }
  }  
  /**
   * Set multipart request
   */
  private function file_upload() {
    $file = $this->file;
    if (!file_exists($file)) {
      throw new Exception("File does not exist");
    }  

    $handle = fopen($file, 'rb'); 
    $buffer = ''; 
    fseek($handle, 0); 
    $binary = fread($handle, filesize($file)); 
    fclose($handle);

    $separator = md5(microtime());
    $this->headers[] = 'Content-Type: multipart/form-data; boundary=' . $separator;

    // TODO: 
    //   parameters 
    //   HTML
    $eol = "\r\n";
    $data = '';
    $data .=  '--' . $separator . $eol;
    $data .= 'Content-Disposition: form-data; name=""; filename="' . $file . '"' . $eol;
    $data .= 'Content-Type: ' . $eol;
    $data .= 'Content-Transfer-Encoding: binary' . $eol . $eol;
    $data .= $binary . $eol;
    $data .= '--' . $separator . "--" . $eol ;

    $this->params = $data;
  }
}
