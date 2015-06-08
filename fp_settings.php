<?php
/**
 * @file
 * Folio Producer Config file.
 */

require_once dirname(__FILE__) . '/fp_settings.php';

abstract class DPSBridgeSettings {
  // For DB / Passwords etc.
  static protected $protected = array();
  // For all public strings such as meta stuff for site.
  static protected $public;

  /**
   * Create new object and initialise the variables.
   *
   * @param object $session_key
   *   Session Key.
   * @param object $config_data
   *   Config Data.
   */
  protected function __construct($session_key, $config_data) {
    if (isset($config_data)) {
      $config_data_object = new ArrayObject($config_data);
      self::$public = $config_data_object->getArrayCopy();
    }
    else {
      self::$public = array();
    }
    self::$public['sessionKey'] = $session_key;
  }
  /**
   * Get Protected.
   */
  public static function getProtected($key) {
    return isset(self::$protected[$key]) ? self::$protected[$key] : FALSE;
  }
  /**
   * Get Public Array.
   */
  public static function getPublicArray() {
    return self::$public;
  }
  /**
   * Get Public.
   */
  public static function getPublic($key) {
    return isset(self::$public[$key]) ? self::$public[$key] : FALSE;
  }
  /**
   * Merge Public.
   */
  public static function mergePublic($merge_data) {
    self::$public = array_merge(self::$public, $merge_data);
    return self::$public;
  }
  /**
   * Set Protected.
   */
  public static function setProtected($key, $value) {
    self::$protected[$key] = $value;
  }
  /**
   * Set Public.
   */
  public static function setPublic($key, $value) {
    self::$public[$key] = $value;
    $_SESSION[self::$public['sessionKey']] = self::$public;
  }
  /**
   * Getter.
   */
  public function __get($key) {
    return isset(self::$public[$key]) ? self::$public[$key] : FALSE;
  }
  /**
   * Setter.
   */
  public function __set($key, $value) {
    self::$public[$key] = $value;
    $_SESSION[self::$public['sessionKey']] = self::$public;
  }
  /**
   * Is Set.
   */
  public function __isset($key) {
    return isset(self::$public[$key]);
  }
}
