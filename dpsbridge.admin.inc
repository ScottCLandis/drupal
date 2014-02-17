<?php

/**
 * @file
 * dpsbridge.admin.inc
 * Defines admin config. screens and functionality for the dpsbridge module
 */

/**
 * General admin settings form for dpsbridge
 */
function dpsbridge_settings() {
  $form = array();  
  // Server information.

  return system_settings_form($form);
}
