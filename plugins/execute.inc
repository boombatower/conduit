<?php
/**
 * @file
 * Provide command execution job type server plugin.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('Execute'),
  'description' => t('Provides basic command execution.'),
  'queue build' => 'conduit_execute_queue_build',
  'default properties' => array(
    'command' => 'echo "hello world"',
  ),
);

/**
 * Queue one item with no additional data.
 */
function conduit_execute_queue_build(array $properties) {
  return array(array());
}
