<?php
/**
 * @file
 * Provide services integration.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_services_resources().
 */
function conduit_services_resources() {
  return array(
    'item' => array(
      'actions' => array(
        'claim' => array(
          'help' => 'Claim an item from a category queue.',
          'callback' => 'conduit_api_claim',
          'file' => array('type' => 'inc', 'module' => 'conduit', 'name' => 'includes/api'),
          'access arguments' => array('claim conduit item'),
          'args' => array(
            array(
              'name' => 'category',
              'type' => 'string',
              'description' => 'The job category from which to claim an item.',
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'result' => array(
          'help' => 'Store a result for an item.',
          'callback' => 'conduit_api_result',
          'file' => array('type' => 'inc', 'module' => 'conduit', 'name' => 'includes/api'),
          'access arguments' => array('result conduit item'),
          'args' => array(
            array(
              'name' => 'item',
              'type' => 'array',
              'description' => 'Queue item the results are related to.',
              'source' => 'data',
              'optional' => FALSE,
            ),
            array(
              'name' => 'status',
              'type' => 'int',
              'description' => 'Chunk result status.',
              'source' => 'data',
              'optional' => FALSE,
            ),
            array(
              'name' => 'result',
              'type' => 'array',
              'description' => 'The result of processing the chunk.',
              'source' => 'data',
              'optional' => FALSE,
            ),
            array(
              'name' => 'log',
              'type' => 'string',
              'description' => 'The log generated while processing the chunk.',
              'source' => 'data',
              'optional' => FALSE,
            ),
            array(
              'name' => 'duration',
              'type' => 'int',
              'description' => 'Indicates the the length of time elapsed processing the chunk.',
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
      ),
    ),
    'job' => array(
      'actions' => array(
        'repeat' => array(
          'help' => 'Repeat a job by creating a new revision.',
          'callback' => 'conduit_api_repeat',
          'file' => array('type' => 'inc', 'module' => 'conduit', 'name' => 'includes/api'),
          'access callback' => 'conduit_node_repeat_access',
          'access arguments append' => TRUE,
          'args' => array(
            array(
              'name' => 'id',
              'type' => 'int',
              'description' => 'Job node ID to repeat.',
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_default_services_endpoint().
 */
function conduit_default_services_endpoint() {
  $enpoints = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'conduit';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'conduit';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'bencode' => TRUE,
        'json' => TRUE,
        'php' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'yaml' => TRUE,
        'jsonp' => FALSE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
        'application/x-www-form-urlencoded' => FALSE,
      ),
    ),
  );
  $endpoint->resources = array(
    'item' => array(
      'actions' => array(
        'claim' => array(
          'enabled' => 1,
        ),
        'result' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'job' => array(
      'actions' => array(
        'repeat' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'node' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'user' => array(
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;

  $endpoints[$endpoint->name] = $endpoint;

  return $endpoints;
}
