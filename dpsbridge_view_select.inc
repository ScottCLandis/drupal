<!-- ========================
Overlay for adding new folios
========================= -->
<div id="dialog-new" title="Add Selected Articles to New Folio">
	<form method="POST">
		<fieldset>
			<label for="fname">Folio Name:</label>
			<input type="text" name="fname" size="20" id="fname" placeholder="Folio Name" /><br/>
			<label for="pname">Publication Name:</label>
			<input type="text" name="pname" size="20" id="pname" placeholder="Publication Name" /><br/>
			<label for="fnumber">Folio Number:</label>
			<input type="text" name="fnumber" size="20" id="fnumber" placeholder="Folio Number" /><br/>
			<label for="pdate">Publication Date:</label>
			<input type="date" name="pdate" size="20" id="pdate" value="<?= date('Y-m-d') ?>" /><br/>
			<br/><br/>
		</fieldset>
	</form>
</div>
<!-- ===============================
Overlay for updating existing folios
================================ -->
<div id="dialog-existing" title="Add Selected Articles to Existing Folio" class="align-center">
	<form method="POST">
		<fieldset>
			<label for="fid">Folio Name:</label>
			<select id="fid">
      <?php
        foreach ($fp_nodes_all as $fp_node)
          echo "<option value='" . $fp_node->nid . "'>" . $fp_node->title . "</option>\n";
      ?>	
			</select><br/><br/>
		</fieldset>
	</form>
</div>
<!-- ========================
Overlay for displaying status
========================= -->
<div id="dialog-status" title="Notice">
	<div id="status"></div>
</div>
