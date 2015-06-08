<?php
/**
 * @file
 * Configuration and setting file of DPSNext.
 */

/**
 * Handle setting and configuration for DPSNext.
 */
class DPSNextConfig {
  /**
   * 
   * Client assigned string to be used to identify the client application.
   * Best practice is to use a reverse DNS style name.
   * @param {string} $client_id
   * @example $client_id = "SwaggerUI"
   */
  const CLIENT_ID = '';
  
  /**
   * The client-secret credentials, assigned on registration.
   * @param {string} $client_secret
   *
   * Please uncomment below and input your value before use.
   */
  const CLIENT_SECRET = '';

  /**
   * Specify the software version of the client application.
   * Best practice is to use double-dot style notation with a major, minor, and dot release number
   * @param {string} $client_version
   * @example $client_version = '1.3.5'
   */
  const CLIENT_VERSION = '0.0.1';
  
  /**
   * Specify differnt content types.
   */
  const CONTENT_TYPE_ALL = 'application/json, text/plain, */*';
  const CONTENT_TYPE_ENTITY = 'application/vnd.adobe.entity+json';
  const CONTENT_TYPE_ARTICLE = 'application/vnd.adobe.folio+zip';
  const CONTENT_TYPE_JPEG = 'image/jpeg';
  const CONTENT_TYPE_PNG = 'image/png';
  const CONTENT_TYPE_GIF = 'image/gif';
  const CONTENT_TYPE_JSON  = 'application/json';
  const CONTENT_TYPE_URLENCODED = 'application/x-www-form-urlencoded';
  
  /**
   * Specify Endpoint server url: STAGE 01
   */
  const AUTHENTICATION_ENDPOINT = 'https://ims-na1-stg1.adobelogin.com';
  const AUTHORIZATION_ENDPOINT = 'https://authorization.stage02.publish.adobe.io';
  const INJESTION_ENDPOINT = 'https://ings.stage02.publish.adobe.io';
  const PRODUCER_ENDPOINT = 'https://pecs.stage02.publish.adobe.io';
  const PRODUCT_ENDPOINT = 'https://ps.stage02.publish.adobe.io';
  /**
   * Speicify default device token
   */
  const DEVICE_TOKEN_DEFAULT = "";

  /**
   * Specify Drupal variable names
   */
  const VARIABLE_DEVICE_ID = 'dpsbridge_next_device_id';
  const VARIABLE_DEVICE_TOKEN = 'dpsbridge_next_device_token';
  const VARIABLE_AUTHENTICATION_ENDPOINT = 'dpsbridge_next_authentication_endpoint';
  const VARIABLE_AUTHORIZATION_ENDPOINT = 'dpsbridge_next_authorization_endpoint';
  const VARIABLE_INJESTION_ENDPOINT = 'dpsbridge_next_injestion_endpoint';
  const VARIABLE_PRODUCER_ENDPOINT = 'dpsbridge_next_producer_endpoint';
  const VARIABLE_NODE_TYPES = 'dpsbridge_next_node_types';
  const VARIABLE_STYLESHEETS = 'dpsbridge_next_stylesheets';
  const VARIABLE_CLIENT_ID = 'dpsbridge_next_client_id';
  const VARIABLE_CLIENT_SECRET = 'dpsbridge_next_client_secret';
  
  /**
   * Specify session variable names
   */
  const SESSION_ACCESS_TOKEN = 'dpsbridge_next_access_token';
  
  /**
   * The reverse DNS style name describing a publisher and publication.
   * PublicationID must match the regular expression pattern:
   * "[0-9a-zA-Z]{1,20}(\\.[0-9a-zA-Z]{1,20}){0,5}"
   * @param {string} $publication_id
   * @example $publication_id = 'com.adobe.inspiremagazine'
   */
  public static $publication_id = 'com.prerelease.drupal.dpci';

  /**
   * The toggler to turn on or off the debugging statements.
   * - for debugging purposes ONLY
   * @param {boolean} $show_debug
   * @example $show_debug = true; // print debug statements
   * @example $show_debug = false; // don't print debug statements
   */
  protected $show_debug = TRUE;
  
  /**
   * The toggler to turn on or off the debugging statements.
   * - for debugging purposes ONLY
   * @param {boolean} $show_input
   * @example $show_input = true; // print user inputs
   * @example $show_input = false; // don't print user inputs
   */
  protected $show_input = FALSE ;
  
  
}