<?php
/**
 * @file
 * Provides plugin information to views.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_views_plugins().
 */
function conduit_views_plugins() {
  return array(
    'argument validator' => array(
      'user_conduit' => array(
        'title' => t('User (conduit)'),
        'handler' => 'views_plugin_argument_validate_user_conduit',
      ),
    ),
  );
}
