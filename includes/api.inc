<?php
/**
 * @file
 * Provides an API for interacting with conduit.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Claim an item from a category queue.
 *
 * @param $category
 *   The job category from which to claim an item from.
 * @return
 *   An array of merged properties for the job and the queue item or FALSE.
 * @see conduit_queue_claim()
 */
function conduit_api_claim($category) {
  conduit_include('queue');
  if ($properties = conduit_queue_claim($category)) {
    $item = $properties['item'];
    $lock = conduit_api_lock_acquire($item->data['nid']);

    // Update chunk state to processing.
    $job = node_load($item->data['nid'], $item->data['vid']);
    $job->conduit_chunk_status[LANGUAGE_NONE][$item->data['delta']]['value'] = 0;
    node_save($job);

    lock_release($lock);
  }
  return $properties;
}

/**
 * Store a result for an item.
 *
 * @param $item
 *   Queue item the results are related to.
 * @param $status
 *   A particular job chunk status.
 * @param $result
 *   The result of processing the chunk.
 * @param $log
 *   The log generated while processing the chunk.
 * @param $duration
 *   Indicates the the length of time elapsed processing the chunk.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function conduit_api_result($item, $status, $result, $log, $duration) {
  $item = (object) $item;

  // Acquire a lock to modify the node.
  $lock = conduit_api_lock_acquire($item->data['nid']);

  // Disable field suppression and force the field values to be reloaded by
  // clearing the cache which will be empty due to field_suppression.
  field_suppress_state(FALSE);

  // @TODO Remove once resetCache() on the entity control does this.
  cache_clear_all('field:node:' . $item->data['nid'], 'cache_field');

  // Attempt to load the job node related to the queue item.
  if ($job = node_load($item->data['nid'], $item->data['vid'], TRUE)) {
    // Ensure that the chunk has not already been saved.
    if ($job->conduit_chunk_status[LANGUAGE_NONE][$item->data['delta']]['value'] > 0) {
      return FALSE;
    }

    // If the chunk failed during setup then set the summary to the result.
    if ($status == CONDUIT_STATUS_SETUP) $job->conduit_summary[LANGUAGE_NONE][0]['value'] = $result;

    // Add the time elapsed processing the chunk to the overall elapse time.
    $job->conduit_duration[LANGUAGE_NONE][0]['value'] += $duration;

    // Store the chunk status and log.
    $job->conduit_chunk_status[LANGUAGE_NONE][$item->data['delta']]['value'] = $status;
    $job->conduit_log[LANGUAGE_NONE][$item->data['delta']]['value'] = $log;

    // Determine if the job has been completed and the overall status.
    $done = TRUE;
    $status = CONDUIT_STATUS_PASS;
    foreach ($job->conduit_chunk_status[LANGUAGE_NONE] as $chunk) {
      // Chunk value of -1 represent an incomplete chunk, 0 a failed chunk, and
      // 1 a chunk that passed.
      if ($chunk['value'] > 0) {
        $status = min($status, $chunk['value']);
      }
      else {
        $done = FALSE;
      }
    }
    $job->conduit_done[LANGUAGE_NONE][0]['value'] = (int) $done;
    $job->conduit_status[LANGUAGE_NONE][0]['value'] = $status;

    // Only invoke hook_conduit_result() if the chunk reached the plugin.
    if ($status > CONDUIT_STATUS_SETUP) {
      // Allow the plugin to place the result on the proper field(s).
      $info = conduit_job_info($job->type);
      module_invoke($info['module'], 'conduit_result', $job, $item->data['delta'], $result);
    }

    // Save the changes made to the job.
    node_save($job);

    // Remove the complete queue item.
    conduit_include('queue');
    conduit_queue_delete($item);
  }

  field_suppress_state(TRUE);
  lock_release($lock);
  return (bool) $job;
}

/**
 * Repeat a job by creating a new revision.
 *
 * @param $nid
 *   Job node ID to repeat.
 * @return
 *   TRUE if the job node was found and repeated, otherwise FALSE.
 */
function conduit_api_repeat($nid) {
  if ($node = node_load($nid)) {
    $node->revision = TRUE;
    node_save($node);
    return TRUE;
  }
  return FALSE;
}

/**
 * Acquire a Conduit API lock for a specific node.
 *
 * @param $nid
 *    Node ID.
 * @return
 *   The lock name to use when releasing lock.
 * @see lock_acquire()
 * @see lock_release()
 */
function conduit_api_lock_acquire($nid) {
  $lock = 'conduit_api_' . $nid;
  while (!lock_acquire($lock)) {
    sleep(1);
  }
  return $lock;
}
