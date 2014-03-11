<?php
require_once dirname(__FILE__).'/fp_settings.php';

class FPConfig extends Settings
{
	var $response = array();
	var $version = "1.01";

	/**
	 * Create new object and initalise the variables
	 * @param array $config stores parameters
	 *
     * Call this method to get singleton
     * @return UserFactory
     */
    public static function Instance()
    {
        static $inst = null;
        if ($inst === null) {
            $inst = new FPConfig();
        }
        return $inst;
    }

    /**
     * protected constructor so others can not violate the singleton
     */
	protected function __construct() 
	{
		session_start();

		$preconfigured = false;
		$cconfig = isset($_SESSION['config'])?$_SESSION['config']:array();
		if($cconfig || !isset($cconfig['mode'])) $preconfigured = false;
		else $preconfigured=true;

		parent::__construct('config', $cconfig);

		if(!$preconfigured) {
			$this->mode ='debug';
			$this->server_family='production';
			
			if($this->mode=='release') {
				$this->proxy='';
				$this->use_ssl=true;
				$this->require_ssl= true;
			} else {
				$this->use_ssl = true;
				$this->proxy='';//'127.0.0.1';
				$this->require_ssl=false;
			}
		
			if($this->server_family=='production') {
				$this->host = 'dpsapi2.digitalpublishing.acrobat.com';
				$this->distributionHost = 'origin.adobe-dcfs.com';
			} else { // staging
				$this->host = 'dpsapi2-stage.digitalpublishing.acrobat.com';
				$this->distributionHost = 'origin-stage.adobe-dcfs.com';
			}
			
			$this->user_agent = 'PHP';
			$this->consumer_key = '';
			$this->consumer_secret='';
			$this->user_email = '';
			$this->user_password = '';
			$this->timestamp='';
			$this->oauth_signature_method='HAC-SHA256';
			$this->curl_ssl_verifyhost='';
			$this->curl_ssl_verifypeer='';
			$this->curl_ca_info= '';
			$this->curl_capath = '';
		}
		
		if($this->require_ssl && $_SERVER["HTTPS"] != "on") {
			session_write_close();
			header("HTTP/1.1 301 Moved Permanently");
			header("Location: https://" . $_SERVER["SERVER_NAME"] . $_SERVER["REQUEST_URI"]);
			exit();
		}
	}
}
