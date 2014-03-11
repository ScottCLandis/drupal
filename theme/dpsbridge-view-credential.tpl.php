<?php
/**
 * @file
 * Template for folio config page.
 *
 * - $dpsbridge_config
 */
?>

<?php print $dpsbridge_config; ?>
<!-- ==================================================
Overlay for displaying options for testing connectivity 
=================================================== -->
<div id="dialog-option-connectivity" title="Test Connectivity" class="align-center"><br/>
  <label>Select an account to test</label>
  <button type="button" onclick="javascript:connectivity('Amazon')">Amazon</button><br/>
  <button type="button" onclick="javascript:connectivity('Android')">Android</button><br/>
  <button type="button" onclick="javascript:connectivity('Apple')">Apple</button>
</div>
<!-- ========================
Overlay for adding dimensions
========================= -->
<div id="dialog-option-dimension-add" title="Add Dimension" class="align-center"><br/>
  <label for="account-type-add">Select an account:</label>
  <select id="account-type-add">
    <option value="amazon_dimension">Amazon</option>
    <option value="android_dimension">Android</option>
    <option value="apple_dimension">Apple</option>
  </select>
  <label>Enter desired dimension:</label>
  <input id="dlength" type="text" size="5" placeholder="length" /> &nbsp;x&nbsp;
  <input id="dwidth" type="text" size="5" placeholder="width" />
</div>
<!-- ==========================
Overlay for deleting dimensions
=========================== -->
<div id="dialog-option-dimension-delete" title="Delete Dimension" class="align-center"><br/>
  <label>Select an account:</label>
  <select id="account-type-delete" onchange="javascript:refreshDimensions()">
    <option value="amazon">Amazon</option>
    <option value="android">Android</option>
    <option value="apple">Apple</option>
  </select>
  <label for="dimension-list">Select dimension(s) to delete:</label>
  <select id="dimension-list" multiple>
  </select>
</div>
<!-- ========================
Overlay for adding stylesheet
========================= -->
<div id="dialog-option-stylesheet-add" title="Add stylesheet">
  <form id="stylesheet-form" action="<?php echo $GLOBALS['base_url'] ?>/dpsbridge/stylesheet/upload" method="POST" enctype="multipart/form-data"><br/>
    <label for="filename">Enter file name:</label>
    <input id="filename" name="filename" type="text" size="40" />
    <label for="zipfile">Select HTMLResource.zip file:</label>
    <input id="zipfile" name="zipfile" type="file" value="" />
    <label for="derivative">Derivative Of:</label>
    <select id="derivative" name="derivative">
      <option type="radio" value="Bootstrap"> Bootstrap</option>
      <option type="radio" value="Foundation" selected> Foundation</option>
    </select>
  </form> 
</div>
<!-- ==========================
Overlay for deleting stylesheet
=========================== -->
<div id="dialog-option-stylesheet-delete" title="Delete stylesheet" class="align-center"><br/>
  <label for="stylesheet-delete">Select a stylesheet to delete:</label>
  <select id="stylesheet-delete"></select>
</div>
<!-- =============================
Overlay for downloading stylesheet
============================== -->
<div id="dialog-option-stylesheet-download" title="Download stylesheet" class="align-center"><br/>
  <label for="stylesheet-download">Select a stylesheet to download:</label>
  <select id="stylesheet-download"></select>
</div>
<!-- ========================
Overlay for displaying status
========================= -->
<div id="dialog-status" title="Notice">
  <div id="status"></div>
</div>
