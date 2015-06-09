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
 * Custom class that handles the article-level API request.
 * This class contains article-specific parameters and functions.
 */
class DPSNextArticle extends DPSNextEntity {

  /**
   * Article class constructor.
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
    $this->entity_type = 'article';
  }

  /**
   * This method will get the article manifest,
   * used to check the uploaded file content (.article zip, thumbnail image).
   */
  public function getManifest() {
    $this->request_name = 'Get article manifest';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    $headers = $this->_setHeaders($this->mimetypes['json']);

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= $this->parameters['publication_path'];
    $url .= '/article/' . $this->entity_name;
    $url .= '/contents;contentVersion=' . $this->contentVersion . '/';

    // call helper to initiate the cURL request
    $this->_request('GET', $url, $headers);
    return $this;
  }

  /**
   * This method will upload an .article zip file to the ingestion server.
   * DO NOT seal() after this method,
   * the ingestion server automatically does this.
   *
   * @param {String} $article_file - The relative path to the article zip file.
   * @example $article_file = '../samples/example.article';
   */
  public function uploadArticle($article_file) {
    $this->request_name = 'Upload article file';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: application/vnd.adobe.article+zip
    $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['article']);

    // get image full path
    $file_path = realpath($article_file);

    // set request URL
    $url  = $this->endpoints['ingestion_endpoint'];
    $url .= $this->parameters['publication_path'];
    $url .= '/article/' . $this->entity_name;
    $url .= ';version=' . $this->version;
    $url .= '/contents/folio';

    // call helper to initiate the cURL request
    $this->_request('PUT', $url, $headers, $file_path, 'file');
    // store latest version and contentVersion
    //$this->contentVersion = $this->_getContentVersion();
    //$this->version = $this->_getVersion();
    return $this;
  }
}
