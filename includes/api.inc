<?php
/**
 * @file
 * Provides an API for interacting with conduit.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Store a chunk result.
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
  // Attempt to load the job node related to the queue item.
  if ($job = node_load($item->data['nid'], $item->data['vid'])) {
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
    module_invoke($job->conduit['properties']['plugin'], 'conduit_result', $job, $item->data['delta'], $result);

    // Save the changes made to the job.
    node_save($job);

    // Remove the complete queue item.
    conduit_queue_delete($item);
    return TRUE;
  }
  return FALSE;
}
