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
function hook_conduit_validate(array $properties) {
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
 * The hook will only be invoked if the chunk status is greater then
 * CONDUIT_STATUS_SETUP.
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
 * General hooks that do not have to be implemented by plugin modules.
 */

/**
 * Alter properties after they have been merged and validated.
 *
 * This can be useful if certain properties need to be overriden for particular
 * node types or other conditions.
 *
 * @param $properties
 *   Merged associative array of properties.
 * @param $node
 *   Node to which merged properties relate.
 */
function hook_conduit_properties_alter(array &$properties, $node) {
  if ($node->type == 'conduit_job_my_custom_type' && $properties['custom']) {
    $properties['mask'] = '/.*/';
  }
}

/**
 * Alter default properties before they are used for merging or validation.
 *
 * This can be useful if properties need to be added by modules that do not
 * provide job types for conduit or if base properties need to be added or
 * changed.
 *
 * @param $properties
 *   Associative array of default properties.
 * @param $module
 *   Module that provided the default properties.
 */
function hook_conduit_default_properties_alter(array &$properties, $module) {
  if ($module == 'conduit') {
    $properties['non_plugin_property'] = 'foo';
  }
}

/**
 * Perform property validation before a group or job is created.
 *
 * Unlike hook_conduit_validate() this hook is called for all modules instead
 * of just the plugin module or base property validation. If a module added or
 * alter properties using hook_conduit_default_properties_alter() then this
 * hook can be used to validate user input for those properties.
 *
 * @param $properties
 *   Merged associative array of properties.
 */
function hook_conduit_validate_all(array $properties) {
  extract($properties);
  if (!is_bool($non_plugin_property)) {
    conduit_validate_error('non_plugin_property', t('must be a boolean (true or false)'));
  }
}

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
