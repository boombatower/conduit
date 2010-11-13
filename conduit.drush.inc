<?php

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
  $items['conduit-client'] = array(
    'description' => dt('Create a conduit client.'),
    'arguments' => array(
      'title' => dt('Title of client.'),
      'properties' => dt('Valid PHP syntax for array excluding the array( ... ) wrapper.'),
      'key' => dt('API key to be used by client.'),
    ),
    'examples' => array(
      'ccc "Client 1"' => 'Create a client with the title "Client 1".',
      'ccc "Client 1" "3a7230c144e0615403eb75f71d44c21f"' => 'Create a client with the title "Client 1" and key "3a7230c144e0615403eb75f71d44c21f".',
    ),
    'aliases' => array('ccc'),
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
  $client = drush_conduit_client('Client 1',  "'client' => 'root'");
  $group = drush_conduit_group('Group 1', $client->nid, "'foo' => 'bar'", $client->nid);
  $job = drush_conduit_job('Job 1', $group->nid, "'plugin' => 'conduit_execute', 'bar' => 'baz'");
  return array($client, $group, $job);
}

/**
 * Generate a complex setup.
 */
function drush_conduit_complex() {
  $client = drush_conduit_client('Client 1', "'client' => 'root'");
  for ($i = 0; $i < 20; $i++) {
    echo "\t";
    $group = drush_conduit_group('Group ' . $i, $client->nid, "'group' => '{$client->nid}'", $client->nid);
    for ($j = 0; $j < 5; $j++) {
      echo "\t\t";
      $subgroup = drush_conduit_group('Sub-Group ' . $j, $client->nid, "'sub-group' => '{$client->nid}.{$group->nid}'", $group->nid);
      for ($k = 0; $k < 5; $k++) {
        echo "\t\t\t";
        $job = drush_conduit_job('Job ' . $k, $subgroup->nid, "'plugin' => 'conduit_execute', 'job' => '{$client->nid}.{$group->nid}.{$subgroup->nid}'");
      }
    }
  }
  return array($client, $group, $job);
}

/**
 * Create a conduit client.
 */
function drush_conduit_client($title = '', $properties = '', $key = NULL) {
  $client = new stdClass();
  $client->type = 'conduit_client';
  $client->language = LANGUAGE_NONE;
  $client->title = $title;
  $client->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  $client->conduit_key[LANGUAGE_NONE][0]['value'] = $key ? $key : md5(mt_rand(1000, 1000000000));
  node_save($client);
  echo "Created client #{$client->nid}.\n";
  return $client;
}

/**
 * Create a conduit group.
 */
function drush_conduit_group($title = '', $owner = NULL, $properties = '', $parent = NULL) {
  $group = new stdClass();
  $group->type = 'conduit_group';
  $group->language = LANGUAGE_NONE;
  $group->title = $title;
  $group->conduit_owner[LANGUAGE_NONE][0]['nid'] = $owner;
  $group->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  $group->conduit_parent[LANGUAGE_NONE][0]['nid'] = $parent;
  node_save($group);
  echo "Created group #{$group->nid}.\n";
  return $group;
}

/**
 * Create a conduit job.
 */
function drush_conduit_job($title = '', $parent = NULL, $properties = '') {
  $job = new stdClass();
  $job->type = 'conduit_job_execute';
  $job->language = LANGUAGE_NONE;
  $job->title = $title;
  $job->conduit_parent[LANGUAGE_NONE][0]['nid'] = $parent;
  $job->conduit_properties[LANGUAGE_NONE][0]['value'] = $properties;
  node_save($job);
  echo "Created job #{$job->nid}.\n";
  return $job;
}
