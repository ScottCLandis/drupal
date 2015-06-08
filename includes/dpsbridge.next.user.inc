<?php
/* ************************************************************************
 * NOTICE: This work is licensed under a Creative Commons Attribution
 * -Noncommercial-Share Alike 3.0 Unported License. Permissions beyond
 * the scope of this license, pertaining to the examples of code included
 * within this work are available at Adobe Commercial Rights.
 *
 * NOTE: You may reuse these files as you want, but they are not officially
 * supported as part of the product. Technical support will not be able to
 * answer questions about these files.
 *
 * Related Links:
 *  - http://creativecommons.org/licenses/by-nc-sa/3.0/
 *  - http://www.adobe.com/communities/guidelines/ccplus/commercialcode_plus_permission.html
 **************************************************************************/

/**
 * Custom class that handles the user-level credentials & parameters.
 */
class DPSNextUser {

  // references to the global parameters
  public $credentials;
  public $parameters;
  public $endpoints;
  // global mimetype parameters
  public $mimetype;
  // global cURL parameter
  public $curl_data;
  // global class parameter
  public $request_name;

  /**
   * User class constructor.
   * Call the respective helper class to set the global values.
   * @param {Array} $credentials - The array of global credentials.
   * @example {
   *      'client_id' => '',
   *      'client_secret' => '',
   * }
   * @param {Array} $parameters - The array of global parameters.
   * @example {
   *      'access_token' => '',
   *      'client_request_id' => '',
   *      'client_session_id' => '',
   *      'client_version' => '',
   *      'publication_path' => ''
   * }
   * @param {Array} $endpoints - The array of global endpoints.
   * @example {
   *      'authentication_endpoint' => '',
   *      'authorization_endpoint' => '',
   *      'ingestion_endpoint' => '',
   *      'producer_endpoint' => ''
   * }
   */
  public function __construct(&$credentials, &$parameters, &$endpoints) {
      $this->credentials = &$credentials;
      $this->parameters = &$parameters;
      $this->endpoints = &$endpoints;
      $this->_setMinetypes();
      $this->_validateObj();
  }

  /**
   * Output the API response in a human-readable format.
   * @param {String} $request_name - The name of the API request
   * @param {Boolean} $show_input - Toggler, whether to show the user input
   * @param {Boolean} $show_debug - Toggler, whether to show the debug value
   */
  public function printCurlData($request_name = null, $show_input = true, $show_debug = false) {
          echo '<pre>';
          echo '<h2>';
          echo ($request_name) ? $request_name : $this->request_name;
          echo '</h2>';
      if ($show_input) {
          echo '<h3>Request URL:</h3>';
          print_r($this->curl_data['request-url']);
          echo '<h3>Request Data:</h3>';
          print_r($this->curl_data['request-header']);
          echo '<h3>Response Header:</h3>';
          print_r($this->curl_data['response-header']);
          echo '<h3>Response Body:</h3>';
      }
          print_r($this->curl_data['response-body']);
      if ($show_debug) {
          echo '<h3>Verbose information:</h3>';
          echo $this->curl_data['response-verbose'];
      }
          echo '</pre>';
      return $this;
  }

  /**
   * Helper.
   * Convert the given date (mm/dd/yyyy hh:mm:ss) into EPOCH.
   * @param {date} $date - The time in the following: mm/dd/yyyy hh:mm:ss
   * @return {date} $epoch - The time in EPOCH if successful, 0 otherwise.
   */
  function _formatDate($date) {

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
          } else { // Wrong date + time format: mm/dd/yyyy hh:mm:ss;
              return 0;
          }
      } else {
          $date_array = explode('/', $date);
      }

      if (count($date_array) === 3) {
          $month = $date_array[0];
          $day = $date_array[1];
          $year = $date_array[2];
      } else { // Wrong date format: mm/dd/yyyy;
          return 0;
      }

      return mktime($hour, $minute, $second, $month, $day, $year) * 1000;
  }

  /**
   * Helper.
   * Generate an alphanumeric (a-f) value of a given length.
   * @param {int} $length - The length of the alphanumeric value
   * @return {string} $id - The alphanumeric value of the given length
   */
  public function _generateId($length) {
      // generate a set of alphanumeric (a-f) value of the given length
      for ($i = 0, $id = ''; $i < $length; $i++) {
          if (mt_rand(0, 1)) // generate a random letter
              $id .= chr(mt_rand(0, 5) + 97);
          else // generate a random number
              $id .= mt_rand(0, 9);
      }
      return $id;
  }

  /**
   * Helper.
   * Generate the UUID (alphanumeric).
   * @return {string} $uuid - The UUID of the following length: 8-4-4-4-12
   */
  public function _generateUuid() {
      $low = $this->_generateId(8);
      $mid = $this->_generateId(4);
      $high = $this->_generateId(4);
      $seq = $this->_generateId(4);
      $node = $this->_generateId(12);
      $uuid = $low . '-' . $mid . '-' . $high . '-' . $seq . '-' . $node;
      return $uuid;
  }

  /**
   * Helper.
   * Pretty-print the missing parameters.
   */
  public function _msgMissingValues($values) {
      echo '<pre>';
      echo '<h2>Error: Missing Values</h2>';
      print_r($values);
      echo '</pre>';
      exit();
  }

  /**
   * Helper.
   * Create a new Curl class per request.
   * The Curl class is set to auto-execute after initialization.
   * If there's an error, quit and print the response.
   * If the error is a 401 (UNAUTHORIZED):
   *  - attempt to get a new access token
   *  - retry request (once) with new access token
   * @param {String} $type - The request type (GET, DELETE, POST, PUT)
   * @param {String} $url - The request URL
   * @param {Array} $headers - The array of request header
   * @param {Array} $data - The array of request data
   * @param {Boolean} $isRetry - Toggler, whether this request is retry or not.
   */
  public function _request($type,
                           $url,
                           $headers = null,
                           $data = null,
                           $fileType = 'json',
                           $isRetry = false) {
      // initialize and execute cURL request
      $curl = new DPSNextCurl($type, $url, $headers, $data, $fileType);

      // get response HTTP code
      $this->curl_data = $curl->exec()->getResponse();

      // check the HTTP code for errors
      switch ($this->curl_data['response-code']) {
          case 200: // Ok
          case 201: // Created
          case 202: // Accepted
          case 204: // No Content
              break;
          case 401: // Unauthorized
              if ($isRetry) // prevents multiple retries
                  return;
              $current_request_name = $this->request_name;
              // attempts to get new access token
              $this->getToken();
              // attempts to resend request using the new access token
              $headers[5] = 'Authorization: bearer ' .
                  $this->credentials['access_token'];
              $this->request_name = $current_request_name;
              $this->_request($type, $url, $headers, $data, $fileType, true);
              break;
          case 400: // Bad Request
          case 404: // Not Found
          case 409: // (Version) Conflict
          case 415: // Unsupported Media Type
          case 500: // Internal Server Error
          default: // Other types of error
            throw new DPSNextException(t('Error: ') . $this->request_name , $this->curl_data['response-code']);
      }
      return $this;
  }

  /**
   * Helper.
   * Generate the request header per request.
   * @param {String} $accept_type - The request accept type, optional
   * @param {String} $content_type - The request content type, optional
   * @param {String} $client_upload_id - The client upload ID, optional
   */
  public function _setHeaders($accept_type = null, $content_type = null, $client_upload_id = null) {
      // set the required reqquest header by PRODUCER and INGESTION server
      $headers = array(
          'X-DPS-Client-Id: ' . $this->credentials['client_id'],
          'X-DPS-Client-Version: ' . $this->parameters['client_version'],
          'X-DPS-Client-Request-Id: ' . $this->parameters['client_request_id'],
          'X-DPS-Client-Session-Id: ' . $this->parameters['client_session_id'],
          'X-DPS-Api-Key: ' . $this->credentials['client_id'],
          'Authorization: bearer ' . $this->credentials['access_token'],
          'Connection: Close'
      );

      // set the optional reqquest header
      if ($client_upload_id)
          $headers[] = 'X-DPS-Upload-Id: ' . $client_upload_id;
      if ($accept_type)
          $headers[] = 'Accept: ' . $accept_type;
      if ($content_type)
          $headers[] = 'Content-Type: ' . $content_type;

      return $headers;
  }

  /**
   * Helper.
   * Generate and store the global minetypes array locally.
   */
  public function _setMinetypes() {
      $this->mimetypes['all'] = 'application/json, text/plain, */*';
      $this->mimetypes['entity'] = 'application/vnd.adobe.entity+json';
      $this->mimetypes['article'] = 'application/vnd.adobe.article+zip';
      $this->mimetypes['jpeg'] = 'image/jpeg';
      $this->mimetypes['png'] = 'image/png';
      $this->mimetypes['gif'] = 'image/gif';
      $this->mimetypes['json'] = 'application/json';
      $this->mimetypes['urlencoded'] = 'application/x-www-form-urlencoded';
  }

  /**
   * Helper.
   * Validate the given global parameters.
   * Depending on the class type, the required parameters will vary.
   */
  public function _validateObj() {
      $missing_params = array();

      // auto-generate UUID for the following if not provided
      if (!isset($this->parameters['client_request_id']))
          $this->parameters['client_request_id'] = $this->_generateUuid();
      if (!isset($this->parameters['client_session_id']))
          $this->parameters['client_session_id'] = $this->_generateUuid();
      if (!isset($this->parameters['client_upload_id']))
          $this->parameters['client_upload_id'] = $this->_generateUuid();

      // all APIs require the client ID
      if (!isset($this->credentials['client_id']) ||
              $this->credentials['client_id'] === '')
          $missing_params[] = '$client_id';

      // almost all APIs require the client version
      if (!isset($this->parameters['client_version']) ||
              $this->parameters['client_version'] === '')
          $missing_params[] = '$client_version';

      // check for required parameters by API
      switch (get_class($this)) {
          case 'DPSNextArticle': // Article class object
          case 'DPSNextCollection': // Collection class object
              if (!isset($this->endpoints['ingestion_endpoint']) ||
                      $this->endpoints['ingestion_endpoint'] === '')
                  $missing_params[] = '$ingestion_endpoint';
              /*if (!isset($this->parameters['publication_path']) ||
                      $this->parameters['publication_path'] === '')
                  $missing_params[] = '$publication_path';*/
          case 'DPSNextEntity': // Entity class object
              if (!isset($this->endpoints['producer_endpoint']) ||
                      $this->endpoints['producer_endpoint'] === '')
                  $missing_params[] = '$producer_endpoint';
          case 'DPSNextAuthentication': // Authentication class object
              if (!isset($this->credentials['client_secret']) ||
                      $this->credentials['client_secret'] === '')
                  $missing_params[] = '$client_secret';
              if (!isset($this->credentials['device_token']) ||
                      $this->credentials['device_token'] === '')
                  $missing_params[] = '$device_token';
              if (!isset($this->credentials['device_id']) ||
                      $this->credentials['device_id'] === '')
                  $missing_params[] = '$device_id';
              if (!isset($this->endpoints['authentication_endpoint']) ||
                      $this->endpoints['authentication_endpoint'] === '')
                  $missing_params[] = '$authentication_endpoint';
              if (!isset($this->endpoints['authorization_endpoint']) ||
                      $this->endpoints['authorization_endpoint'] === '')
                  $missing_params[] = '$authorization_endpoint';
                default:
                    break;
      }

      // if missing value(s), call helper to handle this
      if (count($missing_params) > 0)
          $this->_msgMissingValues($missing_params);
  }
}
