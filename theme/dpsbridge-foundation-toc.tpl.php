<?php
/**
 * @file
 * Template for foundation toc page.
 */
?>

<!doctype html>
<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->
<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->
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
  <div id='kicker'>Foundation</div>
  <div id='display-home' class='row'>
    <div class='large-12 small-12 columns'>
      <div class='large-12 small-12 columns'>
      <h1 class='title'>Table of Contents</h1>
      </div>
      <div class='large-12 small-12 columns'>
      <?php
      for ($i = 1, $n = 1; $i < count($article_names); $i++):
        if ($ads_list[$i - 1] == TRUE):
          continue;
        endif; ?>
        <div class='large-12 small-12 columns'>
          <h4><a class='bluebg' href='navto://<?php print dpsbridge_helper_format_title($article_names[$i]); ?>'><?php print $n ?> ) <?php print $article_names[$i]; ?></a></h4>
        </div>
      <?php
        $n++;
      endfor; ?>
      <?php print drupal_render($ds_articles); ?>
      </div>
    </div>
  </div>
  <script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>
  <script type='text/javascript'>$(document).foundation('orbit').init();</script>
</body>
</html>
