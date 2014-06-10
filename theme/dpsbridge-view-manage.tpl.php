<?php
/**
 * @file
 * Template for folio manage page.
 */
?>

<?php print $folio_listing; ?>

<!-- ==============================
Overlay for editing existing folios
=============================== -->
<div id="dialog-edit-folio" title="">
  <?php print $folio_edit_form; ?>
</div>
<!-- ===========================
Overlay for editing folio images
============================ -->
<div id="dialog-edit-cover" title="">
  <?php print $folio_image_form; ?>
</div>

<!-- =======================================
Overlay for selecting article preview option
======================================== -->
<div id="dialog-iframe-preview-option" title="">
  <label>Select the targeted dimension:</label><br/>
  <div class='align-center'>
  <select id="preview-dimension" class='align-center'>
    <option value="480,320">480 x 320</option>
    <option value="960,640">960 x 640</option>
    <option value="1024,600">1024 x 600</option>
    <option value="1024,768">1024 x 768</option>
    <option value="1136,640">1136 x 640</option>
    <option value="1280,800">1280 x 800</option>
    <option value="1366,768">1366 x 768</option>
    <option value="1920,1200">1920 x 1200</option>
  </select>
  </div><br/>
  <label>Select an orientation to preview the article:</label><br/>
  <div class='align-center'>
    <a href="javascript:previewArticle('landscape');" alt="Landscape Mode" title="Landscape Mode">
      <img src="/<?php echo drupal_get_path('module', 'dpsbridge') ?>/images/icons/preview_h.png" /> Landscape Mode
    </a><br/><br/>
    <a href="javascript:previewArticle('portrait');" alt="Portrait Mode" title="Portrait Mode">
      <img src="/<?php echo drupal_get_path('module', 'dpsbridge') ?>/images/icons/preview_v.png" /> Portrait Mode
    </a>
  </div>
</div>
<!-- ==============================================
Overlay for displaying Folio Producer upload status
=============================================== -->
<div id="dialog-status" title="Status">
  <div id="status"></div>
</div>
