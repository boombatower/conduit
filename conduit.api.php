<?php
/**
 * @file
 * Document conduit hooks.
 *
 * Also see includes/api.inc for aditional API.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * @addtogroup hooks
 * @{
 */

/*
 * Hooks designed for use in modules that define job types.
 */

/**
 * Perform property validation before a group or job is created.
 *
 * @param $properties
 *   Merged array of properties.
 */
function hook_conduit_conduit_validate(array $properties) {
  extract($properties);
  if (!is_numeric($custom)) {
    conduit_validate_error('custom', t('must be numeric'));
  }
}

/**
 * Define the default properties associated with the module's job type.
 *
 * @return
 *   Associative array of property defaults.
 */
function hook_conduit_default_properties() {
  return array(
    'foo' => 'bar',
    'baz' => 'faz',
  );
}

/**
 * Build queue items based on a properties array.
 *
 * @param $properties
 *   Associative array of job properties.
 * @return
 *   An array of queue items. Each queue item is an array of properties to
 *   override. For simple job to be executed in one chunk simply return:
 *   @code
 *     array(array())
 *   @endcode
 *   For more complex jobs that can be split up into chunks then each queue
 *   item should contain the necessary property overrides to differenciate the
 *   item from the others. For example:
 *   @code
 *     array(
 *       array('do' => 'stuff 1'),
 *       array('do' => 'stuff 2'),
 *     )
 *   @endcode
 *   When the finally properties array is built and sent to the worker the
 *   overrides will be applied.
 */
function hook_conduit_queue_build(array $properties) {
  return array(array());
}

/**
 * Initialize plugin result field(s).
 *
 * Based on the $chunk_count each result field value should be initialized with
 * a placeholder so that the values may be filled in any order.
 *
 * @param $node
 *   Reference to job node whos fields are to be initialized.
 * @param $chunk_count
 *   The number of chunks in which the job will be completed and thus the
 *   number of results that will be returned.
 */
function hook_conduit_init($node, $chunk_count) {
  for ($delta = 0; $delta < $chunk_count; $delta++) {
    $node->conduit_result_mymodule_result[LANGUAGE_NONE][$delta]['value'] = '[placeholder]';
  }
}

/**
 * Store plugin result and generate a summary message.
 *
 * @param $node
 *   Reference to job node for which the result will be stored.
 * @param $delta
 *   Chunk delta for the result.
 * @param $result
 *   Result data, or FALSE if the chunk failed prior to plugin.
 */
function hook_conduit_result($node, $delta, $result) {
  if ($result) {
    $node->conduit_result_mymodule_result[LANGUAGE_NONE][$delta]['value'] = $result;
  }

  $node->conduit_summary[LANGUAGE_NONE][0]['value'] = 'Insert summary here.';
}

/*
 * Hooks designed for use in modules that provide queue processing.
 */

/**
 * Respond to queuing of job.
 *
 * @param $node
 *   Queued job node.
 * @param $chunk_count
 *   The number of chunks in which the job will be completed.
 */
function hook_conduit_queued($node, $chunk_count) {
  // Ask the little man inside the machine to complete the job chunks.
}

/**
 * @} End of "addtogroup hooks".
 */
