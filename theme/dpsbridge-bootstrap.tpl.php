<?php
/**
 * @file
 * Template for bootstrap page.
 */
?>
<!doctype html>
<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->
<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->
<head>
  <meta charset='utf-8' />
  <meta name='viewport' content='width=device-width' />
  <title></title>
  <link rel='stylesheet' href='../HTMLResources/css/style.css' />
  <link rel='stylesheet' href='../HTMLResources/css/override.css' />
  <script type='text/javascript'>
  (function() { var config = { kitId: 'sai6pou', scriptTimeout: 3000 };
  var h=document.getElementsByTagName('html')[0];h.className+='wf-loading';var t=setTimeout(function(){h.className=h.className.replace(/(\s|^)wf-loading(\s|$)/g,'');h.className+='wf-inactive'},config.scriptTimeout);var tk=document.createElement('script'),d=false;tk.src='//use.typekit.net/'+config.kitId+'.js';tk.type='text/javascript';tk.async='true';tk.onload=tk.onreadystatechange=function(){var a=this.readyState;if(d||a&&a!='complete'&&a!='loaded')return;d=true;clearTimeout(t);try{Typekit.load(config)}catch(b){}};var s=document.getElementsByTagName('script')[0];s.parentNode.insertBefore(tk,s)
  })();
  </script>
  <script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>
  <script type='text/javascript'>try{Typekit.load();}catch(e){}</script>
</head>
<body>
  <div id='display-home' class='container'>
    <div class='col-lg-12 col-12'>
      <div class='col-lg-12 col-12'>
      <div class='navbar navbar-fixed-top text-center'>
      <h4><?php print $publication; ?></h4>
      <h6><?php print $kicker; ?></h6>
      </div><br/><br/><br/>
      <h1 class='title'><?php print $title; ?></h1>
      <h5>By <?php $author ?></h5>
      </div>
      <div class='col-lg-12 col-12'>
      <?php
        if ($images):
      ?>
        <div id='imageArea' class='col-lg-6 col-12'>
        <?php
        // If there are only one image.
        if (count($images) == 1): ?>
          <img src='" . dpsbridge_helper_linkImg($filename, $images[0]) . "' width='100%' /><br/>
        <?php
        // Creates a slideshow if there are multiple images.
        else: ?>
          <div id='carousel-generic' class='carousel slide'>
          <ol class='carousel-indicators'>
          <!-- TODO: Use jQuery to assign default class=active to first li -->
          <?php for ($q = 0; $q < count($images); $q++): ?>
            <li data-target='#carousel-generic' data-slide-to='<?php print $q; ?>' class=''></li>
          <?php endfor; ?>
          </ol>
          <div class='carousel-inner'>
            <?php for ($q = 0; $q < count($images); $q++): ?>
            <!-- TODO: Use jQuery to add class=active to first div -->
            <div class='item'>
              <img src='<?php print dpsbridge_helper_linkImg($filename, $images[$q]); ?>' alt=''>
              <div class='carousel-caption'>Some Caption Here</div>
            </div>
            <?php endfor; ?>
          </div>
          <a class='left carousel-control' href='#carousel-generic' data-slide='prev'><span class='icon-prev'></span></a>
          <a class='right carousel-control' href='#carousel-generic' data-slide='next'><span class='icon-next'></span></a>
          </div>
        <?php
        endif; ?>
        </div>
      <?php
        // If there are embedded videoes.
        elseif ($videos): ?>
          <div id='videoArea' class='col-lg-6 col-12'>
          <div class='flex-video row'>
          <iframe width='420' height='315' src='<?php dpsbridge_helper_format_url($videos); ?>' frameborder='0' allowfullscreen></iframe>
          </div>
          </div>
      <?php
        endif; ?>
        <?php print $paragraphs; ?>
      </div>
    </div>
  </div>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/jquery.js'></script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/bootstrap.min.js'></script>
  <script>$('.carousel').carousel()</script>
</body>
</html>
