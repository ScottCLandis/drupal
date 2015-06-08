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
 * Custom class that handles the authentication & authorization API request.
 */
class DPSNextAuthentication extends DPSNextUser {

  /**
   * Authentication class constructor.
   * The three arrays are used by the parent class User.
   * @param {Array} $credentials - The array of global credentials.
   * @example {
   *      'client_id' => '',
   *      'client_secret' => ''
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
      parent::__construct($credentials, $parameters, $endpoints);
  }

  /**
   * Get the list of user permissions.
   * If an access token isn't provided, this method will look for one locally.
   * @param {String} $access_token - The base-64 encoded access token
   */
  public function getPermissions($access_token = null) {
      $this->request_name = 'Get User Permissions';
      $access_token = isset($access_token) ?
          $access_token : $this->credentials['access_token'];

      // set request header:
      // [H] X-DPS-Client-Version: {double-dot style notation}
      // [H] X-DPS-Client-Request-Id: {UUID}
      // [H] X-DPS-Client-Session-Id: {UUID}
      // [H] X-DPS-Api-Key: {base-64 encoded}
      // [H] Authorization: bearer {base-64 encoded}
      // [H] Accept: application/json
      $headers = array(
          'Accept: ' . $this->mimetypes['json'],
          'X-DPS-Client-Version: ' . $this->parameters['client_version'],
          'X-DPS-Client-Request-Id: ' . $this->parameters['client_request_id'],
          'X-DPS-Client-Session-Id: ' . $this->parameters['client_session_id'],
          'X-DPS-Api-Key: ' . $this->credentials['client_id'],
          'Authorization: bearer ' . $this->credentials['access_token']
      );

      // set request URL
      $url = $this->endpoints['authorization_endpoint'] . '/permissions';

      // call helper to initiate the cURL request
      $this->_request('GET', $url, $headers);

      // store the publication GUID, this is used for DEMO purposes only!
      $this->parameters['publication_path'] = '/publication/' . $this->_getPublicationGuid();
      return $this;
  }

  /**
   * Get the access token.
   * @param {String} $device_token - The base-64 encoded device token
   * @param {String} $grant_type - The grant type, optional parameters
   * @example $grant_type = 'device_token';
   * @param {String} $scope - The scope value, optional parameters
   * @example $scope = 'AdobeID,openid';
   */
  public function getToken($device_token = null,
                             $device_id = null,
                             $grant_type = 'device',
                             $scope = 'openid') {
      $this->request_name = 'Get Access Token';
      $device_token = isset($device_token) ? $device_token : $this->credentials['device_token'];
      $device_id = isset($device_id) ? $device_id : $this->credentials['device_id'];

      // set request header:
      // [H] Accept: application/json
      // [H] Content-Type: application/x-www-form-urlencoded
      $headers = array(
          'Accept: ' . $this->mimetypes['json'],
          'Content-Type: ' . $this->mimetypes['urlencoded']
      );

      // set request URL
      $url  = $this->endpoints['authentication_endpoint'];
      $url .= '/ims/token/v1';
      $url .= '?grant_type=' . $grant_type;
      $url .= '&client_id=' . urlencode($this->credentials['client_id']);
      $url .= '&client_secret=' . urlencode($this->credentials['client_secret']);
      $url .= '&scope=' . urlencode($scope);
      $url .= '&device_token=' . $device_token;
      $url .= '&device_id=' . $device_id;

      // call helper to initiate the cURL request
      $this->_request('POST', $url, $headers);

      // store the access token locally
      $this->credentials['access_token'] = $this->_getAccessToken();
      // $this->credentials['refresh_token'] = $this->_getRefreshToken();
      DPSNextUtil::saveAccessToken($this->credentials['access_token']);
      // DPSNextUtil::saveRefreshToken($this->credentials['refresh_token']);
      return $this;
  }

  /**
   * Helper.
   * Get the access token from the API response.
   * @return {String} $access_token - The access token
   */
  public function _getAccessToken() {
      $data = $this->curl_data['response-body'];
      $access_token = null;
      if (isset($data['access_token']))
          $access_token = $data['access_token'];
      return $access_token;
  }
  /**
   * Helper.
   * Get the refresh token from the API response.
   * @return {String} $access_token - The access token
   */
  public function _getRefreshToken() {
      $data = $this->curl_data['response-body'];
      $refresh_token = null;
      if (isset($data['refresh_token']))
          $refresh_token = $data['refresh_token'];
      return $refresh_token;
  }
  /**
   * Helper.
   * Get the publication GUID from the API response.
   * Used for demo purposes only!
   * @return {String} $publication_path - The publication GUID
   */
  public function _getPublicationGuid() {
      $data = $this->curl_data['response-body'];
      $publication_path = null;
      if (isset($data['masters']) &&
          isset($data['masters'][0]['publications']) &&
          isset($data['masters'][0]['publications'][0]['id']))
          $publication_path = $data['masters'][0]['publications'][0]['id'];
      return $publication_path;
  }

  /**
   * This method will check if user is authenticated.
   * 
   * @return boolean
   */
  public function isAuthenticated() {
    if (isset($this->getToken()->credentials['access_token'])) {
      return TRUE;
    }
    return FALSE;
  }
}
