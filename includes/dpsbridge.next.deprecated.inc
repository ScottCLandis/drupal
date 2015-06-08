<?php
/**
 * @file
 * Objects, properties, and methods to communicate with the DPSNext API
 */

/**
 * DPSNext class for communicating with the DPSNext API.
 */
class DPSNext extends DPSNextConfig {
	private $curl;
	private $curl_option;
	private $curl_response;
	private $file_path;
	private $request_data;
	private $request_method;
	private $request_header;
	private $request_url;
	private $verbose;

	private $client_request_id;
	private $client_session_id;

	private $accept_type;
	private $content_type;
  
  /**
   * Specify entity type of the content
   * @var string
   */
  private $entity_type = 'article';
  
	public function __construct($type, $url, $headers, $data = null, $isFile = false) {
		if ($data !== null) {
			if ($isFile) { // generates file data
				$this->file_path = $data;
			} else { // store data parameters
				$this->request_data = json_encode($data);
			}
		}
		$this->request_method = $type;
		$this->request_url = $url;
		$this->_set_headers($headers);
		$this->_set_options();
	}

	public function exec() {
		// initializes the cURL
		$this->curl = curl_init();
		// initializes cURL parameters
		curl_setopt_array($this->curl, $this->curl_option);
		// executes cURL request
		$this->curl_response = curl_exec($this->curl);
		// closes cURL request
		curl_close($this->curl);
	}

	public function output($input = false, $debug = false) {
		echo '<pre>';
		if ($input) {
			echo '<h3>cURL Options:</h3>';
			print_r($this->curl_option);
			echo '<h3>cURL Response:</h3>';
		}
		if (strpos($this->curl_response, '<?xml') === 0) { // displays XML data
			print_r($this->curl_response);
		} else { // displays JSON data
			print_r(json_decode($this->curl_response, true));
		}
		if ($debug) {
			echo '<h3>Verbose information:</h3>';
			echo !rewind($this->verbose);
			echo htmlspecialchars(stream_get_contents($this->verbose));
		}
		echo '</pre>';
	}

	public function response() {
		return $this->curl_response;
	}

	public function get_content_version() {
		$data = json_decode($this->curl_response, true);
    if (isset($data['_links']['contentUrl']['href'])) {
      $content_url = $data['_links']['contentUrl']['href'];
      $index = strrpos($content_url, '=');
      return substr($content_url, $index + 1, -1);
    } 
    else {
      return FALSE;
    }
	}

	public function get_version_id() {
		$data = json_decode($this->curl_response, true);
    if (isset($data['version'])) {
      return $data['version'];
    }
    else {
      return FALSE;
    }
	}

	private function _set_headers($headers) {
		$accept_type = $headers['accept_type'];
		$client_id = self::CLIENT_ID;//$headers['client_id'];
		$client_version = self::CLIENT_VERSION;//$headers['client_version'];
		$client_request_id = $headers['request_id'];
		$client_session_id = $headers['session_id'];

		$this->request_header = array(
			'Accept: ' . $accept_type,
			'X-DPS-Client-Id: ' . $client_id,
			'X-DPS-Client-Request-Id: ' . $client_request_id,
			'X-DPS-Client-Session-Id: ' . $client_session_id,
			'X-DPS-Client-Version: ' . $client_version,
		);

		// sets the upload ID IFF when necessary
		if (isset($headers['upload_id']))
			$this->request_header[] = 'X-DPS-Upload-Id: ' . $headers['upload_id'];
		// sets the content type to send IFF it is given
		if (isset($headers['content_type']) && $headers['content_type'] !== '')
			$this->request_header[] = 'Content-Type: ' . $headers['content_type'];
	}

	private function _set_options() {
		$this->curl_option[CURLOPT_CUSTOMREQUEST] = $this->request_method;
		$this->curl_option[CURLOPT_HTTPHEADER] = $this->request_header;
		$this->curl_option[CURLOPT_RETURNTRANSFER] = true;
		$this->curl_option[CURLOPT_STDERR] = $this->verbose = fopen('php://temp', 'rw+');
		$this->curl_option[CURLOPT_URL] = $this->request_url;
		$this->curl_option[CURLOPT_USERAGENT] = $_SERVER['HTTP_USER_AGENT'];
		$this->curl_option[CURLOPT_VERBOSE] = true;

		if ($this->request_data !== null) { // append JSON data
			$this->curl_option[CURLOPT_POSTFIELDS] = $this->request_data;
		} else if ($this->file_path !== null) { // append file data
			$this->curl_option[CURLOPT_INFILE] = fopen($this->file_path, 'r');
			// $this->curl_option[CURLOPT_BUFFERSIZE] = 128;
			$this->curl_option[CURLOPT_INFILESIZE] = filesize($this->file_path);
			$this->curl_option[CURLOPT_UPLOAD] = true;
			// $this->curl_option[CURLOPT_NOPROGRESS] = false;
		}
	}
}