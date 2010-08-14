<?php
/**
 * @file
 * Provide script job type server plugin.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

$plugin = array(
  'title' => t('Script'),
  'description' => t('Provides basic script execution.'),
  'queue build' => 'conduit_script_queue_build',
  'default properties' => array(),
);

/**
 * Queue one item with no additional data.
 */
function conduit_script_queue_build(array $properties) {
  return array(array());
}
