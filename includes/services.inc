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
              'description' => 'The job category from which to claim an item from.',
              'source' => 'data',
              'optional' => FALSE,
            ),
          ),
        ),
        'result' => array(
          'help' => 'Store a chunk result.',
          'callback' => 'conduit_api_result',
          'file' => array('type' => 'inc', 'module' => 'conduit', 'name' => 'includes/api'),
          'access arguments' => array('result conduit item'),
          'args' => array(
            array(
              'name' => 'item',
              'type' => 'struct',
              'description' => 'Queue item the results are related to.',
              'source' => 'data',
              'optional' => FALSE,
            ),
            array(
              'name' => 'pass',
              'type' => 'int',
              'description' => 'TRUE (1) if the chunk passed, otherewise FALSE (0).',
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
          ),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_default_services_endpoint().
 */
function conduit_default_services_endpoint($export) {
  $enpoints = array();

  $endpoint = new stdClass;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'conduit';
  $endpoint->title = 'Conduit';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'conduit';
  $endpoint->authentication = array(
    'services_sessauth' => array(),
  );
  $endpoint->resources = array(
    'user' => array(
      'alias' => '',
      'operations' => array(),
      'actions' => array(
        'login' => array(
          'enabled' => 1,
        ),
      ),
    ),
    'node' => array(
      'alias' => '',
      'operations' => array(
        'create' => array(
          'enabled' => 1,
        ),
        'retrieve' => array(
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
    'item' => array(
      'alias' => '',
      'actions' => array(
        'claim' => array(
          'enabled' => 1,
        ),
        'result' => array(
          'enabled' => 1,
        ),
      ),
    ),
  );
  $endpoint->debug = 0;
  $endpoint->status = 1;

  $endpoints[$endpoint->name] = $endpoint;

  return $endpoints;
}
