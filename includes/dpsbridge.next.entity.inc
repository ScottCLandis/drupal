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
 * Custom class that handles the article- and collection-level API request.
 * This class contains parameters and functions shared by Article and Collection.
 */
class DPSNextEntity extends DPSNextAuthentication {

  // article & collection values
  public $entity_name;
  public $entity_type;
  public $version;
  public $contentVersion;

  /**
   * Toggler,
   * Keep track of whether thumbnail or background image has been uploaded.
   * If a thumbnail image has been uploaded to collection,
   * then during the next update() call,
   * update the collection metadata with the reference to thumbnail/background.
   * @param {Array} $images_uploaded
   * @example {
   *      'thumbnail' => false
   *      'background' => false
   * }
   */
  private $images_uploaded;

  /**
   * Entity class constructor.
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
    $this->images_uploaded = array(
        'thumbnail' => false,
        'background' => false
    );
  }

  /**
   * Create (or update) the article or collection entity.
   * @param {Array} $param - The array of required & optional metadata parameters.
   * @example { // article
   *      'abstract' => '',
   *      'accessState' => '',
   *      'adCategory' => '',
   *      'adType' => '',
   *      'advertiser' => '',
   *      'author' => '',
   *      'authorUrl' => '',
   *      'category' => '',
   *      'department' => '',
   *      'hideFromBrowsePage' => '',
   *      'importance' => ''
   *      'internalKeywords' => '',
   *      'isAd' => '',
   *      'keywords' => '',
   *      'shortAbstract' => '',
   *      'shortTitle' => '',
   *      'title' => '',
   * }
   * @example { // collection
   *      'title' => '',
   *      'shortTitle' => '',
   *      'abstract' => '',
   *      'shortAbstract' => '',
   *      'productId' => '',
   *      'department' => '',
   *      'category' => '',
   *      'keywords' => '',
   *      'internalKeywords' => '',
   *      'importance' => '',
   *      'openTo' => '',
   *      'readingPosition' => ''
   * }
   * @param {Boolean} $isUpdate - The toggler to switch to an update function.
   */
  public function create($param = null, $isUpdate = false) {
    $this->request_name = ($isUpdate) ? 'Update ' . $this->entity_type . ' entity' : 'Create ' . $this->entity_type . ' entity';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: application/json
    $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['json']);

    // set the request data for entity
    if ($this->entity_type === 'article') {
        $data = $this->_genArticleCreateData($param, $isUpdate);
    } else if ($this->entity_type === 'collection') {
        $data = $this->_genCollectionCreateData($param, $isUpdate);
    } else if ($this->entity_type === 'banner') {
        $data = $this->_genBannerCreateData($param, $isUpdate);
    }

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= $this->parameters['publication_path'];
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;
    $url .= ($isUpdate) ? ';version=' . $this->version : '';

    // call helper to initiate the cURL request
    $this->_request('PUT', $url, $headers, $data);
    $this->contentVersion = $this->_getContentVersion();
    $this->version = $this->_getVersion();

    return $this;
  }

  /**
   * Delete the article or collection entity.
   */
  public function delete() {
    $this->request_name = 'Delete ' . $this->entity_type . ' entity';

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
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;
    $url .= ';version=' . $this->version;

    // call helper to initiate the cURL request
    $this->_request('DELETE', $url, $headers);
    return $this;
  }

  /**
   * Get the article or collection metadata.
   * Store the following locally: version, contentVersion, response body.
   */
  public function getMetadata() {
    $this->request_name = 'Get ' . $this->entity_type . ' metadata';

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
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;

    // call helper to initiate the cURL request
    $this->_request('GET', $url, $headers);

    // store latest version and contentVersion
    $this->contentVersion = $this->_getContentVersion();
    $this->version = $this->_getVersion();
    return $this;
  }

  /**
   * Generate and return the HREF to the latest version of:
   *  1. article entity or
   *  2. collection entity
   * @return {String} $entity_href - The reference link to the latest entity.
   */
  public function getHref() {
      $entity_href  = $this->parameters['publication_path'];
      $entity_href .= '/' . $this->entity_type . '/' . $this->entity_name;
      $entity_href .= ';version=' . $this->version;
      return $entity_href;
  }

  /**
   * Get a list of articles, collections, or both articles and collections.
   * @param {String} $option - Specific which list of entities to get.
   * @example $option = 'article'; // get a list of articles
   * @example $option = 'collection'; // get a list of collections
   * @example $option = 'all'; // get a list of articles and collections
   */
  public function getList($option = '') {
    $this->request_name = 'Get ' . $option . ' list';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: application/json
    $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['json']);

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= $this->parameters['publication_path'];

    // use the internal value if available
    $option = ($option) ? $option : $this->entitle_type;
    // set request URL with query parameters
    switch ($option) {
        case 'article':
            $url .= '/article';
            break;
        case 'collection':
            $url .= '/collection';
            break;
        case 'all':
        default:
            $url .= '/collection?q=entityType==collection,entityType==article';
            break;
    }

    // call helper to initiate the cURL request
    $this->_request('GET', $url, $headers);
    return $this;
  }

  /**
   * Get the metadata of a publication.
   */
  public function getPublication() {
      $this->request_name = 'Get publication metadata';

      // set request header:
      // [H] X-DPS-Client-Id: {client-id}
      // [H] X-DPS-Client-Version: {double-dot style notation}
      // [H] X-DPS-Client-Request-Id: {UUID}
      // [H] X-DPS-Client-Session-Id: {UUID}
      // [H] X-DPS-Api-Key: {base-64 encoded}
      // [H] Authorization: bearer {base-64 encoded}
      // [H] Accept: application/json
      // [H] Content-Type: application/json
      $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['json']);

      // set request URL
      $url  = $this->endpoints['producer_endpoint'];
      $url .= '/publication/' . $this->parameters['publication_guid'];

      // call helper to initiate the cURL request
      $this->_request('GET', $url, $headers);
      return $this;
  }
  
  /**
   * Get the status for the following: publishing & ingestion
   */
  public function getStatus() {
    $this->request_name = 'Get ' . $this->entity_type . ' status';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: application/json
    $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['json']);

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= '/status/' . $this->parameters['publication_guid'];
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;

    // call helper to initiate the cURL request
    $this->_request('GET', $url, $headers);
    return $this;
  }

  /**
   * Publish the entity.
   * @param {Date} $publish_schedule_date - The scheduled date to unpublish.
   * @param {String} $workflow_type - The publish type
   * @example $workflow_type = 'publish' // set to publish the entity
   * @example $workflow_type = 'unpublish' // set to unpublish the entity
   * @example $workflow_type = 'layout'
   * @example $workflow_type = 'publishPublication'
   */
  public function publish($publish_schedule_date = 0, $workflow_type = 'publish') {
    $this->request_name = 'Publish ' . $this->entity_type . ' entity';

    // check ingestion status for articles only
    if ($this->entity_type === 'article' &&
        $this->_checkUploadStatus() === false) {
        // TODO: display error when attempting to publish an article where the ingestion is still in process or not started at all
        $this->printCurlData('Error: no article content or article ingestion is not finished yet');
        exit();
    }
        
    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: application/json
    $headers = $this->_setHeaders($this->mimetypes['json'], $this->mimetypes['json']);

    // set request data
    $entry  = $this->parameters['publication_path'];
    $entry .= '/' . $this->entity_type . '/' . $this->entity_name;
    $entry .= ';version=' . $this->version;
    $data = array(
        'workflowType' => $workflow_type,
        'entities' => array($entry),
        'scheduled' => $this->_formatDate($publish_schedule_date)
    );

    // set request URL
    $url = $this->endpoints['producer_endpoint'] . '/job';

    // call helper to initiate the cURL request
    $this->_request('POST', $url, $headers, $data);
    return $this;
  }

  /**
   * Seal the article or collection entity.
   * This is necessary after an image file upload, to commit the changes.
   */
  public function seal() {
    $this->request_name = 'Seal ' . $this->entity_type . ' entity';

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    $headers = $this->_setHeaders($this->mimetypes['json'], null, $this->parameters['client_upload_id']);

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= $this->parameters['publication_path'];
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;
    $url .= ';version=' . $this->version;
    $url .= '/contents';

    // call helper to initiate the cURL request
    $this->_request('PUT', $url, $headers);
    $this->contentVersion = $this->_getContentVersion();
    $this->version = $this->_getVersion();
    return $this;
  }

  /**
   * Set the entity name for an article or collection.
   * This must be called prior to any CRUD job on an entity.
   * @param {String} $entity_name - The name of the entity (a.k.a entity ID)
   * @return {Object} $this - The reference to this object class.
   */
  public function setEntityName($entity_name) {
    $this->entity_name = $entity_name;
    return $this;
  }
    
  /**
   * Rely the create() method to update the article or collection entity.
   * The differences between create() and update() are the following:
   * - providing the entity version number as a matrix parameters
   * - the following becomes optional: importance, openTo, readingPosition
   * - use the response data from get_metadata() as the starting point
   * @param {Array} $param - The array of required & optional metadata parameters.
   * @example { // article
   *      'abstract' => '',
   *      'accessState' => '',
   *      'adCategory' => '',
   *      'adType' => '',
   *      'advertiser' => '',
   *      'author' => '',
   *      'authorUrl' => '',
   *      'category' => '',
   *      'department' => '',
   *      'hideFromBrowsePage' => '',
   *      'importance' => ''
   *      'internalKeywords' => '',
   *      'isAd' => '',
   *      'keywords' => '',
   *      'shortAbstract' => '',
   *      'shortTitle' => '',
   *      'title' => '',
   * }
   * @example { // collection
   *      'title' => '',
   *      'shortTitle' => '',
   *      'abstract' => '',
   *      'shortAbstract' => '',
   *      'productId' => '',
   *      'department' => '',
   *      'category' => '',
   *      'keywords' => '',
   *      'internalKeywords' => '',
   *      'importance' => '',
   *      'openTo' => '',
   *      'readingPosition' => ''
   * }
   */
  public function update($param = null) {
    return $this->create($param, true);
  }

  /**
   * Upload a thumbnail image to the article or collection entity.
   * Must perform an update() with reference to this image,
   * follow by a seal() to commit the image change.
   * @param {String} $image_path - The relative path to the image file.
   * @example $image_path = '../image/creativecloud.jpg';
   * @param {String} $image_type - The image type (background or thumbnail)
   * @example $image_type = 'background'; // uploading an background image
   * @example $image_type = 'thumbnail'; // uploading an thumbnail image
   */
  public function uploadImage($image_path, $image_type = 'thumbnail') {
    $this->request_name = 'Upload image to ' . $this->entity_type;
    
    // Get file extension
    $file_ext = end(explode('.', $image_path));
    if (($file_ext == 'jpg') || ($file_ext == 'jpeg')) {
      $mimetype_image = $this->mimetypes['jpeg'];
    }
    elseif ($file_ext == 'png') {
      $mimetype_image = $this->mimetypes['png'];
    }
    elseif ($file_ext == 'gif') {
      $mimetype_image = $this->mimetypes['gif'];
    }
    else {
      // Not supported image type
      throw new DPSNextException(t('Error: ') . $this->request_name . '-Image type is not supported ');
    }

    // set request header:
    // [H] X-DPS-Client-Id: {client-id}
    // [H] X-DPS-Client-Version: {double-dot style notation}
    // [H] X-DPS-Client-Request-Id: {UUID}
    // [H] X-DPS-Client-Session-Id: {UUID}
    // [H] X-DPS-Upload-Id: {UUID}
    // [H] X-DPS-Api-Key: {base-64 encoded}
    // [H] Authorization: bearer {base-64 encoded}
    // [H] Accept: application/json
    // [H] Content-Type: image/jpg
    $headers = $this->_setHeaders($this->mimetypes['json'], $mimetype_image, $this->parameters['client_upload_id']);

    // get image full path
    $file_path = realpath($image_path);
    

    // set request URL
    $url  = $this->endpoints['producer_endpoint'];
    $url .= $this->parameters['publication_path'];
    $url .= '/' . $this->entity_type . '/' . $this->entity_name;
    $url .= '/contents;contentVersion=' . $this->contentVersion;
    $url .= '/images/' . $image_type;

    // call helper to initiate the cURL request
    $this->_request('PUT', $url, $headers, $file_path, 'file');
    $this->images_uploaded[$image_type] = true;
    return $this;
  }

  /**
   * Rely the publish() method to unpublish the article or collection entity.
   * The differences between publish and unpublish is the workflow_type.
   * @param {Date} $publish_schedule_date - The scheduled date to unpublish.
   */
  function unpublish($publish_schedule_date = 0) {
    return $this->publish($publish_schedule_date, 'unpublish');
  }

  /**
   * Helper.
   * Check the article upload (ingestion) status.
   * Only when an article upload is successful, can user publish().
   * @return {Object} $this - The reference to this object class.
   */
  protected function _checkUploadStatus() {
    $isUploaded = false;
    if (isset($this->curl_data['response-body'])) {
        $data = $this->curl_data['response-body'];
        foreach ($data as $status) {
            if (isset($status['aspect']) &&
                $status['aspect'] === 'ingestion' &&
                $status['numerator'] === $status['denominator']) {
                $isUploaded = true;
            }
        }
    }
    return $isUploaded;
  }
    
  /**
   * Helper.
   * Generate the request data for creating an article entity.
   * @param {Array} $param - The array of optional metadata parameters.
   * @example {
   *      'abstract' => '',
   *      'accessState' => '',
   *      'adCategory' => '',
   *      'adType' => '',
   *      'advertiser' => '',
   *      'author' => '',
   *      'authorUrl' => '',
   *      'category' => '',
   *      'department' => '',
   *      'hideFromBrowsePage' => '',
   *      'importance' => ''
   *      'internalKeywords' => '',
   *      'isAd' => '',
   *      'keywords' => '',
   *      'shortAbstract' => '',
   *      'shortTitle' => '',
   *      'title' => '',
   * }
   * @param {Boolean} $isUpdate - The toggler to switch to an update function.
   */
  protected function _genArticleCreateData($param, $isUpdate) {
    $data = ($isUpdate) ? $this->curl_data['response-body'] : array(
        // accessState default to 'protected'
        'accessState' => isset($param['accessState']) ? $param['accessState'] : 'free',
        // adType default to 'static'
        'adType' => isset($param['adType']) ? $param['adType'] : 'static',
        'entityName' => $this->entity_name,
        'entityType' => 'article',
        // importance default to 'normal'
        'importance' => isset($param['importance']) ? $param['importance'] : 'normal',
    );

    // set optional metadata
    if (isset($param['abstract']) && ($param['abstract'] != ''))
        $data['abstract'] = $param['abstract'];
    if (isset($param['category']))
        $data['category'] = $param['category'];
    if (isset($param['shortTitle']) && ($param['shortTitle'] != ''))
        $data['shortTitle'] = $param['shortTitle'];
    if (isset($param['title']))
        $data['title'] = $param['title'];
    if (isset($param['shortAbstract']) && ($param['shortAbstract'] != ''))
        $data['shortAbstract'] = $param['shortAbstract'];
    if (isset($param['author']))
        $data['author'] = $param['author'];
    if (isset($param['authorUrl']))
        $data['authorUrl'] = $param['authorUrl'];
    if (isset($param['keywords']))
        $data['keywords'] = $param['keywords'];
    if (isset($param['internalKeywords']))
        $data['internalKeywords'] = $param['internalKeywords'];
    if (isset($param['hideFromBrowsePage']))
        $data['hideFromBrowsePage'] = $param['hideFromBrowsePage'];
    if (isset($param['department']))
        $data['department'] = $param['department'];
    if (isset($param['isAd']))
        $data['isAd'] = $param['isAd'];
    if (isset($param['adCategory']))
        $data['adCategory'] = $param['adCategory'];
    if (isset($param['advertiser']))
        $data['advertiser'] = $param['advertiser'];

    // set optional social media data
    if (isset($param['socialShareUrl']))
        $data['socialShareUrl'] = $param['socialShareUrl'];
    if (isset($param['articleText']))
        $data['articleText'] = $param['articleText'];
    if (isset($param['url']))
        $data['url'] = $param['url'];
    
    // set optional metadata for update() only
    if ($isUpdate) {
        if (isset($param['accessState']))
            $data['accessState'] = $param['accessState'];
        if (isset($param['adType']))
            $data['adType'] = $param['adType'];
        if (isset($param['importance']))
            $data['importance'] = $param['importance'];
        if ($this->images_uploaded['thumbnail'])
            $data['_links']['thumbnail']['href'] = 'contents/images/thumbnail';
    }
    return $data;
  }

  /**
   * Helper.
   * Generate the request data for creating an banner entity.
   * @param {Array} $param - The array of optional metadata parameters.
   * @example {
   *      'abstract' => '',
   *      'accessState' => '',
   *      'adCategory' => '',
   *      'adType' => '',
   *      'advertiser' => '',
   *      'author' => '',
   *      'authorUrl' => '',
   *      'category' => '',
   *      'department' => '',
   *      'hideFromBrowsePage' => '',
   *      'importance' => ''
   *      'internalKeywords' => '',
   *      'isAd' => '',
   *      'keywords' => '',
   *      'shortAbstract' => '',
   *      'shortTitle' => '',
   *      'title' => '',
   * }
   * @param {Boolean} $isUpdate - The toggler to switch to an update function.
   */
  protected function _genBannerCreateData($param, $isUpdate) {
    $data = ($isUpdate) ? $this->curl_data['response-body'] : array(
        // adType default to 'static'
        'adType' => isset($param['adType']) ? $param['adType'] : 'static',
        // bannerTapAction default to 'none'
        'bannerTapAction' => isset($param['bannerTapAction']) ? $param['bannerTapAction'] : 'none',
        'entityName' => $this->entity_name,
        'entityType' => 'banner',
        // importance default to 'normal'
        'importance' => isset($param['importance']) ? $param['importance'] : 'normal',
    );

    // set optional metadata
    if (isset($param['abstract']))
        $data['abstract'] = $param['abstract'];
    if (isset($param['category']))
        $data['category'] = $param['category'];
    if (isset($param['shortTitle']))
        $data['shortTitle'] = $param['shortTitle'];
    if (isset($param['title']))
        $data['title'] = $param['title'];
    if (isset($param['shortAbstract']))
        $data['shortAbstract'] = $param['shortAbstract'];
    if (isset($param['keywords']))
        $data['keywords'] = $param['keywords'];
    if (isset($param['internalKeywords']))
        $data['internalKeywords'] = $param['internalKeywords'];
    if (isset($param['department']))
        $data['department'] = $param['department'];
    if (isset($param['isAd']))
        $data['isAd'] = $param['isAd'];
    if (isset($param['adCategory']))
        $data['adCategory'] = $param['adCategory'];
    if (isset($param['advertiser']))
        $data['advertiser'] = $param['advertiser'];

    // set optional social media data
    if (isset($param['socialShareUrl']))
        $data['socialShareUrl'] = $param['socialShareUrl'];
    if (isset($param['url']))
        $data['url'] = $param['url'];

    // set optional metadata for update() only
    if ($isUpdate) {
        if (isset($param['adType']))
            $data['adType'] = $param['adType'];
        if (isset($param['importance']))
            $data['importance'] = $param['importance'];
        if ($this->images_uploaded['thumbnail'])
            $data['_links']['thumbnail']['href'] = 'contents/images/thumbnail';
    }
    return $data;
  }
    
  /**
   * Helper.
   * This method will generate the request data for creating a collection entity.
   *
   * @param {Array} $param - The array of optional metadata parameters.
   * @example {
   *      'title' => '',
   *      'shortTitle' => '',
   *      'abstract' => '',
   *      'shortAbstract' => '',
   *      'productId' => '',
   *      'department' => '',
   *      'category' => '',
   *      'keywords' => '',
   *      'internalKeywords' => '',
   *      'importance' => '',
   *      'openTo' => '',
   *      'readingPosition' => ''
   * }
   * @param {Boolean} $isUpdate - The toggler to switch to an update function.
   */
  protected function _genCollectionCreateData($param, $isUpdate) {
    // set request data
    $data = ($isUpdate) ? $this->curl_data['response-body'] : array(
        'entityName' => $this->entity_name,
        'entityType' => 'collection',
        'importance' => isset($param['importance']) ? $param['importance'] : 'normal',
        'lateralNavigation' => isset($param['lateralNavigation']) ? $param['lateralNavigation'] : true,
        'openTo' => isset($param['openTo']) ? $param['openTo'] : 'contentView',
        'readingPosition' => isset($param['readingPosition']) ? $param['readingPosition'] : 'retain'
    );

    // set the request data during create(),
    // otherwise use the response data during update()
    if (isset($param['title']))
        $data['title'] = $param['title'];
    if (isset($param['shortTitle']) && ($param['shortTitle'] != ''))
        $data['shortTitle'] = $param['shortTitle'];
    if (isset($param['abstract']) && ($param['abstract'] != ''))
        $data['abstract'] = $param['abstract'];
    if (isset($param['shortAbstract']) && ($param['shortAbstract'] != ''))
        $data['shortAbstract'] = $param['shortAbstract'];
    if (isset($param['department']))
        $data['department'] = $param['department'];
    if (isset($param['category']))
        $data['category'] = $param['category'];
    if (isset($param['keywords']))
        $data['keywords'] = $param['keywords'];
    if (isset($param['internalKeywords']))
        $data['internalKeywords'] = $param['internalKeywords'];
    if (isset($param['productIds']))
        $data['productIds'] = $param['productIds'];

    // set optional metadata for update() only
    if ($isUpdate) {
        if (isset($param['importance']))
            $data['importance'] = $param['importance'];
        if (isset($param['openTo']))
            $data['openTo'] = $param['openTo'];
        if (isset($param['readingPosition']))
            $data['readingPosition'] = $param['readingPosition'];
        if ($this->images_uploaded['background'])
            $data['_links']['background']['href'] = 'contents/images/background';
        if ($this->images_uploaded['thumbnail'])
            $data['_links']['thumbnail']['href'] = 'contents/images/thumbnail';
        foreach($this->contentElements as $contentElement) {
            $data['_links']['contentElements'][] = array(
                'href' => $contentElement
            );
        }
    }
    return $data;
  }

  /**
   * Helper.
   * This method will return the entity content version from the API response.
   * @return {String} $content_version - The entity's content version
   */
  protected function _getContentVersion() {
    $data = $this->curl_data['response-body'];
    if (!isset($data['_links']) || !isset($data['_links']['contentUrl']))
        return null;
    $content_url = $data['_links']['contentUrl']['href'];
    $index = strrpos($content_url, '=');
    $content_version = substr($content_url, $index + 1, -1);
    return $content_version;
  }

  /**
   * Helper.
   * Get the entity version from the API response.
   * @return {String} $version_id - The entity's version
   */
  protected function _getVersion() {
    $data = $this->curl_data['response-body'];
    $version_id = null;
    if (isset($data['version']))
        $version_id = $data['version'];
    return $version_id;
  }
}