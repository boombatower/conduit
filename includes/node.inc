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

  // Set title description using type.
  $form['title']['#description'] = t('Displayed when being referencing.');

  // Move all conduit fields into a fieldset.
  $is_job = (bool) conduit_job_info($form_id);
  $fieldsets = $is_job ? array('meta', 'status', 'result') : array('meta');
  foreach ($fieldsets as $fieldset) {
    $form['conduit_' . $fieldset] = array(
      '#type' => 'fieldset',
      '#title' => ucfirst($fieldset),
      '#collapsible' => TRUE,
      '#collapsed' => $fieldset != 'meta',
      '#access' => user_access('edit conduit job ' . $fieldset),
    );
  }

  $status = array('conduit_done', 'conduit_chunk_count');
  $result = array('conduit_pass', 'conduit_summary', 'conduit_chunk_pass', 'conduit_log');
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#type']) && $form[$key]['#type'] == 'fieldset') {
      continue;
    }

    if ($key == 'title' || strpos($key, 'conduit_') === 0) {
      $fieldset = 'meta';
      if ($is_job) {
        if (in_array($key, $status)) {
          $fieldset = 'status';
        }
        elseif (in_array($key, $result) || strpos($key, 'conduit_result') === 0) {
          $fieldset = 'result';
        }
      }

      $form['conduit_' . $fieldset][$key] = $form[$key];
      unset($form[$key]);
    }
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
  // If properties can be loaded then allow the plugins to validate them,
  // otherwise any errors with the properties defintion will be delt with by
  // the serial field validation.
  if (isset($node->conduit_properties) && ($properties = serial_eval($node->conduit_properties[LANGUAGE_NONE][0]['value']))) {
    // Allow plugins to validate the properties of which they are responsible
    // and provide error messages.
    if ($errors = module_invoke_all('conduit_validate', $properties)) {
      form_set_error('conduit_properties', t('The properties field contains the following problems.') . theme('item_list', array('items' => $errors)));
    }
  }
}

/**
 * Implements hook_conduit_validate().
 *
 * Validate all standard properties that any job may contain.
 */
function conduit_conduit_validate(array $properties) {
  $errors = array();
  foreach ($properties as $key => $value) {
    if (is_numeric($key)) {
      $errors[] = t('Root level properties must have a key.');
      continue;
    }

    switch ($key) {
      case 'context':
        if (!is_numeric($value) || intval($value) != $value) {
          $errors[] = t('The <em>context</em> property must be an integer.');
        }
        break;
      case 'database':
        if (!in_array($value, array('mysql', 'pgsql', 'sqlite'))) {
          $errors[] = t('The <em>database</em> key must be set to "mysql", "pgsql", or "sqlite".');
        }
        break;
      case 'mask':
      case 'nomask':
        if (($message = conduit_validate_regex($value)) !== TRUE) {
          $errors[] = t('The <em>@key</em> property must be a valid Perl regular expression as accepted by the PHP function <a href="http://php.net/preg_match">preg_match()</a>. The following error was found: @error.',
            array('@key' => $key, '@error' => $message));
        }
        break;
      case 'newline':
        if (!in_array($value, array('any', 'unix'))) {
          $errors[] = t('The <em>newline</em> property must be either "any" or "unix".');
        }
        break;
      case 'patch':
        if (!is_array($value)) {
          $errors[] = t('The <em>patch</em> property must be an array containing URLs.');
        }
        else {
          foreach ($value as $number => $url) {
            if (!parse_url($url)) {
              $errors[] = t('The <em>patch</em> property contains an invalid URL in entry number %number.', array('%number' => $number + 1));
            }
          }
        }
        break;
      case 'path':
        if (!conduit_validate_path($value)) {
          $errors[] = t('The <em>path</em> property must be a relative path that does not begin or end with "/", does contain parts of "." or "..", or any backslashes ("\").');
        }
        break;
      case 'setup':
        if (!is_array($value)) {
          $errors[] = t('The <em>setup</em> property must be an array of commands.');
        }
        break;
      case 'ssh':
        if (!is_array($value)) {
          $errors[] = t('The <em>ssh</em> property must be an array of ssh information.');
        }
        else {
          foreach ($value as $number => $ssh) {
            $invalid = FALSE;
            foreach ($ssh as $key => $value) {
              if (!in_array($key, array('key', 'host', 'user', 'port'))) {
                $errors[] = t('The <em>ssh</em> property value number %number has an invalid key.', array('%number' => $number + 1));
                $invalid = TRUE;
              }
              elseif ($key == 'port' && (!is_numeric($value) || intval($value) != $value)) {
                $errors[] = t('The <em>ssh</em> property value number %number contains an invalid port number.', array('%number' => $number + 1));
              }
            }
            if (!$invalid && count($ssh) < 4) {
              $errors[] = t('The <em>ssh</em> property value number %number does not contain the four required keys.', array('%number' => $number + 1));
            }
          }
        }
        break;
      case 'vcs':
         if (!is_array($value)) {
          $errors[] = t('The <em>vcs</em> property must be an array of version control information.');
        }
        else {
          foreach ($value as $path => $url) {
            if ($path != '' && !conduit_validate_path($path)) {
              $errors[] = t('The <em>vcs</em> property contains the invalid path "@path".', array('@path' => $path));
            }
            if (!in_array(substr($url, 0, 3), array('bzr', 'cvs', 'git', 'svn'))) { // @TODO Strong validation.
              $errors[] = t('The <em>vcs</em> property contains a URL that references in unsupported version control system at path "@path".', array('@path' => $path));
            }
          }
        }
        break;
      case 'verbose':
        if (!is_bool($value)) {
          $errors[] = t('The <em>verbose</em> property must be a boolean, either TRUE or FALSE.');
        }
        break;
    }
  }
  return $errors;
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
 *   TRUE if valid path, otherwise FALSE.
 */
function conduit_validate_path($path) {
  $parts = explode('/', $path);
  foreach ($parts as $part) {
    if (in_array($part, array('', '.', '..')) || strpos($part, '\\') !== FALSE) {
      return FALSE;
    }
  }
  return TRUE;
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
 * Implements hook_view().
 */
function conduit_view($node, $view_mode) {
  if ($view_mode == 'full' && node_is_page($node)) {
    // Add a breakcrumb on group or job nodes.
    if ($node->type == 'conduit_group' || conduit_job_info($node->type)) {
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
