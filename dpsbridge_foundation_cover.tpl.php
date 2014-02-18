<?php
/**
 * @file
 * Template for NYCD register confirmation.
 *
 * - $account: The current use account.
 * - $project: The orientation project signed up.
 * - $project_content: content to print on the page.
 * - $user_name
 * - $project_title
 * - $project_directions
 * - $project_date
 * - $project_time
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
  <div id='kicker'>Cover</div>
  <div id='display-home' class='row'>
    <div class='large-12 small-12 columns'>
      <div class='large-12 small-12 columns'>
      // If a landscape image was provided.
      if ($landscape) {
        $landscape_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $landscape);
        file_put_contents($directory . 'Cover/landscape.png', $landscape_img);
        $content .= "<div class='show-for-landscape'><img src='landscape.png' width='100%' /></div>
      }
      else {
        $content .= "<div class='show-for-landscape'><img src='portrait.png' width='100%' /></div>
      }
      // If a portrait image was provided.
      if ($portrait) {
        $portrait_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $portrait);
        file_put_contents($directory . 'Cover/portrait.png', $portrait_img);
        $content .= "<div class='show-for-portrait'><img src='portrait.png' width='100%' /></div>
      }
      else {
        $content .= "<div class='show-for-portrait'><img src='landscape.png' width='100%' /></div>
      }
      // Generate the contents of the HTML article.


        $html .= $content;
      </div>
    </div>
  </div>
  <script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>
  <script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>
  <script type='text/javascript'>$(document).foundation('orbit').init();</script>
</body>
</html>