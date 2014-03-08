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
  $html = '';
  $variables = array(
    'publication' => $publication,
    'landscape' => $landscape,
    'portrait' => $portrait,
    'directory' => $directory,
  );
  $html = theme('dpsbridge_foundation_cover', $variables);
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
  // Generate the contents of the HTML article.
  $variables = array(
    'publication' => $publication,
    'ads_list' => $ads_list,
    'article_names' => $article_names,
  );
  $html = theme('dpsbridge_foundation_toc', $variables);
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
function helper_style_foundation($publication, $title, $author, $kicker, $body, $images, $videos, $ds_content) {
  $slideshow = "";
  $filename = helper_format_title($title);
  $paragraphs = isset($body['und'][0]['safe_value']) ? $body['und'][0]['safe_value'] : '';
  $directory = drupal_get_path('module', 'dpsbridge') . '/html/';
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
  $html = theme('dpsbridge_foundation', $variables);
  // Save the above HTML @ /dpsbridge/html/[article name]/[article name].html.
  $result = file_put_contents($directory . $filename . '/' . $filename . '.html', $html);
  // Take a snapshot of the webpage.
  $toc_img = helper_format_thumbnail($GLOBALS['base_url'] . '/' . $directory . $filename . '/' . $filename . '.html');
  helper_save_img($toc_img, $filename . '/toc.png');
}
