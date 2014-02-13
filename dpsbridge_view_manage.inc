<!-- ==============================
Overlay for editing existing folios 
=============================== -->
<div id="dialog-edit-folio" title="">
	<form action="#" method="POST">
	<div style="margin-top:10px;margin-bottom:10px;">
		<table id="edit-folio-table" class="views-table cols-12"><tbody>
		<tr class="odd views-row-first">
			<td class="views-field">
				<label for="folio-ui-fname">Folio Name:</label>
				<input type="text" id="folio-ui-fname" value="" placeholder="Folio Name" /></td>
			<td class="views-field">
				<label for="folio-ui-pname">Publication Name:</label>
				<input type="text" id="folio-ui-pname" value="" placeholder="Publication Name" /></td>
			<td class="views-field">
				<label for="folio-ui-fnumber">Folio Number:</label>
				<input type="text" id="folio-ui-fnumber" value="" placeholder="Folio Number" /></td>
			<td class="views-field">
				<label for="folio-ui-pdate">Publication Date:</label>
				<input type="date" id="folio-ui-pdate" value="" id="pdate" /></td>
			<td class="views-field" rowspan="2">
				<div><label>Cover Previews:</label></div>
				<div class="align-center down-5">
					<a href="javascript:$('#dialog-edit-cover').dialog('open');">
						<img id="image" style="height:100px;width:150px;" src="" />
					</a></div>
			</td></tr>
		<tr class="even">
			<td class="views-field" rowspan="2">
				<label for="folio-ui-fdesc" class="align-left">Description:</label><br/>
				<textarea id="folio-ui-fdesc" rows="4" placeholder="Folio Description"></textarea></td>
			<td class="views-field">
				<label for="accounts">DPS Account:</label>
				<select id="accounts" onchange="javascript:refreshDimensions()">
					<option value="amazon">Amazon</option>
					<option value="android">Android</option>
					<option value="apple">Apple</option>
				</select></td>
			<td class="views-field">
				<label for="filter">Library Filter:</label>
				<input type="text" id="folio-ui-filter" placeholder="Library Filter" /></td>
			<td class="views-field">
				<label for="pubcss">HTML Resources:</label>
				<select id="pubcss"></select></td></tr>
		<tr class="odd views-row-last">
			<td class="views-field">
				<label for="dimension">Resolution:</label>
				<select id="dimension"></select></td>
			<td class="views-field">
				<label for="folio-ui-fversion">Viewer Version:</label>
				<select id="folio-ui-fversion">
					<option value="24.0.0">24</option>
					<option value="25.0.0">25</option>
					<option value="26.0.0">26</option>
					<option value="27.0.0">27</option>
				</select></td>
			<td class="views-field">
				<label for="folio-ui-orientation">Folio Orientation:</label>
				<select id="folio-ui-orientation">
					<option value="Landscape">Horizontal Orientation</option>
					<option value="Portrait">Vertical Orientation</option>
					<option value="Always">Both Orientations</option>
				</select></td>
			<td class="views-field">
				<label for="folio-ui-generator">TOC & Cover:</label>
				<select id="folio-ui-generator">
					<option value="Generate ToC">Generate ToC Only</option>
					<option value="Generate Cover">Generate Cover Only</option>
					<option value="Generate Both">Generate Both</option>
					<option value="Generate None">Generate None</option>
				</select></td></tr>
		</tbody></table>
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
	</form>
</div>
<!-- ===========================
Overlay for editing folio images
============================ -->
<div id="dialog-edit-cover" title="">
	<form id="image-form" action="<?= $GLOBALS['base_url'] ?>/dpsbridge/img/upload" method="POST" enctype="multipart/form-data">
		<input type="hidden" id="image-ui-fname" name="fname" value="" />
		<input type="hidden" id="image-ui-fid" name="fid" value="" />
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
			<td><div class="file-upload"><input id="portrait-file" name="portrait" type="file" value="" accept="image/*" /></div></td>
			<td><div class="file-upload"><input id="landscape-file" name="landscape" type="file" value="" accept="image/*" /></div></td>
		</tr>
		</table>
	</form>
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
			<img src="<?= drupal_get_path('module', 'dpsbridge') ?>/images/icons/preview_h.png" /> Landscape Mode
		</a><br/><br/>
		<a href="javascript:previewArticle('portrait');" alt="Portrait Mode" title="Portrait Mode">
			<img src="<?= drupal_get_path('module', 'dpsbridge') ?>/images/icons/preview_v.png" /> Portrait Mode
		</a>
	</div>
</div>
<!-- ==============================================
Overlay for displaying Folio Producer upload status
=============================================== -->
<div id="dialog-status" title="Status">
	<div id="status"></div>
</div>
