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
 * server:
 * - default argument values
 * - list of result keys with textual representation
 * - some sort of result output for HTML, xml, and e-mail (possibly hook into node views)
 * - provide queue items
 *
 * worker:
 * - result consists of string key PATCH_APPLY and details array
 */

function conduit_plugins_server_get($server) {
  ctools_include('plugins');
  return ctools_get_plugins('conduit', 'server', $server);
}

function conduit_plugins_server_get_all() {
  ctools_include('plugins');
  return ctools_get_plugins('conduit', 'server');
}

/**
 * Implements hook_ctools_plugin_TYPE().
 */
function conduit_ctools_plugin_server() {
  return array(
    'cache' => TRUE,
  );
}

/**
 * Implements hook_ctools_plugin_directory().
 */
function conduit_ctools_plugin_directory($module, $plugin) {
  if ($module == 'conduit' && ($plugin == 'server' || $plugin == 'worker')) { // @TODO should worker plugins be implement in conduit base module?
    return 'plugins/' . $plugin;
  }
}
