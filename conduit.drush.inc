<?php
/**
 * @file
 * Provide drush commands.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_drush_command().
 */
function conduit_drush_command() {
  $items = array();

  $items['conduit-basic'] = array(
    'description' => dt('Generate a basic setup.'),
    'aliases' => array('cb'),
  );
  $items['conduit-complex'] = array(
    'description' => dt('Generate a complex setup.'),
  );
  $items['conduit-group'] = array(
    'description' => dt('Create a conduit group.'),
    'arguments' => array(
      'title' => dt('Title of group.'),
      'owner' => dt('NID of client that owns the group.'),
      'properties' => dt('Valid PHP syntax for array excluding the array( ... ) wrapper.'),
      'parent' => dt('NID of parent group or client.'),
    ),
    'examples' => array(
      'ccg "Group 1" 17' => 'Create a group with the title "Group 1" and owner with NID of 17.',
      'ccg "Group 1" 17 "\'foo\' => \'bar\'"' => 'Create a group with the title "Group 1", owner with NID of 17, and property of "foo" with value "bar".',
    ),
    'aliases' => array('ccg'),
  );
  $items['conduit-job'] = array(
    'description' => dt('Create a conduit job.'),
    'arguments' => array(
      'module' => dt('Module providing conduit plugin.'),
      'title' => dt('Title of group.'),
      'parent' => dt('NID of parent group or client.'),
      'properties' => dt('Valid PHP syntax for array excluding the array( ... ) wrapper.'),
    ),
    'examples' => array(
      'ccj "Job 1" 17' => 'Create a job with the title "Job 1" and group parent with NID of 17.',
      'ccj "Job 1" 17 "\'foo\' => \'bar\'"' => 'Create a job with the title "Job 1", group parent with NID of 17, and property of "foo" with value "bar".',
    ),
    'aliases' => array('ccj'),
  );

  return $items;
}

/**
 * Generate a basic setup.
 */
function drush_conduit_basic() {
  $group = drush_conduit_group('Group 1', "'foo' => 'bar'");
  $job = drush_conduit_job('conduit_execute', 'Job 1', $group->nid, "'bar' => 'baz'");
  return array($group, $job);
}

/**
 * Generate a complex setup.
 */
function drush_conduit_complex() {
  for ($i = 0; $i < 20; $i++) {
    $group = drush_conduit_group('Group ' . $i, "'group' => '$i'");
    for ($j = 0; $j < 5; $j++) {
      echo "\t";
      $subgroup = drush_conduit_group('Sub-Group ' . $j, "'sub-group' => '$i.$j'", $group->nid);
      for ($k = 0; $k < 5; $k++) {
        echo "\t\t";
        $job = drush_conduit_job('conduit_execute', 'Job ' . $k, $subgroup->nid, "'job' => '$i.$j.$k'");
      }
    }
  }
}

/**
 * Create a conduit group.
 */
function drush_conduit_group($title = '', $properties = '', $parent = NULL) {
  $group = new stdClass();
  $group->uid = 1;
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
function drush_conduit_job($module = 'conduit_execute', $title = '', $parent = NULL, $properties = '') {
  $job = new stdClass();
  $job->uid = 1;
  $job->type = 'conduit_job_' . $module;
  $job->language = LANGUAGE_NONE;
  $job->title = $title;
  $job->conduit_parent[LANGUAGE_NONE][0]['nid'] = $parent;
  $job->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  node_save($job);
  echo "Created job #{$job->nid}.\n";
  return $job;
}
