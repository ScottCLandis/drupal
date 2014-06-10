<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_library.php';

/**
 * FolioProducer Helper.
 */
class DPSBridgeFPHelper {
  public $fp;
  public $config;

  /**
   * Constructor.
   */
  public function __construct($email, $password, $consumer_key, $consumer_secret) {
    $this->fp = new DPSBridgeFPLibrary(array(
      'user_email' => $email,
      'user_password' => $password,
      'consumer_key' => $consumer_key,
      'consumer_secret' => $consumer_secret,
    ));
  }

  /**
   * Create Session.
   */
  public function createSession() {
    $fp_sessiondata = $this->fp->request('POST', 'sessions');
    return $fp_sessiondata;
  }

  /**
   * Create Distribution Session.
   */
  public function createDistributionSession() {
    $fp_sessiondata = $this->fp->request('GET', 'ddp/issueServer/signInWithCredentials?emailAddress=' . $this->fp->config->user_email . '&password=' . $this->fp->config->user_password, array(), '', FALSE, TRUE);
    return $fp_sessiondata;
  }

  /**
   * Delete Session.
   */
  public function deleteSession() {
    return $this->fp->request('DELETE', 'sessions');
  }

  /**
   * Create Folio.
   */
  public function createFolio($folio_params) {
    if (!isset($folio_params['folioName'])) {
      throw new Exception('Folio parameters required');
    }
    return $this->fp->request('POST', 'folios', $folio_params);
  }

  /**
   * Delete Folio.
   */
  public function deleteFolio($folio_id) {
    if (!isset($folio_id)) {
      throw new Exception('Folio ID required');
    }
    return $this->fp->request('DELETE', 'folios/' . $folio_id);
  }

  /**
   * Update Folio.
   */
  public function updateFolio($folio_id, $metadata) {
    if (!isset($folio_id) || !isset($metadata)) {
      throw new Exception('Folio ID and parameters required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id, $metadata);
  }

  /**
   * Metadata for either all folios or particular one.
   *
   * Depending on whether folioID is set or not.
   *
   * @param string $folio_id
   *   Value of Folio Id.
   */
  public function folios($folio_id = '') {
    return $this->fp->request('GET', 'folios/' . $folio_id);
  }

  /**
   * Articles.
   */
  public function articles($folio_id) {
    if (!isset($folio_id)) {
      throw new Exception('Folio ID required');
    }
    return $this->fp->request('GET', 'folios/' . $folio_id . '/articles' . '?resultData="All"');
  }

  /**
   * Update Article.
   */
  public function updateArticle($folio_id, $article_id, $metadata) {
    if (!isset($folio_id) || !isset($article_id) || !isset($metadata)) {
      throw new Exception('Folio ID and parameters required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/articles/' . $article_id . '/metadata', $metadata);
  }

  /**
   * Upload Article.
   */
  public function uploadArticle($folio_id, $metadata, $filepath) {
    if (!isset($folio_id) || !isset($filepath)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/articles/'/*.'?name='.$metadata['name']*/, $metadata, $filepath);
  }

  /**
   * Delete Article.
   */
  public function deleteArticle($folio_id, $article_id) {
    if (!isset($folio_id) || !isset($article_id)) {
      throw new Exception('Folio ID and Article ID required');
    }
    return $this->fp->request('DELETE', 'folios/' . $folio_id . '/articles/' . $article_id);
  }

  /**
   * Upload HTML Resources.
   */
  public function uploadHTMLResources($folio_id, $filepath) {
    if (!isset($folio_id) || !isset($filepath)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/htmlresources', '', $filepath);
  }

  /**
   * Upload Cover.
   */
  public function uploadCover($folio_id, $orientation, $url) {
    if (!isset($folio_id) || !isset($orientation) || !isset($url)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/previews/' . $orientation, '', $url);
  }
}
