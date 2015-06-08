<?php
/**
 * @file
 * Template for bootstrap cover page.
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
      <h6>Cover</h6>
    </div>
    <h1 class='title'>Table of Contents</h1>
    </div>
    <div class='col-lg-12 col-12'>
    <?php
    // If a landscape image was provided.
    if ($landscape):
      $landscape_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $landscape);
      file_put_contents($directory . '/html/Cover/landscape.png', $landscape_img);?>
      <div class='visible-lg'><img src='landscape.png' width='100%' /></div>
    <?php
    else:?>
      <div class='visible-lg'><img src='portrait.png' width='100%' /></div>
    <?php
    endif;
    // If a portrait image was provided.
    if ($portrait):
      $portrait_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $portrait);
      file_put_contents($directory . '/html/Cover/portrait.png', $portrait_img);?>
      <div class='hidden-lg'><img src='portrait.png' width='100%' /></div>
    <?php
    else:?>
      <div class='hidden-lg'><img src='landscape.png' width='100%' /></div>
    <?php
    endif;?>
    </div>
  </div>
  </div>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/jquery.js'></script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/bootstrap.min.js'></script>
</body>
</html>
