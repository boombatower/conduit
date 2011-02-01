<?php
/**
 * @file
 * Provide node API related functions.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Implements hook_node_info().
 */
function conduit_node_info() {
  $info = array();

  $info['conduit_group'] = array(
    'name' => t('Conduit group'),
    'base' => 'conduit',
    'description' => t('Groups jobs together and can be used to create a hierarchy.'),
  );

  return $info;
}

/**
 * Generate job node type definition for a plugin.
 *
 * @param $plugin
 *   Plugin name.
 * @return
 *   Node type definition to be used with hook_node_info().
 * @see hook_node_info()
 */
function conduit_node_info_job($plugin) {
  $title = ucfirst(str_replace('_', ' ', $plugin));
  return array(
    'conduit_job_' . $plugin => array(
      'name' => t('Conduit job - @plugin', array('@plugin' => $title)),
      'base' => 'conduit',
      'description' => t('Job, of type @plugin, to be performed.', array('@plugin' => $title)),
    ),
  );
}

/**
 * Implements hook_form().
 */
function conduit_form($node, &$form_state) {
  return node_content_form($node, $form_state);
}

/**
 * Implements hook_form_alter().
 */
function conduit_form_alter(&$form, &$form_state, $form_id) {
  if (!conduit_node_type($form_id)) {
    return;
  }

  // If parent value exists then asign it to the default value.
  if (isset($_GET['parent'])) {
    $form['conduit_parent'][LANGUAGE_NONE][0]['nid']['#default_value'] = $_GET['parent'];
  }

  $form['title']['#description'] = t('Displayed when being referenced.');

  if (conduit_job_info($form_id)) {
    // Hide standard non-meta fields.
    foreach (array('done', 'chunk_count', 'pass', 'summary', 'chunk_pass', 'log') as $field) {
      $form['conduit_' . $field]['#access'] = FALSE;
    }

    // Jobs may not be updated so do not allow create revision to be unchecked.
    $form['revision_information']['revision']['#disabled'] = TRUE;
  }
}

/**
 * Implements hook_node_load().
 *
 * All the conduit specific data should already be loaded, but the merged
 * properties array needs to be generated. This has to be done in
 * hook_node_load() instead of hook_load() since the fields are not present.
 */
function conduit_node_load($nodes, $types) {
  if (array_filter($types, 'conduit_node_type')) {
    foreach ($nodes as $node) {
      $node->conduit_properties_merged = !empty($node->conduit_properties) ? conduit_properties_merge($node) : array();
    }
  }
}

/**
 * Determine if a node type is a conduit type.
 *
 * @param $type
 *   Node type.
 * @return
 *   TRUE if the type is a conduit type, otherwise FALSE.
 */
function conduit_node_type($type) {
  return strpos($type, 'conduit') === 0;
}

/**
 * Implements hook_validate().
 */
function conduit_validate($node, $form, &$form_state) {
  // @TODO Serial should behave like node_reference so this is unecessary.
  $node->conduit_properties[LANGUAGE_NONE][0]['value'] = serial_eval($node->conduit_properties[LANGUAGE_NONE][0]['value']);

  if (!is_array($properties = $node->conduit_properties[LANGUAGE_NONE][0]['value'])) {
    // Any errors with the properties definition will be dealt with by the
    // serial field validation.
    return;
  }

  // Merge properties so each key is assigned a value.
  $properties = conduit_properties_merge($node);

  // Conduit validates base properties.
  module_invoke('conduit', 'conduit_validate', $properties);

  if ($node->type != 'conduit_group') {
    // The module responsible for the job type validate its properties.
    $info = conduit_job_info($node->type);
    module_invoke($info['module'], 'conduit_validate', $properties);
  }

  // If errors where generated then print them and set on properties field.
  if ($errors = conduit_validate_error()) {
    form_set_error('conduit_properties', t('The <em>properties</em> field contains the following problems.') . theme('item_list', array('items' => $errors)));
  }
}

/**
 * Implements hook_conduit_validate().
 *
 * Validate all standard properties that any job may contain.
 */
function conduit_conduit_validate(array $properties) {
  extract($properties);

  if (!is_numeric($context) || intval($context) != $context) {
    conduit_validate_error('context', t('must be an integer'));
  }

  foreach (array('mask', 'nomask') as $name) {
    if (($message = conduit_validate_regex($$name)) !== TRUE) {
      conduit_validate_error($name, t('must be a valid regular expression (see <a href="http://php.net/preg_match">preg_match()</a>) [@message]',
        array('@message' => $message)));
    }
  }

  if (!in_array($newline, array('any', 'unix'))) {
    conduit_validate_error('newline', t('must be "any" or "unix"'));
  }

  if (!is_array($patch)) {
    conduit_validate_error('patch', t('must be an array'));
  }
  else {
    foreach ($patch as $number => $url) {
      if (!parse_url($url)) {
        conduit_validate_error('patch', t('invalid URL'), $number);
      }
    }
  }

  if (!is_array($path)) {
    conduit_validate_error('path', t('must be an array'));
  }
  else {
    foreach ($path as $number => $value) {
      if (!conduit_validate_path($value)) {
        conduit_validate_error('path', t('must be a relative path (does not begin or end with "/", does contain parts of "." or "..", or any backslashes "\")'), $number);
      }
    }
  }

  if (!is_array($setup)) {
    conduit_validate_error('setup', t('must be an array'));
  }

  if (!is_array($ssh)) {
    conduit_validate_error('ssh', t('must be an array'));
  }
  else {
    foreach ($ssh as $number => $set) {
      if (!is_array($set)) {
        conduit_validate_error('ssh', t('must be an array'), $number);
        continue;
      }

      $invalid = FALSE;
      foreach ($set as $key => $value) {
        if (!in_array($key, array('key', 'host', 'user', 'port'))) {
          conduit_validate_error('ssh', t('invalid key'), array($number, $key));
          $invalid = TRUE;
        }
        elseif ($key == 'port' && (!is_numeric($value) || intval($value) != $value)) {
          conduit_validate_error('ssh', t('must be an integer'), array($number, 'port'));
        }
      }
      if (!$invalid && count($set) < 4) {
        conduit_validate_error('ssh', t('must contain the four required keys (key, host, user, port)'), $number);
      }
    }
  }

  if (!is_array($vcs)) {
    conduit_validate_error('vcs', t('must be an array'));
  }
  else {
    foreach ($vcs as $path => $url) {
      if (!conduit_validate_path($path)) {
        conduit_validate_error('vcs', t('invalid path'), $path);
      }
      if (!in_array(substr($url, 0, 3), array('bzr', 'cvs', 'git', 'svn'))) { // @TODO Strong validation.
        conduit_validate_error('vcs', t('unsupported version control system'), $path);
      }
    }
  }

  if (!is_bool($verbose)) {
    conduit_validate_error('verbose', t('must be a boolean (true or false)'));
  }

  // Check for any numberic keys.
  foreach ($properties as $key => $value) {
    if (is_numeric($key)) {
      conduit_validate_error($key, t('root level property keys must be strings'));
      continue;
    }
  }
}

/**
 * Validate a regular expression.
 *
 * @param $pattern
 *   Regular expression pattern.
 * @return
 *   TRUE if valid, otherwise an error message.
 * @see preg_match()
 */
function conduit_validate_regex($pattern) {
  restore_error_handler();
  $return = @preg_match($pattern, '');
  set_error_handler('_drupal_error_handler');
  if ($return === FALSE) {
    $error = error_get_last();
    return $error['message'];
  }
  return TRUE;
}

/**
 * Validate a relative directory or file path.
 *
 * @param $path
 *   A directory or file path.
 * @return
 *   TRUE if valid path (includes empty path), otherwise FALSE.
 */
function conduit_validate_path($path) {
  if ($path) {
    $parts = explode('/', $path);
    foreach ($parts as $part) {
      if (in_array($part, array('', '.', '..')) || strpos($part, '\\') !== FALSE) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

/**
 * Files an error against the conduit properties field.
 *
 * @param $name
 *   (Optional) Name of property.
 * @param $message
 *   (Optional) Message related to property.
 * @param $index
 *   (Optional) If applicable the index(es) related to the property.
 * @return
 *   List of errors.
 */
function conduit_validate_error($name = NULL, $message = NULL, $index = NULL) {
  $errors = &drupal_static(__FUNCTION__, array());
  if ($name && $message) {
    if (!is_null($index)) {
      $name .= is_array($index) ? '[' . implode('][', $index) . ']' : "[$index]";
    }
    $errors[] = t('<strong>@name</strong>: !message', array('@name' => $name, '!message' => $message));
  }
  return $errors;
}

/**
 * Implements hook_insert().
 */
function conduit_insert($node) {
  if ($info = conduit_job_info($node->type)) {
    // Since the node is new it will not have the merged properties array.
    conduit_node_load(array($node), array($node->type));

    // Create the queue items for the job and thus the number of chunks.
    conduit_include('queue');
    $chunk_count = conduit_queue_create($node);

    // Initialize job fields and insert placeholder values into all multivalued
    // chunk fields to ensure the results can be inserted in any order.
    $node->conduit_done[LANGUAGE_NONE][0]['value'] = (int) FALSE;
    $node->conduit_chunk_count[LANGUAGE_NONE][0]['value'] = $chunk_count;
    $node->conduit_pass[LANGUAGE_NONE][0]['value'] = -1;
    $node->conduit_summary[LANGUAGE_NONE][0]['value'] = t('The job has not yet been performed.');
    for ($i = 0; $i < $chunk_count; $i++) {
      $node->conduit_chunk_pass[LANGUAGE_NONE][$i]['value'] = -1;
      $node->conduit_log[LANGUAGE_NONE][$i]['value'] = '[placeholder]';
    }

    // Since the plugin stores its results it must initialize its field(s).
    module_invoke($info['module'], 'conduit_init', $node, $chunk_count);

    // Allow all modules to react to a job being queued.
    module_invoke_all('conduit_queued', $node, $chunk_count);
  }
}

/**
 * Implements hook_update().
 */
function conduit_update($node) {
  // If a new revision is being generated then process the new job.
  if (isset($node->old_vid)) {
    conduit_insert($node);
  }
}

/**
 * Implements hook_form_FORM_ID_alter(): node_delete_confirm.
 */
function conduit_form_node_delete_confirm_alter(&$form, &$form_state) {
  // If a group then determine if there are any children.
  if ($node->type == 'conduit_group') {
    $group = views_get_view('conduit_group');
    $group->set_arguments(array($form['#node']->nid));
    $group->execute();

    $job = views_get_view('conduit_job');
    $job->set_arguments(array($form['#node']->nid));
    $job->execute();

    // Add a note about the children to be deleted.
    if ($total = $group->total_rows + $job->total_rows) {
      $form['description']['#markup'] = format_plural($total,
        'The group contains a child that will also be deleted.',
        'The group contains @count children that will also be deleted.'
      ) . ' ' . $form['description']['#markup'];
    }
  }
}

/**
 * Implements hook_delete().
 */
function conduit_delete($node) {
  // If a group then delete any child groups or jobs.
  if ($node->type == 'conduit_group') {
    conduit_delete_view('conduit_group', $node->nid);
    conduit_delete_view('conduit_job', $node->nid);
  }
}

/**
 * Delete all nodes in the view.
 *
 * @param $name
 *   View name.
 * @param $parent
 *   Parent node ID argument for view.
 */
function conduit_delete_view($name, $parent) {
  $page = 0;
  do {
    $view = views_get_view($name);
    $view->set_arguments(array($parent));
    $view->set_items_per_page(200);
    $view->set_current_page($page++);
    $view->execute();
    foreach ($view->result as $node) {
      node_delete($node->nid);
    }
  }
  while ($page * $view->get_items_per_page() < $view->total_rows);
}

/**
 * Implements hook_view().
 */
function conduit_view($node, $view_mode) {
  if ($view_mode == 'full' && node_is_page($node)) {
    // Add a breakcrumb on group or job nodes.
    if (conduit_node_type($node->type)) {
      // The breadcrumbs are order top-down, but drupal_set_breadcrumb() will
      // display them left-to-right so we need to reverse order and remove the
      // current node from the list.
      $breadcrumb = array_reverse(conduit_view_breadcrumb($node));
      array_pop($breadcrumb);
      drupal_set_breadcrumb($breadcrumb);
    }

    // Add a list of groups and jobs on group nodes.
    if ($node->type == 'conduit_group') {
      $view = views_get_view('conduit_group');
      $view->override_path = $_GET['q'];
      $node->content['conduit_group'] = array(
        '#markup' => $view->preview('default', array($node->nid)),
        '#weight' => 10,
      );

      $view = views_get_view('conduit_job');
      $view->override_path = $_GET['q'];
      $node->content['conduit_job'] = array(
        '#markup' => $view->preview('default', array($node->nid)),
        '#weight' => 11,
      );
    }
  }
  return $node;
}

/**
 * Generate a set of breadcrumb links from the top node down.
 *
 * @param $node
 *   Top node in tree.
 * @param $breadcrumb
 *   (Optional) Used internally to keep track of breadcrumbs.
 * @return
 *   A list of links ordered from the top node down.
 */
function conduit_view_breadcrumb($node, $breadcrumb = array()) {
  // Add the current node to the breadcrumb.
  $breadcrumb[] = l($node->title, 'node/' . $node->nid);

  // If the node has a parent the recurse.
  if (empty($node->conduit_parent)) {
    $breadcrumb[] = theme('username', array('account' => user_load($node->uid)));
  }
  else {
    $breadcrumb = conduit_view_breadcrumb(node_load($node->conduit_parent[LANGUAGE_NONE][0]['nid']), $breadcrumb);
  }
  return $breadcrumb;
}

/**
 * Implements hook_node_view_alter().
 */
function conduit_node_view_alter(&$build) {
  if ($build['#view_mode'] == 'full' && isset($build['conduit_properties'])) {
    // If the merged properties differ from the properties of the node then add
    // the merged set of properties to the node view.
    if ($build['#node']->conduit_properties[LANGUAGE_NONE][0]['value'] != $build['#node']->conduit_properties_merged) {
      $build['conduit_properties_inherit'] = array(
        '#type' => 'markup',
        '#markup' => highlight_string('<?php ' . var_export($build['#node']->conduit_properties_merged, TRUE), TRUE),
        '#weight' => $build['conduit_properties']['#weight'] + 0.1,
      );
    }
  }
}
