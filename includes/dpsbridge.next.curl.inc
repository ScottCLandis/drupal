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
 * Custom class that handles the HTTPS request using PHP cURL.
 */
class DPSNextCurl {
  // global HTTPS request values
  private $curl;
  private $curl_http_code;
  private $curl_options;
  private $curl_response;
  private $curl_response_header;
  private $curl_response_body;
  private $curl_response_size;
  private $file_path;
  private $request_data;
  private $request_data_raw;
  private $request_method;
  private $request_header;
  private $request_url;
  private $verbose;

  /**
   * Curl class constructor.
   *
   * @param {String} $request_type - The request type (GET, DELETE, POST, PUT)
   * @param {String} $request_url - The request URL
   * @param {Array} $request_headers - The array of request header
   * @param {Array} $request_data - The array of request data
   * @param {Boolean} $isFile - Specifies if the $request_data is a filepath
   */
  public function __construct($request_type,
                              $request_url,
                              $request_headers = null,
                              $request_data = null,
                              $fileType = 'json') {
    if ($request_data !== null) {
        switch ($fileType) {
            case 'file':
                $this->file_path = $request_data;
                break;
            case 'json':
                $this->request_data_raw = $request_data;
                $this->request_data = json_encode($request_data);
                break;
            case 'xml':
            default:
                $this->request_data_raw = $request_data;
                $this->request_data = $request_data;
                break;
        }
    }
    $this->request_header = $request_headers;
    $this->request_method = $request_type;
    $this->request_url = $request_url;
    $this->_setOptions();
  }

 /**
   * This method will return the access token from the API response.
   * @return {String} $access_token - The access token
   */
  public function getAccessToken() {
    $data = $this->curl_response_body;
    $access_token = null;
    if (isset($data['access_token']))
      $access_token = $data['access_token'];
    return $access_token;
  }

  /**
   * This method will return the refresh token from the API response.
   * @return {String} $refresh_token - The refresh token
   */
  public function getRefreshToken() {
    $data = $this->curl_response_body;
    $refresh_token = null;
    if (isset($data['refresh_token']))
      $refresh_token = $data['refresh_token'];
    return $refresh_token;
  }

  /**
   * Initialize and execute the HTTPS request.
   */
  public function exec() {
    // initialize the cURL
    $this->curl = curl_init();
    // initialize cURL parameters
    curl_setopt_array($this->curl, $this->curl_options);
    // execute cURL request
    $this->curl_response = curl_exec($this->curl);
    // get the response HTTP code
    $this->curl_http_code = curl_getinfo($this->curl, CURLINFO_HTTP_CODE);
    // get the response header size
    $this->curl_response_size = curl_getinfo($this->curl, CURLINFO_HEADER_SIZE);
    // call helper to parse & store the response header and body
    $this->_formatResponse();
    // close cURL request
    curl_close($this->curl);
    return $this;
  }

  /**
   * Get the full request and response parameters.
   * @return {Array} $full_response
   * @example $full_response = array(
   *              'request-header'    => {Array},
   *              'response-body'     => {Array},
   *              'response-code'     => {String},
   *              'response-header'   => {Array},
   *              'response-verbose'  => {String}
   *          )
   */
  public function getResponse() {
    $full_response = array(
        'request-url' => $this->request_url,
        'request-header' => $this->_getRequestHeader(),
        'response-body' => $this->curl_response_body,
        'response-code' => $this->curl_http_code,
        'response-header' => $this->curl_response_header,
        'response-verbose' => !rewind($this->verbose) . htmlspecialchars(stream_get_contents($this->verbose))
    );
    return $full_response;
  }

  /**
   * Helper.
   * Format and separate the response header and body:
   * 1. if response body is JSON, decode and store it as JSON
   * 2. if response body is XML, store it as XML
   */
  private function _formatResponse() {
    // store the response header
    $this->curl_response_header = substr($this->curl_response, 0, $this->curl_response_size);
    $response_body = substr($this->curl_response, $this->curl_response_size);
    if (strpos($response_body, '<?xml') === 0) // stores response body as XML
      $this->curl_response_body = new SimpleXMLElement($response_body);
    else // store response body as JSON
      $this->curl_response_body = json_decode($response_body, true);
  }

  /**
   * Helper.
   * Get the API request header in its RAW format.
   * @return {Array} $curl_options - The API request header
   */
  public function _getRequestHeader() {
    $curl_options = array(
      'CURLOPT_CUSTOMREQUEST' => $this->request_method,
      'CURLOPT_HTTPHEADER' => $this->request_header
    );
    if ($this->request_data_raw !== null)
       $curl_options['CURLOPT_POSTFIELDS'] = $this->request_data_raw;
    else if ($this->file_path !== null)
       $curl_options['CURLOPT_INFILE'] = $this->file_path;
    return $curl_options;
  }

  /**
   * Helper.
   * Set the HTTPS request options.
   */
  private function _setOptions() {
    $this->verbose = fopen('php://temp', 'rw+');
    $this->curl_options[CURLOPT_CUSTOMREQUEST] = $this->request_method;
    $this->curl_options[CURLOPT_HEADER] = true;
    $this->curl_options[CURLOPT_RETURNTRANSFER] = true;
    $this->curl_options[CURLOPT_STDERR] = $this->verbose;
    $this->curl_options[CURLOPT_URL] = $this->request_url;
    $this->curl_options[CURLOPT_USERAGENT] = $_SERVER['HTTP_USER_AGENT'];
    $this->curl_options[CURLOPT_VERBOSE] = true;
    // set the request header, optional
    if ($this->request_header !== null) {
      $this->curl_options[CURLOPT_HTTPHEADER] = $this->request_header;
    }
    // set the request data
    if ($this->request_data !== null) { // append JSON data
      $this->curl_options[CURLOPT_POSTFIELDS] = $this->request_data;
    } else if ($this->file_path !== null) { // append file data
      $this->curl_options[CURLOPT_INFILE] = fopen($this->file_path, 'r');
      $this->curl_options[CURLOPT_INFILESIZE] = filesize($this->file_path);
      $this->curl_options[CURLOPT_UPLOAD] = true;
    }
  }
}