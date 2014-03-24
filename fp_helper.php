<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_library.php';

/**
 * FolioProducer Helper.
 * R25
 * @dbeaton
 */
class FPHelper {
  public $fp;
  public $config;

  /**
   * Constructor.
   */
  function __construct($email, $password, $consumer_key, $consumer_secret) {
    $this->fp = new FPLibrary(array(
      'user_email' => $email,
      'user_password' => $password,
      'consumer_key' => $consumer_key,
      'consumer_secret' => $consumer_secret,
    ));
  }

  /**
   * Create Session.
   */
  public function create_session() {
    $fp_sessiondata = $this->fp->request('POST', 'sessions');
    return $fp_sessiondata;
  }

  /**
   * Create Distribution Session.
   */
  public function create_distribution_session() {
    $fp_sessiondata = $this->fp->request('GET', 'ddp/issueServer/signInWithCredentials?emailAddress=' . $this->fp->config->user_email . '&password=' . $this->fp->config->user_password, array(), '', FALSE, TRUE);
    return $fp_sessiondata;
  }

  /**
   * Delete Session.
   */
  public function delete_session() {
    return $this->fp->request('DELETE', 'sessions');
  }

  /**
   * Create Folio.
   */
  public function create_folio($folio_params) {
    if (!isset($folio_params['folioName'])) {
      throw new Exception('Folio parameters required');
    }
    return $this->fp->request('POST', 'folios', $folio_params);
  }

  /**
   * Delete Folio.
   */
  public function delete_folio($folio_id) {
    if (!isset($folio_id)) {
      throw new Exception('Folio ID required');
    }
    return $this->fp->request('DELETE', 'folios/' . $folio_id);
  }

  /**
   * Update Folio.
   */
  public function update_folio($folio_id, $metadata) {
    if (!isset($folio_id) || !isset($metadata)) {
      throw new Exception('Folio ID and parameters required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id, $metadata);
  }

  /**
   * Metadata for either all folios or particular one.
   * 
   * Depending on whether folioID is set or not
   * 
   * @param string $folio_id
   *   Value of Folio Id.   * 
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
  public function update_article($folio_id, $article_id, $metadata) {
    if (!isset($folio_id) || !isset($article_id) || !isset($metadata)) {
      throw new Exception('Folio ID and parameters required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/articles/' . $article_id . '/metadata', $metadata);
  }

  /**
   * Upload Article.
   */
  public function upload_article($folio_id, $metadata, $filepath) {
    if (!isset($folio_id) || !isset($filepath)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/articles/'/*.'?name='.$metadata['name']*/, $metadata, $filepath);
  }

  /**
   * Delete Article.
   */
  public function delete_article($folio_id, $article_id) {
    if (!isset($folio_id) || !isset($article_id)) {
      throw new Exception('Folio ID and Article ID required');
    }
    return $this->fp->request('DELETE', 'folios/' . $folio_id . '/articles/' . $article_id);
  }

  /**
   * Upload HTML Resources.
   */
  public function upload_htmlresources($folio_id, $filepath) {
    if (!isset($folio_id) || !isset($filepath)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/htmlresources', '', $filepath);
  }

  /**
   * Upload Cover.
   */
  public function upload_cover($folio_id, $orientation, $url) {
    if (!isset($folio_id) || !isset($orientation) || !isset($url)) {
      throw new Exception('Folio ID and File required');
    }
    return $this->fp->request('POST', 'folios/' . $folio_id . '/previews/' . $orientation, '', $url);
  }
}
