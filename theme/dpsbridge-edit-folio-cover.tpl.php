<?php
/**
 * @file
 * Template for edit folio cover form.
 */
?>

<?php echo drupal_render($form['fname']); ?>
<?php echo drupal_render($form['fid']); ?>
<label style="margin-bottom:10px;">Cover Image</label>
<table>
<tr>
  <td>
    <img id="portrait" style="height:300px;width:200px;" src="" />
    <div id="portrait-dlt" class="hide"><br/><br/>
    <a class="ui-button ui-widget ui-state-default ui-button-text-icon-primary" onclick="javascript:alert('work-in-progress')">
      <span class="ui-button-icon-primary ui-icon ui-icon-circle-close"></span>
      <span class="ui-button-text">Delete Image</span>
    </a>
    </div>
  </td><td><br/><br/>
    <img id="landscape" style="height:200px;width:300px;" src="" />
    <div id="landscape-dlt" class="hide"><br/><br/><br/><br/>
    <a class="ui-button ui-widget ui-state-default ui-button-text-icon-primary" onclick="javascript:alert('work-in-progress')">
      <span class="ui-button-icon-primary ui-icon ui-icon-circle-close"></span>
      <span class="ui-button-text">Delete Image</span>
    </a>
    </div>
  </td>
</tr><tr>
  <td><div class="file-upload"><?php echo drupal_render($form['portrait']); ?></div></td>
  <td><div class="file-upload"><?php echo drupal_render($form['landscape']); ?></div></td>
</tr>
</table>
<?php echo drupal_render_children($form); ?>
