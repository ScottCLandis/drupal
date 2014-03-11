<?php

require_once dirname(__FILE__).'/fp_library.php';

/**
* FolioProducer Helper
* R25
* @dbeaton
*/
class FPHelper
{
	var $fp;
	var $config;

	function __construct($email, $password, $consumer_key, $consumer_secret)
	{
		$this->fp = new FPLibrary(array(
			'user_email'	=> $email,
			'user_password'	=> $password,
			'consumer_key' => $consumer_key,
			'consumer_secret' => $consumer_secret
		));
	}
	public function create_session()
	{
		$fp_sessiondata = $this->fp->request('POST', 'sessions');
		//$_SESSION['ticket'] = $fp_sessiondata['ticket'];
		return $fp_sessiondata;
	}
	public function create_distribution_session()
	{
		$fp_sessiondata = $this->fp->request('GET', 'ddp/issueServer/signInWithCredentials?emailAddress='.$this->fp->config->user_email.'&password='.$this->fp->config->user_password,array(), '', false, true);
		//$_SESSION['ticket'] = $fp_sessiondata['ticket'];
		return $fp_sessiondata;
	}
	public function delete_session()
	{
		return $this->fp->request('DELETE', 'sessions');
	}
	public function create_folio($folioParams)
	{
		if (!isset($folioParams['folioName']))
			throw new Exception('Folio parameters required');
		return $this->fp->request('POST', 'folios', $folioParams);
	}
	public function delete_folio($folio_id)
	{
		if (!isset($folio_id))
			throw new Exception('Folio ID required');
		return $this->fp->request('DELETE', 'folios/'.$folio_id);
	}
	public function update_folio($folio_id, $metadata)
	{
		if (!isset($folio_id) || !isset($metadata))
			throw new Exception('Folio ID and parameters required');
		return $this->fp->request('POST', 'folios/'.$folio_id, $metadata);
	}
	/**
	 * Metadata for either all folios or particular one
	 * Depending on whether folioID is set or not
	 * @param  string $folio_id
	 */
	public function folios($folio_id = '')
	{
		return $this->fp->request('GET', 'folios/'.$folio_id);
	}
	public function articles($folio_id)
	{
		if (!isset($folio_id) )
			throw new Exception('Folio ID required');
		return $this->fp->request('GET', 'folios/'.$folio_id.'/articles'.'?resultData="All"');
	}
	public function update_article($folio_id, $articleID, $metadata)
	{
		if (!isset($folio_id) || !isset($articleID) || !isset($metadata))
			throw new Exception('Folio ID and parameters required');
		return $this->fp->request('POST', 'folios/'.$folio_id.'/articles/'.$articleID.'/metadata', $metadata);
	}
	public function upload_article($folio_id, $metadata, $filePath)
	{
		if (!isset($folio_id) || !isset($filePath) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folio_id.'/articles/'/*.'?name='.$metadata['name']*/, $metadata, $filePath);
	}
	public function delete_article($folio_id, $articleID)
	{
		if (!isset($folio_id) || !isset($articleID) )
			throw new Exception('Folio ID and Article ID required');
		return $this->fp->request('DELETE', 'folios/'.$folio_id.'/articles/'.$articleID);
	}

	public function upload_htmlresources($folio_id, $filePath)
	{
		if (!isset($folio_id) || !isset($filePath) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folio_id.'/htmlresources', '', $filePath);
	}

	public function upload_cover($folio_id, $orientation, $url)
	{
		if (!isset($folio_id) || !isset($orientation) || !isset($url) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folio_id.'/previews/'.$orientation, '', $url);
	}
}
