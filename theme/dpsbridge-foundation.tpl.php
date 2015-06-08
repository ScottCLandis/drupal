<?php
/**
 * @file
 * Template for foundation page.
 */
?>
<!doctype html>
<html class='no-js' lang='en'>
<head>
  <meta charset='utf-8' />
  <meta name='viewport' content='width=device-width' />
  <title></title>
  <link rel='stylesheet' href='../HTMLResources/css/normalize.css' />
  <link rel='stylesheet' href='../HTMLResources/css/style.css' />
  <link rel='stylesheet' href='../HTMLResources/css/override.css' />
  <script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>
  <script type='text/javascript'>try{Typekit.load();}catch(e){}</script>
  <script type='text/javascript' src='../HTMLResources/js/vendor/custom.modernizr.js'></script>
</head>
<body>
  <div id='pubtitle'><?php print $publication; ?></div>
  <div id='kicker'><?php print $kicker; ?></div>
  <div id='display-home' class='row'>
    <div class='large-12 small-12 columns'>
      <div class='large-12 small-12 columns'>
      <h1 class='title'><?php print $title; ?></h1>
      <h5>By <?php print $author; ?></h5>
      </div>
      <div class='large-12 small-12 columns'>
      <?php if ($images):?>
        <div id='imageArea' class='large-6 small-12 columns'>
          <div class='row'>
          <?php
          // If there are only one image.
          if (count($images) == 1): ?>
            <img src='<?php print dpsbridge_helper_link_img($filename, $images[0]); ?>' width='100%' /><br/>
          <?php
          // Creates a slideshow if there are multiple images.
          else: ?>
            <ul data-orbit>
            <?php
            for ($q = 0; $q < count($images); $q++): ?>
              <li>
                <img src='<?php print dpsbridge_helper_link_img($filename, $images[$q]); ?>' />
                <div class='orbit-caption'>Some caption here</div>
              </li>
            <?php
            endfor; ?>
            </ul>
          <?php
          endif; ?>
          </div>
        </div>
      <?php
      // If there are embedded videoes.
      elseif ($videos): ?>
        <div id='videoArea' class='large-6 small-12 columns'>
          <div class='flex-video row'>
            <iframe width='420' height='315' src='" . dpsbridge_helper_format_url($videos) . "' frameborder='0' allowfullscreen></iframe>
          </div>
        </div>
      <?php
      endif; ?>
      <?php print $paragraphs; ?>
      </div>
    </div>
  </div>
  <?php print drupal_render($ds_content); ?>
  <script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>
  <script type='text/javascript'>$(document).foundation('orbit').init();</script>
</body>
</html>
