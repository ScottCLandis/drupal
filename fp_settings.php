<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_settings.php';

abstract class Settings {
    static private $protected = array(); //For DB / Passwords etc
    static private $public;// = array(); //For all public strings such as meta stuff for site

  protected function __construct($sessionKey,$configData) {
    if(isset($configData)) {
      $configDataObject = new ArrayObject($configData);
      self::$public = $configDataObject->getArrayCopy();
    /*	self::$public =array();
      foreach($configData as $key => $value) {
        self::$public[$key] = $value;
      }*/
    } else {
      self::$public = array();
    }
    self::$public['sessionKey'] = $sessionKey;
  }
	
  public static function getProtected($key) {
      return isset(self::$protected[$key]) ? self::$protected[$key] : false;
  }
	
	public static function getPublicArray() {
		return self::$public;
	}

  public static function getPublic($key) {
      return isset(self::$public[$key]) ? self::$public[$key] : false;
  }
	
	public static function mergePublic($mergeData) {
		self::$public = array_merge(self::$public,$mergeData);
		return self::$public;
	}

  public static function setProtected($key,$value) {
      self::$protected[$key] = $value;
  }

  public static function setPublic($key,$value) {
      self::$public[$key] = $value;
  $_SESSION[self::$public['sessionKey']]=self::$public;
  }

  public function __get($key) {
      return isset(self::$public[$key]) ? self::$public[$key] : false;
  }

  public function __set($key,$value) {
    self::$public[$key]=$value;
    $_SESSION[self::$public['sessionKey']]=self::$public;
  }
	
  public function __isset($key) {
      return isset(self::$public[$key]);
  }
}
