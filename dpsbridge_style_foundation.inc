<?php

/**
 * @file
 * DPSBridge Style Foundation functions.
 */

/**
 * Helper method for generating the HTML article with Foundation stylesheet.
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
 *   The article image(s), generating a slideshow if more than 1 images.
 * @param string $videos
 *   The article video.
 * @param string $ds_content
 *   A list of Display Suite content.
 */
function dpsbridge_helper_next_style_foundation($publication, $title, $author, $kicker, $body, $images, $videos, $ds_content) {
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
  $html = theme('dpsbridge_foundation_next', $variables);
  // Save the above HTML @ /dpsbridge/html/[article name]/index.html.
  file_put_contents($directory . $filename . '/index.html', $html);
}