<?php
/**
 * @file
 * DPSBridge Style Bootstrap functions.
 */

/**
 * Helper method for creating the HTML cover page with Bootstrap stylesheet.
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
function dpsbridge_helper_style_bootstrap_cover($publication, $landscape, $portrait, $dimension) {
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  $variables = array(
    'publication' => $publication,
    'landscape' => $landscape,
    'portrait' => $portrait,
    'directory' => $directory,
  );
  $html = theme('dpsbridge_bootstrap_cover', $variables);
  // Save the above HTML @ /dpsbridge/html/Cover/Cover.html.
  file_put_contents($directory . 'Cover/Cover.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = dpsbridge_helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . 'Cover/Cover.html');
  dpsbridge_helper_save_img($toc_img, 'Cover/toc.png');
}

/**
 * Helper method to create the HTML TOC page with Bootstrap stylesheet.
 */
function dpsbridge_helper_style_bootstrap_toc($publication, $article_names, $ds_articles) {
  $ads_list = NULL;
  $directory     = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';
  // Generate the contents of the HTML article.
  $variables = array(
    'publication' => $publication,
    'ads_list' => $ads_list,
    'article_names' => $article_names,
    'ds_articles' => $ds_articles,
  );
  $html = theme('dpsbridge_bootstrap_toc', $variables);
  // Save the above HTML @ /dpsbridge/html/TableofContents/TableofContents.html.
  file_put_contents($directory . 'TableofContents/TableofContents.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = dpsbridge_helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . 'TableofContents/TableofContents.html');
  dpsbridge_helper_save_img($toc_img, 'TableofContents/toc.png');
}

/**
 * Helper method for generating the HTML article with Bootstrap stylesheet.
 *
 * @param string $publication
 *   The folio publication name.
 * @param string $title
 *   The article title.
 * @param string $author
 *   The article author.
 * @param string $kicker
 *   The folio kicker.
 * @param string $body
 *   The article content.
 * @param object $images
 *   The article image(s), generate a slideshow if there are more than 1 images.
 * @param string $videos
 *   The article video.
 * @param object $ds_content
 *   A list of Display Suite articles.
 */
function dpsbridge_helper_style_bootstrap($publication, $title, $author, $kicker, $body, $images, $videos, $ds_content) {
  $filename = dpsbridge_helper_format_title($title);
  $paragraphs = isset($body[LANGUAGE_NONE][0]['safe_value']) ? $body[LANGUAGE_NONE][0]['safe_value'] : '';
  $directory = variable_get('file_public_path', conf_path() . '/files') . '/dpsbridge' . '/html/';

  $variables = array(
    'ds_content' => $ds_content,
    'publication' => $publication,
    'kicker' => $kicker,
    'title' => $title,
    'author' => $author,
    'images' => $images,
    'filename' => $filename,
    'videos' => $videos,
    'paragraphs' => $paragraphs,
  );
  $html = theme('dpsbridge_bootstrap', $variables);
  // Save the above HTML @ /dpsbridge/html/[article name]/[article name].html.
  file_put_contents($directory . $filename . '/' . $filename . '.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = dpsbridge_helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . $filename . '/' . $filename . '.html');
  dpsbridge_helper_save_img($toc_img, $filename . '/toc.png');
}
