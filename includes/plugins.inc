<?php
/**
 * @file
 * Provide ctools plugin definitions and implementation hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 * @see http://panels3.dev.logrus.com/help/ctools/plugins-creating
 * @see http://panels3.dev.logrus.com/help/ctools/plugins-implementing
 */

/**
 * conduit:
 * - default argument values
 * - list of result keys with textual representation
 * - some sort of result output for HTML, xml, and e-mail (possibly hook into node views)
 * - provide queue items
 */

/**
 * Fetch metadata on a specific conduit plugin.
 *
 * @param $conduit
 *   Name of conduit plugin.
 * @return
 *   An array of information about the requested conduit plugin.
 */
function conduit_plugins_get($conduit) {
  ctools_include('plugins');
  return ctools_get_plugins('conduit', 'conduit', $conduit);
}

/**
 * Fetch metadata for all conduit plugins.
 *
 * @return
 *   An array of arrays with information about all available conduit plugins.
 */
function conduit_plugins_get_all() {
  ctools_include('plugins');
  return ctools_get_plugins('conduit', 'conduit');
}

/**
 * Implements hook_ctools_plugin_type().
 */
function conduit_ctools_plugin_type() {
  return array(
    'conduit' => array('cache' => TRUE),
  );
}

/**
 * Implements hook_ctools_plugin_directory().
 */
function conduit_ctools_plugin_directory($module, $plugin) {
  if ($module == 'conduit' && $plugin == 'conduit') {
    return 'plugins';
  }
}
