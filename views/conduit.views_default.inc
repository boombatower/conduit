<?php
/**
 * @file
 * Provides default information to views.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_views_default_views().
 */
function conduit_views_default_views() {
  // If views have been loaded then return them.
  if ($views = &drupal_static(__FUNCTION__, array())) {
    return $views;
  }

  // Load default view information from include files.
  $files = file_scan_directory(drupal_get_path('module', 'conduit') . '/views/default', '/\.inc$/');
  foreach ($files as $file_path => $file) {
    include $file_path;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  return $views;
}
