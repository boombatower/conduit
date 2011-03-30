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

/**
 * Create revision view from a non-revision field view.
 *
 * @param $view
 *   A non-revision field view.
 * @return
 *   A review view created from the passed view.
 */
function conduit_views_revision($view) {
  $view = $view->clone_view();

  // Change base information.
  $view->name .= '_revision';
  $view->base_table = conduit_views_revision_table($view->base_table);
  $view->human_name .= ' (revision)';

  $display = $view->display['default'];

  // Change fields and a arguments table.
  foreach ($display->display_options['fields'] as &$field) {
    $field['table'] = conduit_views_revision_table($field['table']);
  }
  foreach ($display->display_options['arguments'] as &$argument) {
    $argument['table'] = conduit_views_revision_table($argument['table']);
  }

  // Convert entity_id_raw argument to entity_id.
  $display->display_options['arguments']['entity_id'] = $display->display_options['arguments']['entity_id_raw'];
  $entity_id = &$display->display_options['arguments']['entity_id'];
  $entity_id['id'] = $entity_id['field'] = 'entity_id';

  // Add revision ID argument.
  $display->display_options['arguments']['revision_id_raw'] = $display->display_options['arguments']['entity_id_raw'];
  $revision_id = &$display->display_options['arguments']['revision_id_raw'];
  $revision_id['id'] = $revision_id['field'] = 'revision_id_raw';
  $revision_id['validate']['type'] = 'numeric';
  unset($revision_id['validate_options']);

  unset($display->display_options['arguments']['entity_id_raw']);

  // Convert the page display path.
  $display = $view->display['page'];
  $display->display_options['path'] = str_replace('node/%', 'node/%/revisions/%', $display->display_options['path']);

  return $view;
}

/**
 * Convert field_data table to field_revision table.
 *
 * @param $table
 *   A field_data table.
 * @return
 *   A field_revision table converted from the passed table.
 */
function conduit_views_revision_table($table) {
  return str_replace('field_data', 'field_revision', $table);
}
