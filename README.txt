============
Installation
============

Dependencies
============
Entity Reference
Entity API
Chaos Tools
Views
Views Bulk Operations
Views PHP
Shrink the Web (optional)

Permissions
===========
Navigate to [base_url]/admin/people/permissions to configuring the user permissions for this module.

Content Type
============
Folio content type is added by this module.

field_folio_product_id
Folio Producer metadata: folio folder ID

field_folio_publication_name
Folio Producer metadata: folio publication name

field_folio_number
Folio Producer metadata: folio number

field_folio_description
Folio Producer metadata: folio description

field_folio_dimension
Folio Producer metadata: folio targeted dimension

field_folio_viewer_version
Folio Producer metadata: folio targeted viewer version

field_folio_producer_accounts
Folio Producer Account Types: Amazon, Android, & Apple

field_folio_producer_timestamp
The last date that the folio was uploaded to the Folio Producer

field_folio_status
Upload status (Uploaded vs Not uploaded)

field_folio_sync_status
Synchronication status with Folio Producer

field_folio_orientation
Folio Producer metadata: article orientation (landscape, portrait, or both)

field_folio_publication_css
Stylesheet for generated HTML Articles

field_folio_published_date
Folio Producer metadata: folio published date

field_folio_toc
Table of Contents status (Generated vs Not Generated) <- unnecessary ATM

field_folio_toc_layout
Stylesheet for generated Table of Contents

field_folio_library_filter
Folio Producer metadata: folio library filter

field_folio_ads_filter
Keeps track of which article is an ad and which isn't

field_folio_landscape_url
Folio landscape image, used for the auto-generated cover page

field_folio_portrait_url
Folio portrait image, used for the auto-generated cover page

field_folio_reference
Reference to the respective article nodes within the folio node

field_folio_producer_article_id
Reference to the non-Drupal articles
Folio Selection view

emory Issue
============

Drupal default memory limit issues (i.e. blank page when loading Drupal)
Configure following:
php_value memory_limit 128M
