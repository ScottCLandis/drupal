<?php
/**
 * DPSNextEntity class to define the properties of an entity
 */
class DPSNextEntity {
  const ENTITY_TYPE_ARTICLE = 'article';
  const ENTITY_TYPE_COLLECTION = 'collection';
  
  protected $entity_type;
  protected $entity_name;
  protected $entity_version_id;
  protected $entity_content_version;
  
  /**
   * The title of the article.
   * - the value must be UNIQUE
   * @param {string} $title
   * @example  'This is the article title';
   */
  protected $title;
  
  /**
   * The description of the entity.
   */
  protected $description;
  
  /**
   * The keywords associated with the entity.
   * - separate multiple keywords by a comma
   * @param {array} $keywords
   * @example ['development'];
   * @example ['development', 'test', 'api'];
   */
  protected $keywords;
  
  /**
   * The priority of the entity.
   * - use one of the following terms: low, normal, high.
   * @param {string} $importance
   */
  protected $importance = 'normal';
  
  /**
   * @var boolean Specifies if the entity is an ad or not
   */
  protected $is_ad = FALSE;
  
  
  public function __construct($variables = array(), $data_loader = NULL) {
 
		foreach ($variables as $property => $value) {
      $method_name = 'set_' . $property;
      
      // If property exists and the setter for that property exists
      // Call the setter to set property. 
      if (property_exists($this, $property) && method_exists($this, $method_name)) {
        $this->$method_name($value);
      }
    }
	}
  
  /**
   * Create an entity.
   * @param array $variables
   */
  public function create($variables = array()) {
    
  }
  /**
   * Get an entity.
   * @param array $variables
   */
  public function read($variables = array()) {
    
  }
  /**
   * Update an entity.
   * @param array $variables
   */
  public function update($variables = array()) {
    
  }
  /**
   * Delete and entity.
   * @param array $variables
   */
  public function delete($variables = array()) {
    
  }
  /**
   * Upload content to entity.
   * @param array $variables
   */
  public function upload($variables = array()) {
    
  }
  
  
  public function set_entity_name($value) {
    $this->entity_name = $value;
  }
  public function set_entity_version_id($value) {
    $this->entity_version_id = $value;
  }
  public function set_entity_content_version($value) {
    $this->entity_content_version = $value;
  }
  public function set_title($value) {
    $this->title = $value;
  }
  public function set_description($value) {
    $this->description = $value;
  }
  public function set_keywords($value) {
    $this->keywords = $value;
  }
  public function set_importance($value) {
    $this->importance = $value;
  }
  public function set_is_ad($value) {
    $this->is_ad = $value;
  }
  
  public function get_entity_type() {
    return $this->entity_type;
  }
  public function get_entity_name() {
    return $this->entity_name;
  }
  public function get_entity_version_id() {
    return $this->entity_version_id;
  }
  public function get_entity_content_version() {
    return $this->entity_content_version;
  }
  public function get_title() {
    return $this->title;
  }
  public function get_description() {
    return $this->description;
  }
  public function get_keywords() {
    return $this->keywords;
  }
  public function get_importance() {
    return $this->importance;
  }
  public function get_is_ad() {
    return $this->is_ad;
  }
}

/**
 * DPSNextArticle class to define the properties of an article entity
 */
class DPSNextArticle extends DPSNextEntity {
  
  protected $entity_type = self::ENTITY_TYPE_ARTICLE;
  
  /**
   * @var string the article byline text
   * CURRENTLY NOT USED!
   */
  protected $byline;
  
  /**
   * @var string The article byline URL
   * CURRENTLY NOT USED!
   */
  protected $byline_url;
  
  /**
   * The access state of the article.
   * - use one of the following terms: free, metered, protected
   * @param {string} $access_state
   * @example $access_state = 'free'; // free article
   * @example $access_state = 'metered'; // metered article
   * @example $access_state = 'protected'; // protected article
   */
  protected $access_state = 'protected';
  
  /**
   * Specifies whether to show or hide the article from the browse page.
   * - set true to hide the article from the browse page, false otherwise
   * @param {boolean} $hide_from_browser_page
   * @example $hide_from_browser_page = TRUE; // hide from browse page
   * @example $hide_from_browser_page = FALSE; // do NOT hide from browse page
   */
  protected $hide_from_browser_page = FALSE;
  
  
  /**
   * Create an entity.
   * @param array $variables
   */
  public function create($variables = array()) {
    $client_request_id = DPSNextUtil::generate_uuid();
    $client_session_id = isset($client_session_id) ? $client_session_id : DPSNextUtil::generate_uuid();
    
    $headers = array(
      'accept_type' => DPSNextConfig::CONTENT_TYPE_JSON,
      'content_type' => DPSNextConfig::CONTENT_TYPE_JSON,
      'request_id' => $client_request_id,
      'session_id' => $client_session_id,
    );
    
    $data = $this->_load_metadata();
    $url = DPSNextConfig::JUPITER_ENDPOINT . '/publication/' . DPSNextConfig::$publication_id . '/' . $this->get_entity_type() . '/' . $this->get_entity_name();
    $curl = new DPSNext('PUT', $url, $headers, $data);
    $curl->exec();
    //$curl->output(TRUE, TRUE);
    
    $content_version_id_returned = $curl->get_content_version();
    if ($content_version_id_returned !== FALSE) {
      $_SESSION['dps_article_content_version'] = $content_version_id_returned;
    }
    $version_id_returned = $curl->get_version_id();
    if ($version_id_returned !== FALSE) {
      $this->set_entity_version_id($version_id_returned);
      //dpsbridge_helper_next_set_version($article_metadata['node']['nid'], $version_id_returned);
      return $version_id_returned;
    }
    return '';
    }
  /**
   * Get an entity.
   * @param array $variables
   */
  public function read($variables = array()) {
    
  }
  /**
   * Update an entity.
   * @param array $variables
   */
  public function update($variables = array()) {
    $client_request_id = DPSNextUtil::generate_uuid();
    $client_session_id = isset($client_session_id) ? $client_session_id : DPSNextUtil::generate_uuid();
    
    $article_version_id = $this->get_entity_version_id();
    
    $headers = array(
      'accept_type' => DPSNextConfig::CONTENT_TYPE_JSON,
      'content_type' => DPSNextConfig::CONTENT_TYPE_JSON,
      'request_id' => $client_request_id,
      'session_id' => $client_session_id,
    );
    
    $data = $this->_load_metadata();
    $url = DPSNextConfig::JUPITER_ENDPOINT . '/publication/' . DPSNextConfig::$publication_id 
           . '/' . $this->get_entity_type() . '/' . $this->get_entity_name() . ';version=' . $article_version_id;
    
    $curl = new DPSNext('PUT', $url, $headers, $data);
    $curl->exec();
    //$curl->output(TRUE, TRUE);
    
    $content_version_id_returned = $curl->get_content_version();
    if ($content_version_id_returned !== FALSE) {
      $_SESSION['dps_article_content_version'] = $content_version_id_returned;
    }
    $version_id_returned = $curl->get_version_id();
    if ($version_id_returned !== FALSE) {
      $this->set_entity_version_id($version_id_returned);
      //dpsbridge_helper_next_set_version($article_metadata['node']['nid'], $version_id_returned);
      return $version_id_returned;
    }
    return '';
  }
  /**
   * Delete and entity.
   * @param array $variables
   */
  public function delete($variables = array()) {
    
  }
  /**
   * Upload .article file to entity
   * @param type $variables
   */
  public function upload($variables = array()) {
    $client_request_id = DPSNextUtil::generate_uuid();
    $client_session_id = isset($client_session_id) ? $client_session_id : DPSNextUtil::generate_uuid();
    $client_upload_id = DPSNextUtil::generate_uuid();
    
    $article_version_id = $this->get_entity_version_id();
    
    $headers = array(
      'accept_type' => DPSNextConfig::CONTENT_TYPE_JSON,
      'content_type' => DPSNextConfig::CONTENT_TYPE_FOLIO,
      'request_id' => $client_request_id,
      'session_id' => $client_session_id,
      'upload_id' => $client_upload_id,
    );
    
    // Get filepath
    $file_path = DPSNextUtil::get_article_file_path($this->get_title());
    $url = DPSNextConfig::JUPITER_INJESTION . '/publication/' . DPSNextConfig::$publication_id . '/' . $this->get_entity_type() . '/' . $this->get_entity_name() . ';version=' . $article_version_id . '/contents/folio' ;
    $curl = new DPSNext('PUT', $url, $headers, $file_path, true);
    $curl->exec();
    
    $content_version_id_returned = $curl->get_content_version();
    if ($content_version_id_returned !== FALSE) {
      $_SESSION['dps_article_content_version'] = $content_version_id_returned;
    }
    $version_id_returned = $curl->get_version_id();
    if ($version_id_returned !== FALSE) {
      $this->set_entity_version_id($version_id_returned);
      //dpsbridge_helper_next_set_version($article_metadata['node']['nid'], $version_id_returned);
      return $version_id_returned;
    }
    // $curl->output(TRUE, TRUE);
    return '';
  }
  
  /**
   * Builds a metadata array to send to API.
   * 
   * @return array 
   *   Metadata array that matches with the API
   */
  private function _load_metadata() {
    $data = array(
      'entityName' => $this->get_entity_name(),
      'entityType' => $this->get_entity_type(),
    );
    if ($this->get_title()) { 
      $data['title'] = $this->get_title(); 
    }
    if ($this->get_access_state()) { 
      $data['accessState'] = $this->get_access_state();
    }
    /* if ($this->get_byline()) {
      $data['byline'] = $this->get_byline();
    } */
    // if ($article_bylineUrl) 		 { $data['bylineUrl'] = urlencode($article_bylineUrl); }
    // if ($relatedContent) 		 { $data['relatedContent'] = $article_relatedContent; }
    if ($this->get_keywords()) {
      $data['keywords'] = $this->get_keywords();
    }
    if ($this->get_importance()) {
      $data['importance'] = $this->get_importance(); 
    }
    if ($this->get_is_ad()) {
      $data['isAd'] = $this->get_is_ad();
    }
    if ($this->get_description()) {
      $data['description'] = $this->get_description();
    }
    if ($this->get_hide_from_browser_page()) {
      $data['hideFromBrowsePage'] = $this->get_hide_from_browser_page();
    }
    return $data;
  }
  
  public function set_byline($value) {
    $this->byline = $value;
  }
  public function set_byline_url($value) {
    $this->byline_url = $value;
  }
  public function set_access_state($value) {
    $this->access_state = $value;
  }
  public function set_hide_from_browser_page($value) {
    $this->hide_from_browser_page = $value;
  }

  public function get_byline() {
    return $this->byline;
  }
  public function get_byline_url() {
    return $this->byline_url;
  }
  public function get_access_state() {
    return $this->access_state;
  }
  public function get_hide_from_browser_page() {
    return $this->hide_from_browser_page;
  }
}

/**
 * DPSNextCollection class to define the properties of a collection entity
 */
class DPSNextCollection extends DPSNextEntity {
  protected $entity_type = self::ENTITY_TYPE_COLLECTION;
}