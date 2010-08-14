<?php
/**
 * @file
 * Provide a wrapper layer around the default system queue implementation.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Create queue item(s) for a job.
 *
 * @param $job
 *   Job node.
 */
function conduit_queue_create($job) {
  $queue = DrupalQueue::get('conduit_job');

  // Plugin builds the item(s) to be queued in association with job.
  if ($plugin = conduit_plugins_server_get($job->conduit['properties']['plugin'])) {
    $items = $plugin['queue build']($job->conduit['properties']);
    foreach ($items as $delta => $data) {
      $item = array(
        'nid' => $job->nid,
        'vid' => $job->vid,
        'delta' => $delta,
      ) + $data;
      $queue->createItem($item);
    }
  }
}

/**
 * Claim an item from the queue.
 *
 * @return
 *   An array of merged properties for the job and the queue item or FALSE.
 */
function conduit_queue_claim() {
  // Attempt to claim an item from the queue.
  $queue = DrupalQueue::get('conduit_job');
  if ($item = $queue->claimItem(variable_get('conduit_least_time', 3600))) {
    // Load the job node related to the item, add the item to the job
    // properties, and return the properties array.
    $job = node_load($item->data['nid'], $item->data['vid']);
    $job->conduit['properties']['item'] = $item;

    // Load group properties recursively and merge in default properties.
    return conduit_properties_merge($job);
  }
  return FALSE;
}

/**
 * Release a queue item.
 *
 * @param
 *   Queue item object.
 */
function conduit_queue_release($item) {
  return DrupalQueue::get('conduit_job')->releaseItem($item);
}

/**
 * Delete a item from the queue.
 *
 * @param $item
 *   Queue item object.
 */
function conduit_queue_delete($item) {
  return DrupalQueue::get('conduit_job')->deleteItem($item);
}
