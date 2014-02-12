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
	public function delete_folio($folioID) 
	{
		if (!isset($folioID))
			throw new Exception('Folio ID required');
		return $this->fp->request('DELETE', 'folios/'.$folioID);
	}
	public function update_folio($folioID, $metadata) 
	{
		if (!isset($folioID) || !isset($metadata))
			throw new Exception('Folio ID and parameters required');
		return $this->fp->request('POST', 'folios/'.$folioID, $metadata);
	}
	/**
	 * Metadata for either all folios or particular one 
	 * Depending on whether folioID is set or not
	 * @param  string $folioId 
	 */
	public function folios($folioID = '') 
	{
		return $this->fp->request('GET', 'folios/'.$folioID);
	}
	public function articles($folioID)
	{
		if (!isset($folioID) )
			throw new Exception('Folio ID required');
		return $this->fp->request('GET', 'folios/'.$folioID.'/articles'.'?resultData="All"');
	}
	public function update_article($folioID, $articleID, $metadata) 
	{
		if (!isset($folioID) || !isset($articleID) || !isset($metadata))
			throw new Exception('Folio ID and parameters required');
		return $this->fp->request('POST', 'folios/'.$folioID.'/articles/'.$articleID.'/metadata', $metadata);
	}
	public function upload_article($folioID, $metadata, $filePath)
	{
		if (!isset($folioID) || !isset($filePath) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folioID.'/articles/'/*.'?name='.$metadata['name']*/, $metadata, $filePath);
	}
	public function delete_article($folioID, $articleID) 
	{
		if (!isset($folioID) || !isset($articleID) )
			throw new Exception('Folio ID and Article ID required');
		return $this->fp->request('DELETE', 'folios/'.$folioID.'/articles/'.$articleID);
	}

	public function upload_htmlresources($folioID, $filePath)
	{
		if (!isset($folioID) || !isset($filePath) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folioID.'/htmlresources', '', $filePath);
	}

	public function upload_cover($folioID, $orientation, $url)
	{
		if (!isset($folioID) || !isset($orientation) || !isset($url) )
			throw new Exception('Folio ID and File required');
		return $this->fp->request('POST', 'folios/'.$folioID.'/previews/'.$orientation, '', $url);
	}
}

?>
