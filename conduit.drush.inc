<?php
/**
 * @file
 * Provide drush commands.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implements hook_drush_command().
 */
function conduit_drush_command() {
  $items = array();

  $items['conduit-basic'] = array(
    'description' => dt('Generate a basic setup -- one group and one job.'),
    'arguments' => array(
      'module' => dt('Module providing conduit plugin -- conduit_execute, drupal_coder, drupal_plumber.'),
    ),
    'aliases' => array('cb'),
  );
  $items['conduit-complex'] = array(
    'description' => dt('Generate a complex setup.'),
    'arguments' => array(
      'module' => dt('Module providing conduit plugin -- conduit_execute, drupal_coder, drupal_plumber.'),
    ),
  );
  $items['conduit-group'] = array(
    'description' => dt('Create a conduit group.'),
    'arguments' => array(
      'title' => dt('Title of group.'),
      'properties' => dt('Valid PHP syntax for array excluding the outermost array( ... ) wrapper.'),
      'parent' => dt('NID of parent group.'),
      'owner' => dt('UID of client user that owns the group.'),
    ),
    'examples' => array(
      'ccg "Group 1"' => 'Create a group with the title "Group 1."',
      'ccg "Group 1" "\'foo\' => \'bar\'"' => 'Create a group with the title "Group 1", and property of "foo" with value "bar."',
    ),
    'aliases' => array('ccg'),
  );
  $items['conduit-job'] = array(
    'description' => dt('Create a conduit job.'),
    'arguments' => array(
      'title' => dt('Title of job.'),
      'parent' => dt('NID of parent group.'),
      'module' => dt('Module providing conduit plugin -- conduit_execute, drupal_coder, drupal_plumber.'),
      'properties' => dt('Valid PHP syntax for array excluding the outermost array( ... ) wrapper.'),
      'file' => 'PHP file containing the job properties array defined as $properties in the [global] namespace.',
      'owner' => dt('UID of client user that owns the job.'),
    ),
    'examples' => array(
      'ccj "Job 1" 17' => 'Create a job for plugin "conduit_execute" with the title "Job 1" and group parent with NID of 17.',
      'ccj "Job 1" 17 "conduit_execute" "\'foo\' => \'bar\'"' => 'Create a job for plugin "conduit_execute" with the title "Job 1", group parent with NID of 17, and property of "foo" with value "bar."',
      'ccj "Job 1" 17 "drupal_plumber" "" "<path/to/properties/file>"' => 'Create a job for plugin "drupal_plumber" with the title "Job 1", group parent with NID of 17, and properties file of "<path/to/properties/file>."',
    ),
    'aliases' => array('ccj'),
  );
  $items['conduit-delete-data'] = array(
    'description' => dt('Delete all conduit group, job and queue data.'),
    'arguments' => array(
      'types' => dt('Space-delimited list of node types to delete -- conduit_group, conduit_job_conduit_execute.'),
    ),
    'aliases' => array('cdd'),
  );

  return $items;
}

/**
 * Generate a basic setup.
 */
function drush_conduit_basic($module = 'conduit_execute') {
  $group = drush_conduit_group('Group 1', "'foo' => 'bar'");
  $job = drush_conduit_job('Job 1', $group->nid, $module, "'bar' => 'baz'");
  return array($group, $job);
}

/**
 * Generate a complex setup.
 */
function drush_conduit_complex($module = 'conduit_execute') {
  for ($i = 0; $i < 20; $i++) {
    $group = drush_conduit_group('Group ' . $i, "'group' => '$i'");
    for ($j = 0; $j < 5; $j++) {
      echo "\t";
      $subgroup = drush_conduit_group('Sub-Group ' . $j, "'sub-group' => '$i.$j'", $group->nid);
      for ($k = 0; $k < 5; $k++) {
        echo "\t\t";
        $job = drush_conduit_job('Job ' . $k, $subgroup->nid, $module, "'job' => '$i.$j.$k'");
      }
    }
  }
}

/**
 * Create a conduit group.
 */
function drush_conduit_group($title = '', $properties = '', $parent = NULL, $owner = 1) {
  $group = new stdClass();
  $group->uid = $owner;
  $group->type = 'conduit_group';
  $group->language = LANGUAGE_NONE;
  $group->title = $title;
  if ($parent) {
    $group->conduit_parent[LANGUAGE_NONE][0]['nid'] = $parent;
  }
  $group->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  node_save($group);
  echo "Created group #{$group->nid}.\n";
  return $group;
}

/**
 * Create a conduit job.
 */
function drush_conduit_job($title = '', $parent = NULL, $module = 'conduit_execute', $properties = '', $file = NULL, $owner = 1) {
  if ($file) {
    $properties = serial_eval($properties);
    // This merge order gives preference to entries in $properties over $file.
    $properties = array_merge($properties, drush_conduit_job_load($file));
    $properties = serial_display($properties, 'edit');
  }
  $job = new stdClass();
  $job->uid = $owner;
  $job->type = 'conduit_job_' . $module;
  $job->language = LANGUAGE_NONE;
  $job->title = $title;
  $job->conduit_parent[LANGUAGE_NONE][0]['nid'] = $parent;
  $job->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  node_save($job);
  echo "Created job #{$job->nid}.\n";
  return $job;
}

/**
 * Load a properties file.
 *
 * @param $file
 *   Path to properties file.
 * @return
 *   Array of properties.
 */
function drush_conduit_job_load($file) {
  $properties = array();
  require $file;
  return $properties;
}

/**
 * Remove all data for conduit node types.
 * 
 * Adapted from conduit_uninstall_node_type() which could stand a
 * refactoring into: deleting the data and deleting the fields/instances.
 *
 * @param $types
 *   String of node types delimited by a space.
 */
function drush_conduit_delete_data($types = '') {
  if ($types) {
    $types = explode(' ', $types);
  }
  else {
    $types = conduit_types();
  }

  foreach ($types as $type => $title) {
    // Delete all nodes of specified type.
    $nids = db_select('node')
      ->fields('node', array('nid'))
      ->condition('type', $type)
      ->execute()
      ->fetchCol();
    if ($nids) {
      node_delete_multiple($nids);
    }

    // Delete the queue for the category.
    $info = conduit_job_info($type);
    $category = $info['category'];
    DrupalQueue::get('conduit_job_' . $category)->deleteQueue();
  }
}
