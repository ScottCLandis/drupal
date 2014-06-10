<?php
/**
 * @file
 * Template for edit existing folio form.
 */
?>

<div style="margin-top:10px;margin-bottom:10px;">
    <table id="edit-folio-table" class="views-table cols-12"><tbody>
    <tr class="odd views-row-first">
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_fname']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_pname']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_fnumber']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_pdate']); ?>
      </td>
      <td class="views-field" rowspan="2">
        <div><label>Cover Previews:</label></div>
        <div class="align-center down-5">
          <a href="javascript:jQuery('#dialog-edit-cover').dialog('open');">
            <img id="image" style="height:100px;width:150px;" src="" />
          </a></div>
      </td></tr>
    <tr class="even">
      <td class="views-field" rowspan="2">
        <?php echo drupal_render($form['folio_ui_fdesc']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['accounts']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_filter']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['pubcss']); ?>
      </td>
    </tr>
    <tr class="odd views-row-last">
      <td class="views-field">
        <?php echo drupal_render($form['dimension']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_fversion']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_orientation']); ?>
      </td>
      <td class="views-field">
        <?php echo drupal_render($form['folio_ui_generator']); ?>
      </td>
    </tr>
    </tbody>
    </table>
    <div class="folio-edit-dialog-articles">
    <table id="sortable-table">
    <thead>
    <tr>
      <th>&nbsp;</th>
      <th>&nbsp;</th>
      <th>Order</th>
      <th>Edit</th>
      <th>Article Title</th>
      <th>Type</th>
      <!--<th>Override CSS</th>-->
      <th>Last Modified</th>
      <th>Is Ad</th>
      <th>In Sync</th>
    </tr>
    </thead><tbody id="articles-wrapper"></tbody></table>
    </div>
  </div>
<?php echo drupal_render_children($form); ?>
