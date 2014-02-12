<?php

/**
 * Helper method for creating the HTML cover page with Bootstrap stylesheet.
 * @param string $publication ..the folio publication name
 * @param string $landscape ....the landscape image URL (@ /adobe/landscape)
 * @param string $portrait .....the portrait image URL (@ /adobe/portrait)
 * @param string $dimension ....the targeted folio dimension
 */
function helper_style_bootstrap_cover($publication, $landscape, $portrait, $dimension) {
	$directory = drupal_get_path('module', 'adobe').'/html/';
	//$split     = explode(" x ", $dimension);
	//$width     = $split[0].'px';
	//$height    = $split[1].'px';
	$content   = "";
	// if a landscape image was provided
	if ($landscape) {
		$landscape_img = file_get_contents('http://'.$_SERVER['SERVER_NAME'].$landscape);
		file_put_contents($directory.'Cover/landscape.png', $landscape_img);
		$content .= "<div class='visible-lg'><img src='landscape.png' width='100%' /></div>\n";
	} else {
		$content .= "<div class='visible-lg'><img src='portrait.png' width='100%' /></div>\n";
	}
	// if a portrait image was provided
	if ($portrait) {
		$portrait_img = file_get_contents('http://'.$_SERVER['SERVER_NAME'].$portrait);
		file_put_contents($directory.'Cover/portrait.png', $portrait_img);
		$content .= "<div class='hidden-lg'><img src='portrait.png' width='100%' /></div>\n";
	} else {
		$content .= "<div class='hidden-lg'><img src='landscape.png' width='100%' /></div>\n";
	}
	// generate the contents of the HTML article
	$html  =  "<!doctype html>\n";
	$html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
	$html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
	$html .= "<head>\n";
	$html .= 	"<meta charset='utf-8' />\n";
	$html .= 	"<meta name='viewport' content='width=device-width' />\n";
	$html .= 	"<title></title>\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
	$html .=	"<script type='text/javascript'>\n";
	$html .=		"(function() { var config = { kitId: 'sai6pou', scriptTimeout: 3000 };\n";
	$html .=			"var h=document.getElementsByTagName('html')[0];h.className+='wf-loading';var t=setTimeout(function(){h.className=h.className.replace(/(\s|^)wf-loading(\s|$)/g,'');h.className+='wf-inactive'},config.scriptTimeout);var tk=document.createElement('script'),d=false;tk.src='//use.typekit.net/'+config.kitId+'.js';tk.type='text/javascript';tk.async='true';tk.onload=tk.onreadystatechange=function(){var a=this.readyState;if(d||a&&a!='complete'&&a!='loaded')return;d=true;clearTimeout(t);try{Typekit.load(config)}catch(b){}};var s=document.getElementsByTagName('script')[0];s.parentNode.insertBefore(tk,s)\n";
	$html .=		"})();\n";
	$html .=	"</script>\n";
	$html .= 	"<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
	$html .= 	"<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
	$html .= "</head>\n";
	$html .= "<body>\n";
	$html .= 	"<div id='display-home' class='container'>\n";
	$html .= 	"<div class='col-lg-12 col-12'>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .=			"<div class='navbar navbar-fixed-top text-center'>\n";
	$html .= 				"<h4>".$publication."</h4>\n";
	$html .= 				"<h6>Cover</h6>\n";
	$html .=			"</div>\n";
	$html .=			"<h1 class='title'>Table of Contents</h1>\n";
	$html .=		"</div>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .= 			$content;
	$html .=		"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/jquery.js'></script>\n";
	$html .=	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/bootstrap.min.js'></script>\n";
	$html .= "</body>\n";
	$html .= "</html>\n";
	// save the above HTML @ /adobe/html/Cover/Cover.html
	file_put_contents($directory.'Cover/Cover.html', $html);
	// take a snapshot of the webpage
	$tocImg = helper_format_thumbnail($GLOBALS['base_url'].'/'.$directory.'Cover/Cover.html');
	helper_save_img($tocImg, 'Cover/toc.png');
}
/**
 * Helper method for creating the HTML table of contents page with Bootstrap stylesheet.
 * @param string $publication ....the folio publication name
 * @param array  $article_names ..a list of article names, used to generate the nav-to links
 */
function helper_style_bootstrap_toc($publication, $article_names) {
	$directory     = drupal_get_path('module', 'adobe').'/html/';
	$article_links = "";
	// generate the navigation links
	for ($i = 1, $n = 1; $i < sizeof($article_names); $i++) {
		if ($ads_list[$i-1] == true)
			continue;
		$article_links .= "<div class='col-lg-12 col-12'>\n";
		$article_links .=	"<h4><a class='bluebg' href='navto://".helper_format_title($article_names[$i])."'>".$n.") ".$article_names[$i]."</a></h4>\n";
		$article_links .= "</div><br/>\n";
		$n++;
	}
	// generate the contents of the HTML article
	$html  =  "<!doctype html>\n";
	$html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
	$html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
	$html .= "<head>\n";
	$html .= 	"<meta charset='utf-8' />\n";
	$html .= 	"<meta name='viewport' content='width=device-width' />\n";
	$html .= 	"<title></title>\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
	$html .=	"<script type='text/javascript'>\n";
	$html .=		"(function() { var config = { kitId: 'sai6pou', scriptTimeout: 3000 };\n";
	$html .=			"var h=document.getElementsByTagName('html')[0];h.className+='wf-loading';var t=setTimeout(function(){h.className=h.className.replace(/(\s|^)wf-loading(\s|$)/g,'');h.className+='wf-inactive'},config.scriptTimeout);var tk=document.createElement('script'),d=false;tk.src='//use.typekit.net/'+config.kitId+'.js';tk.type='text/javascript';tk.async='true';tk.onload=tk.onreadystatechange=function(){var a=this.readyState;if(d||a&&a!='complete'&&a!='loaded')return;d=true;clearTimeout(t);try{Typekit.load(config)}catch(b){}};var s=document.getElementsByTagName('script')[0];s.parentNode.insertBefore(tk,s)\n";
	$html .=		"})();\n";
	$html .=	"</script>\n";
	$html .= 	"<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
	$html .= 	"<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
	$html .= "</head>\n";
	$html .= "<body>\n";
	$html .= 	"<div id='display-home' class='container'>\n";
	$html .= 	"<div class='col-lg-12 col-12'>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .=			"<div class='navbar navbar-fixed-top text-center'>\n";
	$html .= 				"<h4>".$publication."</h4>\n";
	$html .= 				"<h6>Foundation</h6>\n";
	$html .=			"</div>\n";
	$html .=			"<h1 class='title'>Table of Contents</h1>\n";
	$html .=		"</div>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .= 			$article_links;
	$html .=		"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/jquery.js'></script>\n";
	$html .=	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/bootstrap.min.js'></script>\n";
	$html .= "</body>\n";
	$html .= "</html>\n";
	// save the above HTML @ /adobe/html/TableofContents/TableofContents.html
	$result = file_put_contents($directory.'TableofContents/TableofContents.html', $html);
	// take a snapshot of the webpage
	$tocImg = helper_format_thumbnail($GLOBALS['base_url'].'/'.$directory.'TableofContents/TableofContents.html');
	helper_save_img($tocImg, 'TableofContents/toc.png');
}
/**
 * Helper method for generating the HTML article with Bootstrap stylesheet.
 * @param string $publication ..the folio publication name
 * @param string $title ........the article title
 * @param string $author .......the article author
 * @param string $kicker .......the folio kicker
 * @param string $body .........the article content
 * @param array  $images .......the article image(s), generating a slideshow if there are more than 1 image
 * @param string $videos .......the article video
 */
function helper_style_bootstrap($publication, $title, $author, $kicker, $body, $images, $videos) {
	$slideshow 	 = "";
	$filename    = helper_format_title($title);
	$paragraphs  = isset($body['und'][0]['safe_value'])?$body['und'][0]['safe_value']:'';
	$directory   = drupal_get_path('module', 'adobe').'/html/';
	// if there are images
	if ($images) {
		$slideshow .= "<div id='imageArea' class='col-lg-6 col-12'>\n";
		// if there are only one image
		if (sizeof($images) == 1) { 
			$slideshow .= "<img src='".helper_linkImg($filename, $images[0])."' width='100%' /><br/>\n";
		} else { // creates a slideshow if there are multiple images
			$slideshow .= "<div id='carousel-generic' class='carousel slide'>\n";
			$slideshow .= "<ol class='carousel-indicators'>\n";
			$wrapper    = "<div class='carousel-inner'>\n";
			$active     = "active";
			for ($q = 0; $q < sizeof($images); $q++) {
				// build the indicator
				$slideshow .= "<li data-target='#carousel-generic' data-slide-to='".$q."' class='".$active."'></li>\n";
				// build the wrapper
				$wrapper .= "<div class='item ".$active."'>\n";
				$wrapper .= 	"<img src='".helper_linkImg($filename, $images[$q])."' alt=''>\n";
				$wrapper .= 	"<div class='carousel-caption'>Some Caption Here</div>\n";
				$wrapper .= "</div>\n";
				$active   = "";
			}
			$wrapper   .= "</div>\n";
			$slideshow .= "</ol>\n";
			$slideshow .= $wrapper;
			$slideshow .= "<a class='left carousel-control' href='#carousel-generic' data-slide='prev'><span class='icon-prev'></span></a>\n";
			$slideshow .= "<a class='right carousel-control' href='#carousel-generic' data-slide='next'><span class='icon-next'></span></a>\n";
			$slideshow .= "</div>\n";
		}
		$slideshow .= "</div>\n";
	} 
	// if there are embedded videoes
	else if ($videos) {
		$slideshow  = "<div id='videoArea' class='col-lg-6 col-12'>\n";
		$slideshow .= 	"<div class='flex-video row'>\n";
		$slideshow .= 		"<iframe width='420' height='315' src='".helper_format_URL($videos)."' frameborder='0' allowfullscreen></iframe>\n";
		$slideshow .= 	"</div>\n";
		$slideshow .= "</div>\n";
	}
	// generate the contents of the HTML article
	$html  =  "<!doctype html>\n";
	$html .= "<!--[if IE 8]><html class='no-js lt-ie9' lang='en'><![endif]-->\n";
	$html .= "<!--[if gt IE 8]><!--><html class='no-js' lang='en'><!--<![endif]-->\n";
	$html .= "<head>\n";
	$html .= 	"<meta charset='utf-8' />\n";
	$html .= 	"<meta name='viewport' content='width=device-width' />\n";
	$html .= 	"<title></title>\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/style.css' />\n";
	$html .= 	"<link rel='stylesheet' href='../HTMLResources/css/override.css' />\n";
	$html .=	"<script type='text/javascript'>\n";
	$html .=		"(function() { var config = { kitId: 'sai6pou', scriptTimeout: 3000 };\n";
	$html .=			"var h=document.getElementsByTagName('html')[0];h.className+='wf-loading';var t=setTimeout(function(){h.className=h.className.replace(/(\s|^)wf-loading(\s|$)/g,'');h.className+='wf-inactive'},config.scriptTimeout);var tk=document.createElement('script'),d=false;tk.src='//use.typekit.net/'+config.kitId+'.js';tk.type='text/javascript';tk.async='true';tk.onload=tk.onreadystatechange=function(){var a=this.readyState;if(d||a&&a!='complete'&&a!='loaded')return;d=true;clearTimeout(t);try{Typekit.load(config)}catch(b){}};var s=document.getElementsByTagName('script')[0];s.parentNode.insertBefore(tk,s)\n";
	$html .=		"})();\n";
	$html .=	"</script>\n";
	$html .= 	"<script type='text/javascript' src='//use.typekit.net/sai6pou.js'></script>\n";
	$html .= 	"<script type='text/javascript'>try{Typekit.load();}catch(e){}</script>\n";
	$html .= "</head>\n";
	$html .= "<body>\n";
	$html .= 	"<div id='display-home' class='container'>\n";
	$html .= 	"<div class='col-lg-12 col-12'>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .=			"<div class='navbar navbar-fixed-top text-center'>\n";
	$html .= 				"<h4>".$publication."</h4>\n";
	$html .= 				"<h6>".$kicker."</h6>\n";
	$html .=			"</div><br/><br/><br/>\n";
	$html .=			"<h1 class='title'>".$title."</h1>\n";
	$html .=			"<h5>By ".$author."</h5>\n";
	$html .=		"</div>\n";
	$html .=		"<div class='col-lg-12 col-12'>\n";
	$html .= 			$slideshow;
	$html .= 			$paragraphs;
	$html .=		"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"</div>\n";
	$html .= 	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/jquery.js'></script>\n";
	$html .=	"<script type='text/javascript' type='text/javascript' src='../HTMLResources/js/bootstrap.min.js'></script>\n";
	$html .= 	"<script>$('.carousel').carousel()</script>\n";
	$html .= "</body>\n";
	$html .= "</html>\n";
	// save the above HTML @ /adobe/html/[article name]/[article name].html
	$result = file_put_contents($directory.$filename.'/'.$filename.'.html', $html);
	// take a snapshot of the webpage
	$tocImg = helper_format_thumbnail($GLOBALS['base_url'].'/'.$directory.$filename.'/'.$filename.'.html');
	helper_save_img($tocImg, $filename.'/toc.png');
}

?>

