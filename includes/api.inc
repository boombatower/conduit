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
  return conduit_queue_claim($category);
}

/**
 * Store a result for an item.
 *
 * @param $item
 *   Queue item the results are related to.
 * @param $pass
 *   TRUE if the chunk passed, otherewise FALSE.
 * @param $result
 *   The result of processing the chunk.
 * @param $log
 *   The log generated while processing the chunk.
 * @return
 *   TRUE if successfull, otherwise FALSE.
 */
function conduit_api_result($item, $pass, $result, $log) {
  $item = (object) $item;

  // Acquire a lock to modify the node.
  $lock = conduit_api_lock_acquire($item->data['nid']);

  // Attempt to load the job node related to the queue item.
  if ($job = node_load($item->data['nid'], $item->data['vid'])) {
    // Ensure that the chunk has not already been saved.
    if ($job->conduit_chunk_pass[LANGUAGE_NONE][$item->data['delta']]['value'] != -1) {
      return FALSE;
    }

    // Store the chunk pass value and log.
    $job->conduit_chunk_pass[LANGUAGE_NONE][$item->data['delta']]['value'] = (int) $pass;
    $job->conduit_log[LANGUAGE_NONE][$item->data['delta']]['value'] = $log;

    // Determine if the job has been completed and the overall pass/fail state.
    $done = $pass = TRUE;
    foreach ($job->conduit_chunk_pass[LANGUAGE_NONE] as $chunk) {
      // Chunk value of -1 represent an incomplete chunk, 0 a failed chunk, and
      // 1 a chunk that passed.
      if ($chunk['value'] != -1) {
        $pass = $pass && $chunk['value'] == 1;
      }
      else {
        $done = FALSE;
      }
    }
    $job->conduit_done[LANGUAGE_NONE][0]['value'] = (int) $done;
    $job->conduit_pass[LANGUAGE_NONE][0]['value'] = (int) $pass;

    // Allow the plugin to place the result on the proper field(s).
    $info = conduit_job_info($job->type);
    module_invoke($info['module'], 'conduit_result', $job, $item->data['delta'], $result);

    // Save the changes made to the job.
    node_save($job);

    // Remove the complete queue item.
    conduit_queue_delete($item);
  }
  lock_release($lock);
  return (bool) $job;
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
