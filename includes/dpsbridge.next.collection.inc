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
 * Custom class that handles the collection-level API request.
 * This class contains collection-specific parameters and functions.
 */
class DPSNextCollection extends DPSNextEntity {

  /**
   * Toggler.
   * Keep track of collections or articles added to this collection.
   * If the list is not empty, then during the next update() call,
   * update the collection metadata with the reference to the added entity.
   * @param {Array} $contentElements
   * @example {
   *      'href/link/to/article/entity',
   *      'href/link/to/collection/entity',
   * }
   */
  public $contentElements;

  /**
   * Collection class constructor.
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
        $this->entity_type = 'collection';
        $this->contentElements = array();
  }

  /**
   * Add the article or collection entity to this collection.
   * Given the entity (article or collection) object,
   * get the latest entity version and then generate the entity link (HREF).
   * @param {Object} $entity - The article or collection class object
   */
  public function add($entity) {
    // use the getHref() method to pull the entity link (href) reference
    $this->contentElements[] = $entity->getMetadata()->getHref();
    return $this;
  }
}
