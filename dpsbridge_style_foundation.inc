<?php

/**
 * @file
 * DPSBridge Style Foundation functions. 
 */

/**
 * Helper method for creating the HTML cover page with Foundation stylesheet.
 * 
 * @param string $publication
 *   The folio publication name.
 * @param string $landscape
 *   The landscape image URL (@ /dpsbridge/landscape).
 * @param string $portrait
 *   The portrait image URL (@ /dpsbridge/portrait).
 * @param string $dimension
 *   The targeted folio dimension.
 */
function helper_style_foundation_cover($publication, $landscape, $portrait, $dimension) {
  $directory = drupal_get_path('module', 'dpsbridge') . '/html/';
  /*$split     = explode(" x ", $dimension);*/
  /*$width     = $split[0] . 'px';*/
  /*$height    = $split[1] . 'px';*/
  $content   = "";
  // If a landscape image was provided.
  if ($landscape) {
    $landscape_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $landscape);
    file_put_contents($directory . 'Cover/landscape.png', $landscape_img);
    $content .= "<div class='show-for-landscape'><img src='landscape.png' width='100%' /></div>\n";
  }
  else {
    $content .= "<div class='show-for-landscape'><img src='portrait.png' width='100%' /></div>\n";
  }
  // If a portrait image was provided.
  if ($portrait) {
    $portrait_img = file_get_contents('http://' . $_SERVER['SERVER_NAME'] . $portrait);
    file_put_contents($directory . 'Cover/portrait.png', $portrait_img);
    $content .= "<div class='show-for-portrait'><img src='portrait.png' width='100%' /></div>\n";
  }
  else {
    $content .= "<div class='show-for-portrait'><img src='landscape.png' width='100%' /></div>\n";
  }
  // Generate the contents of the HTML article.
  $html  = "<!doctype html>\n";
  $html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
  $html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
  $html .= "<head>\n";
  $html .= "<meta charset='utf-8' />\n";
  $html .= "<meta name='viewport' content='width=device-width' />\n";
  $html .= "<title></title>\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/normalize.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
  $html .= "<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
  $html .= "<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
  $html .= "<script type='text/javascript' src='../HTMLResources/js/vendor/custom.modernizr.js'></script>\n";
  $html .= "</head>\n";
  $html .= "<body>\n";
  $html .= "<div id='pubtitle'>" . $publication . "</div>\n";
  $html .= "<div id='kicker'>Cover</div>\n";
  $html .= "<div id='display-home' class='row'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= $content;
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "<script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>\n";
  $html .= "<script type='text/javascript'>$(document).foundation('orbit').init();</script>\n";
  $html .= "</body>\n";
  $html .= "</html>\n";
  // Save the above HTML @ /dpsbridge/html/Cover/Cover.html.
  $result = file_put_contents($directory . 'Cover/Cover.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . 'Cover/Cover.html');
  helper_save_img($toc_img, 'Cover/toc.png');
}
/**
 * Helper method to create the HTML TOC page with Foundation stylesheet.
 * 
 * @param string $publication
 *   The folio publication name
 * @param array $article_names
 *   A list of article names, used to generate the nav-to links.
 */
function helper_style_foundation_toc($publication, $article_names) {
  $directory     = drupal_get_path('module', 'dpsbridge') . '/html/';
  $article_links = "";
  // Generate the navigation links.
  for ($i = 1, $n = 1; $i < count($article_names); $i++) {
    if ($ads_list[$i - 1] == TRUE) {
      continue;
    }
    $article_links .= "<div class='large-12 small-12 columns'>\n";
    $article_links .= "<h4><a class='bluebg' href='navto://" . helper_format_title($article_names[$i]) . "'>" . $n . ") " . $article_names[$i] . "</a></h4>\n";
    $article_links .= "</div><br/>\n";
    $n++;
  }
  // Generate the contents of the HTML article.
  $html  = "<!doctype html>\n";
  $html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
  $html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
  $html .= "<head>\n";
  $html .= "<meta charset='utf-8' />\n";
  $html .= "<meta name='viewport' content='width=device-width' />\n";
  $html .= "<title></title>\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/normalize.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
  $html .= "<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
  $html .= "<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
  $html .= "<script type='text/javascript' src='../HTMLResources/js/vendor/custom.modernizr.js'></script>\n";
  $html .= "</head>\n";
  $html .= "<body>\n";
  $html .= "<div id='pubtitle'>" . $publication . "</div>\n";
  $html .= "<div id='kicker'>Foundation</div>\n";
  $html .= "<div id='display-home' class='row'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= "<h1 class='title'>Table of Contents</h1>\n";
  $html .= "</div>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= $article_links;
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "<script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>\n";
  $html .= "<script type='text/javascript'>$(document).foundation('orbit').init();</script>\n";
  $html .= "</body>\n";
  $html .= "</html>\n";
  // Save the above HTML @ /dpsbridge/html/TableofContents/TableofContents.html.
  $result = file_put_contents($directory . 'TableofContents/TableofContents.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . 'TableofContents/TableofContents.html');
  helper_save_img($toc_img, 'TableofContents/toc.png');
}
/**
 * Helper method for generating the HTML article with Foundation stylesheet.
 * 
 * @param string $publication
 *   The folio publication name
 * @param string $title
 *   The article title
 * @param string $author
 *   The article author
 * @param string $kicker
 *   The folio kicker
 * @param string $body
 *   The article content
 * @param array $images
 *   The article image(s), generating a slideshow if more than 1 images.
 * @param string $videos
 *   The article video
 */
function helper_style_foundation($publication, $title, $author, $kicker, $body, $images, $videos) {
  $slideshow = "";
  $filename = helper_format_title($title);
  $paragraphs = isset($body['und'][0]['safe_value']) ? $body['und'][0]['safe_value'] : '';
  $directory = drupal_get_path('module', 'dpsbridge') . '/html/';
  // If there are images.
  if ($images) {
    $slideshow .= "<div id='imageArea' class='large-6 small-12 columns'>\n";
    $slideshow .= "<div class='row'>\n";
    // If there are only one image.
    if (count($images) == 1) {
      $slideshow .= "<img src='" . helper_link_img($filename, $images[0]) . "' width='100%' /><br/>\n";
    }
    // Creates a slideshow if there are multiple images.
    else {
      $slideshow .= "<ul data-orbit>\n";
      for ($q = 0; $q < count($images); $q++) {
        $slideshow .= "<li>\n";
        $slideshow .= "<img src='" . helper_link_img($filename, $images[$q]) . "' />\n";
        $slideshow .= "<div class='orbit-caption'>Some caption here</div>\n";
        $slideshow .= "</li>\n";
      }
      $slideshow .= "</ul>\n";
    }
    $slideshow .= "</div>\n";
    $slideshow .= "</div>\n";
  }
  // If there are embedded videoes.
  elseif ($videos) {
    $slideshow  = "<div id='videoArea' class='large-6 small-12 columns'>\n";
    $slideshow .= "<div class='flex-video row'>\n";
    $slideshow .= "<iframe width='420' height='315' src='" . helper_format_url($videos) . "' frameborder='0' allowfullscreen></iframe>\n";
    $slideshow .= "</div>\n";
    $slideshow .= "</div>\n";
  }
  // Generate the contents of the HTML article.
  $html  = "<!doctype html>\n";
  $html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
  $html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
  $html .= "<head>\n";
  $html .= "<meta charset='utf-8' />\n";
  $html .= "<meta name='viewport' content='width=device-width' />\n";
  $html .= "<title></title>\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/normalize.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
  $html .= "<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
  $html .= "<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
  $html .= "<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
  $html .= "<script type='text/javascript' src='../HTMLResources/js/vendor/custom.modernizr.js'></script>\n";
  $html .= "</head>\n";
  $html .= "<body>\n";
  $html .= "<div id='pubtitle'>" . $publication . "</div>\n";
  $html .= "<div id='kicker'>" . $kicker . "</div>\n";
  $html .= "<div id='display-home' class='row'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= "<h1 class='title'>" . $title . "</h1>\n";
  $html .= "<h5>By " . $author . "</h5>\n";
  $html .= "</div>\n";
  $html .= "<div class='large-12 small-12 columns'>\n";
  $html .= $slideshow;
  $html .= $paragraphs;
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "</div>\n";
  $html .= "<script type='text/javascript'>document.write('<script src=' + ('__proto__' in {} ? '../HTMLResources/js/vendor/zepto' : '../HTMLResources/js/vendor/jquery') + '.js><\/script>')</script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.js'></script>\n";
  $html .= "<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/foundation.orbit.js'></script>\n";
  $html .= "<script type='text/javascript'>$(document).foundation('orbit').init();</script>\n";
  $html .= "</body>\n";
  $html .= "</html>\n";
  // Save the above HTML @ /dpsbridge/html/[article name]/[article name].html.
  $result = file_put_contents($directory . $filename . '/' . $filename . '.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . $filename . '/' . $filename . '.html');
  helper_save_img($toc_img, $filename . '/toc.png');
}
