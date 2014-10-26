CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers


INTRODUCTION
------------
The DPSBridge module converts selected articles into HTML stacks and then
uploads the articles directly to Adobe DPS Folio Producer by utilizing the Folio
Producer APIs. In addition, DPSBridge compares each selected Drupal article with
the version in the folio and indicates when the content is out of sync. An
authorized user can then update the contents of the folio to utilize the most
current version of the Drupal article.

The DPSBridge Module allows you to work within Drupal to:

 * Create a new HTML Folio for Adobe DPS
 * Search for and select articles for publishing to Adobe DPS
 * Add and remove HTML articles from a folio
 * Convert images associated with a Drupal node to a slideshow in the HTML
   article
 * Assign your own CSS to style the resulting HTML article
 * Reorder articles contained within an HTML Folio
 * Compare Drupal article content to that which has been uploaded to Folio
   Producer with the option to update the folio articles to keep the content in
   sync (for Adobe DPS Enterprise customers)
 * Clone a folio to create a copy of the selected folio and all article
   references (useful when publishing the same content to multiple marketplaces)
 * Upload article content directly to Folio Producer (for Adobe DPS Enterprise
   customers; requires no interaction with Adobe InDesign)
 * Automatically generate a Cover article and Table of Contents article if
   desired


REQUIREMENTS
------------
 The DPSBridge module requires the following modules:

 * Entity reference (https://www.drupal.org/project/entityreference)
 * Entity API (https://www.drupal.org/project/entity)
 * Chaos tool suite (https://www.drupal.org/project/ctools)
 * Views (https://www.drupal.org/project/views)
 * Views Bulk Operations (https://www.drupal.org/project/views_bulk_operations)
 * Views PHP (https://www.drupal.org/project/views_php)

 This module integrates with Adobe Digital Publishing Suite (Adobe DPS).
 An Adobe Digital Publishing Suite Enterprise License is required to publish the
 generated folios on with Adobe.


RECOMMENDED MODULES
-------------------
 * Display Suite (https://www.drupal.org/project/ds)
  When enabled, it harness the power of Display Suite for theming the HTML folio


INSTALLATION
------------
 * Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
 * Configure user permissions in Administration » Content Authoring » DPS Bridge
   Configuration:
   - Configure the Adobe API Key and API Secret
   - Configure any of the 3 publishing platforms (Amazon, Android, Apple)
 * Configure user permissions in Administration » People » Permissions:
 * A screencast "DPSBridge: Getting Started" i available here, which describes
   how to install, configure and use this module with Adobe DPS.
   http://youtu.be/PufG2DosTkU

TROUBLESHOOTING
---------------
 Drupal default memory limit issues (i.e. blank page when loading Drupal)
 Configure following:  php_value memory_limit 128M

MAINTAINERS
-----------
Current maintainers:
 * Joozer Tohfafarosh (justy) - https://www.drupal.org/user/115811
This project has been sponsored by:
 * DPCI
   This project is maintained by staff at DPCI. For more information about this
   module or for consulting and implementation support, please contact us
   directly at (800) 818-2905 or http://www.databasepublish.com.
