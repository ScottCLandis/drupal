<?php

/**
 * @file
 * fp_config.php
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_settings.php';

abstract class Settings {
  // For DB / Passwords etc.
  static private $protected = array();
  // For all public strings such as meta stuff for site
  static private $public;// = array();

  /** 
   * Create new object and initialise the variables.
   * 
   * @param type $session_key
   * @param type $config_data
   * 
   */
  protected function __construct($session_key, $config_data) {
    if (isset($config_data)) {
      $configDataObject = new ArrayObject($config_data);
      self::$public = $configDataObject->getArrayCopy();
    /*  self::$public =array();
      foreach($config_data as $key => $value) {
        self::$public[$key] = $value;
      }*/
    }
    else {
      self::$public = array();
    }
    self::$public['sessionKey'] = $session_key;
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
