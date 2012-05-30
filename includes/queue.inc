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
 * @return
 *   The number of queue items created.
 */
function conduit_queue_create($job) {
  $info = conduit_job_info($job->type);
  $queue = DrupalQueue::get('conduit_job_' . $info['category']);

  // Plugin may build the item(s) to be queued in association with the job.
  $items = module_invoke($info['module'], 'conduit_queue_build', $job->conduit_properties_merged);

  // If the plugin did not provide any items then default to one item with no
  // overrides. In other words process the job in one shot.
  if (!$items) {
    $items = array(array());
  }

  $count = count($items);
  foreach ($items as $delta => $overrides) {
    $item = array(
      'category' => $info['category'],
      'nid' => $job->nid,
      'vid' => $job->vid,
      'delta' => $delta,
      'count' => $count,
      'overrides' => $overrides,
    );
    $queue->createItem($item);
  }
  return count($items);
}

/**
 * Claim an item from the queue.
 *
 * @param $category
 *   The job category from which to claim an item from.
 * @return
 *   An array of merged properties for the job and the queue item or FALSE.
 */
function conduit_queue_claim($category) {
  // Attempt to claim an item from the queue.
  $queue = DrupalQueue::get('conduit_job_' . $category);
  if ($item = $queue->claimItem(variable_get('conduit_lease_time', 3600))) {
    // Load the job node related to the item, add the item to the job
    // properties, and apply the property overrides.
    if ($job = node_load($item->data['nid'], $item->data['vid'])) {
      $properties = $job->conduit_properties_merged;
      $properties['item'] = $item;
      $properties = array_replace($properties, $item->data['overrides']);
      unset($properties['item']->data['overrides']);
      return $properties;
    }

    // Remove stale queue item and try again.
    conduit_queue_delete($item);
    return conduit_queue_claim($category);
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
  return DrupalQueue::get('conduit_job_' . $item->data['category'])->releaseItem($item);
}

/**
 * Delete a item from the queue.
 *
 * @param $item
 *   Queue item object.
 */
function conduit_queue_delete($item) {
  return DrupalQueue::get('conduit_job_' . $item->data['category'])->deleteItem($item);
}

/**
 * Determine the number of items in the queue.
 *
 * @param $category
 *   The job category from which to count the items.
 */
function conduit_queue_count($category) {
  return DrupalQueue::get('conduit_job_' . $category)->numberOfItems();
}
