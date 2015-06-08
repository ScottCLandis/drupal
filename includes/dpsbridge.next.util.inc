<?php
/**
 * @file
 * Contains utility methods related with DPSNext API.
 */

/**
 * Contains utility methods related with DPSNext API.
 */
class DPSNextUtil {
  /**
   * Custom date formatter. 
   */
  public static function format_date_custom($date) {
    $has_time = explode(' ', $date);
    $hour = 0;
    $minute = 0;
    $second = 0;

    if (count($has_time) === 2) {
      $date_array = explode('/', $has_time[0]);
      $time_array = explode(':', $has_time[1]);
      if (count($time_array) === 3) {
        $hour = $time_array[0];
        $minute = $time_array[1];
        $second = $time_array[2];
      } else {
        return '';
      }
    } else {
      $date_array = explode('/', $date);
    }

    if (count($date_array) === 3) {
      $month = $date_array[0];
      $day = $date_array[1];
      $year = $date_array[2];
    } else {
      return '';
    }

    return mktime($hour, $minute, $second, $month, $day, $year) * 1000;
  }
  /**
   * Generates a random number of a given length.
   */
  public static function generate_id($length) {
    for ($i = 0, $id = ''; $i < $length; $i++)
      $id .= mt_rand(0, 9);
    return $id;
  }
  /**
   * Generates a GUID.
   */
  public static function generate_uuid() {
    $low = self::generate_id(8);
    $mid = self::generate_id(4);
    $high = self::generate_id(4);
    $seq = self::generate_id(4);
    $node = self::generate_id(12);
    return $low . '-' . $mid . '-' . $high . '-' . $seq . '-' . $node;
  }
  /**
   * Given an article title, get file path of the .article file.
   */
  public static function get_article_file_path($article_title) {
    $dir = strstr(realpath(__FILE__), '/sites', TRUE);
    $file_path = 'folio/' . dpsbridge_helper_format_title($article_title) . '.article';
    $file_path = empty($file_path) ? NULL : $dir . '/sites/default/files' . '/dpsbridge/' . $file_path;
    return $file_path;
  }
  /**
   * Get neceesary request parameters
   */
  public static function getRequestParams($variables) {
    // generate an array with global credentials values
    $client_id =  variable_get(DPSNextConfig::VARIABLE_CLIENT_ID, DPSNextConfig::CLIENT_ID);
    $client_secret = variable_get(DPSNextConfig::VARIABLE_CLIENT_SECRET, DPSNextConfig::CLIENT_SECRET);
    $device_id = isset($variables['device_token']) ? $variables['device_token'] : self::getDeviceId();
    $device_token = isset($variables['device_token']) ? $variables['device_token'] : self::getDeviceToken();
    $access_token = isset($variables['access_token']) ? $variables['access_token'] : self::getAccessToken();
    
    $credentials = array(
      'access_token' => isset($access_token) ? $access_token : NULL,
      'client_id' => isset($client_id) ? $client_id : NULL,
      'client_secret' => isset($client_secret) ? $client_secret : NULL,
      'device_id' => isset($device_id) ? $device_id : NULL,
      'device_token' => isset($device_token) ? $device_token : NULL
    );
    
    // generate an array with global endpoints values
    $authentication_endpoint = DPSNextConfig::AUTHENTICATION_ENDPOINT;
    $authorization_endpoint = DPSNextConfig::AUTHORIZATION_ENDPOINT;
    $ingestion_endpoint = DPSNextConfig::INJESTION_ENDPOINT;
    $producer_endpoint = DPSNextConfig::PRODUCER_ENDPOINT;
    $endpoints = array(
      'authentication_endpoint' => isset($authentication_endpoint) ?
        $authentication_endpoint : NULL,
      'authorization_endpoint' => isset($authorization_endpoint) ?
        $authorization_endpoint : NULL,
      'ingestion_endpoint' => isset($ingestion_endpoint) ?
        $ingestion_endpoint : NULL,
      'producer_endpoint' => isset($producer_endpoint) ?
        $producer_endpoint : NULL
    );

    // generate an array with global parameters values
    $parameters = array(
      'client_request_id' => isset($variables['client_request_id']) ? $variables['client_request_id'] : NULL,
      'client_session_id' => isset($variables['client_session_id']) ? $variables['client_session_id'] : NULL,
      'client_upload_id' => isset($variables['client_upload_id']) ? $variables['client_upload_id'] : NULL,
      'client_version' => DPSNextConfig::CLIENT_VERSION,
      'publication_guid' => isset($variables['publication_guid']) ? $variables['publication_guid'] : NULL,
      'publication_path' => isset($variables['publication_path']) ? $variables['publication_path'] : NULL
    );
    return array(
      'credentials' => $credentials,
      'endpoints' => $endpoints,
      'parameters' => $parameters,
    );
    
  }
  /**
   * Get device id locally from Drupal variable.
   */
  public static function getDeviceId() {
    $device_id_variable = variable_get(DPSNextConfig::VARIABLE_DEVICE_ID, '');
    return $device_id_variable;
  }
  /**
   * Save device id locally as Drupal variable.
   * @param {String} $value - the value to be saved. 
   */
  public static function saveDeviceId($value = NULL) {
    $name = DPSNextConfig::VARIABLE_DEVICE_ID;
    $value = isset($value) ? $value : '';
    variable_set($name, $value);
  }
  /**
   * Get device token locally from Drupal variable.
   */
  public static function getDeviceToken() {
    $device_token_variable = variable_get(DPSNextConfig::VARIABLE_DEVICE_TOKEN);
    if (!isset($device_token_variable)) {
      self::saveDeviceToken();
      return DPSNextConfig::DEVICE_TOKEN_DEFAULT;
    }
    return $device_token_variable;
  }
  /**
   * Save device token locally as Drupal variable.
   * @param {String} $value - the value to be saved. 
   */
  public static function saveDeviceToken($value = NULL) {
    $name = DPSNextConfig::VARIABLE_DEVICE_TOKEN;
    $value = isset($value) ? $value : DPSNextConfig::DEVICE_TOKEN_DEFAULT;
    variable_set($name, $value);
  }
  /**
   * Get access token locally.
   */
  public static function getAccessToken() {
    $name = DPSNextConfig::SESSION_ACCESS_TOKEN;
    return isset($_SESSION[$name]) ? $_SESSION[$name] : NULL;
  }
  /**
   * Save access token locally.
   * @param {String} $value - the value to be saved. 
   */
  public static function saveAccessToken($value = NULL) {
    $name = DPSNextConfig::SESSION_ACCESS_TOKEN;
    $_SESSION[$name] = $value;
  }
}