<?php
/**
 * @file
 * Provide node API related functions.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

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
  // Check for conduit node form.
  if (strpos($form_id, 'conduit') !== 0) {
    return;
  }

  // Set title description using type.
  $form['title']['#description'] = t('Displayed when being referencing.');

  // If key is present, but not set the create a random key.
  if (isset($form['conduit_key']) && empty($form['conduit_key'][LANGUAGE_NONE][0]['value']['#default_value'])) {
    $form['conduit_key'][LANGUAGE_NONE][0]['value']['#default_value'] = md5(mt_rand(1000, 1000000000));
  }

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
 * All the conduit specific data should already be loaded, but some of it
 * requires processing and all the conduit data will be placed in an easy to
 * access format instead of the abstract field api format for reading. This has
 * to be done in hook_node_load() instead of hook_load() since fields are not
 * present in hook_load().
 */
function conduit_node_load($nodes, $types) {
  // Look for any conduit types among the list of node types.
  $found = FALSE;
  foreach ($types as $type) {
    if (strpos($type, 'conduit') === 0) {
      $found = TRUE;
      break;
    }
  }
  // Only process nodes if a conduit type is present.
  if (!$found) {
    return;
  }

  foreach ($nodes as $node) {
    // Ensure the current node is a conduit node.
    if (strpos($node->type, 'conduit') !== 0) {
      continue;
    }
    $node->conduit = array();
    $fields = array('key', 'owner', 'parent', 'properties', 'done', 'chunk_count', 'pass');
    foreach ($fields as $field) {
      $key = 'conduit_' . $field;
      if (!empty($node->$key)) {
        $value = $node->$key;
        $node->conduit[$field] = $value[LANGUAGE_NONE][0][$field == 'owner' || $field == 'parent' ? 'nid' : 'value'];
      }
    }

    // If the properties field has a value then parse the value, otherwise
    // initialize the properties attribute if applicable.
    if (isset($node->conduit['properties'])) {
      $node->conduit['properties'] = conduit_properties($node->conduit['properties']);
    }
    elseif ($node->type != 'conduit_worker') {
      $node->conduit['properties'] = array();
    }

    // If chunk_pass field is defined then entire the attribute is initialized
    // and if it has values then simplify them and place in attribute.
    if (isset($node->conduit_chunk_pass)) {
      $node->conduit['chunk_pass'] = array();
      if (!empty($node->conduit_chunk_pass)) {
        foreach ($node->conduit_chunk_pass[LANGUAGE_NONE] as $key => $chunk) {
          $node->conduit['chunk_pass'][$key] = $chunk['value'];
        }
      }
    }
  }
}

/**
 * Implements hook_validate().
 */
function conduit_validate($node, $form = array(), &$form_state) {
  // Check conduit key length.
  if (isset($node->conduit_key)) {
    $key = $node->conduit_key[LANGUAGE_NONE][0]['value'];
    if (drupal_strlen($key) < 32 || drupal_strlen($key) > 64) {
      form_set_error('conduit_key', t('The key field must be at least 32 and no more then 64 characters in length.'));
    }
  }

  // Ensure properties are in valid php syntax.
  if (isset($node->conduit_properties)) {
    $properties = $node->conduit_properties[LANGUAGE_NONE][0]['value'];

    // Ensure the properties string contains only allowed tokens, ignoring the
    // first token which will be the <?php token.
    $tokens = token_get_all('<?php ' . $properties);
    array_shift($tokens);
    $allowed = array(T_ARRAY, T_CONSTANT_ENCAPSED_STRING, T_DNUMBER, T_DOUBLE_ARROW, T_LNUMBER, T_STRING, T_WHITESPACE);
    $valid = TRUE;
    foreach ($tokens as $token) {
      if (is_array($token) && !in_array($token[0], $allowed)) {
        $valid = FALSE;
        break;
      }
    }

    // If the properties string contains only valid tokens then attempt to
    // evaluate it as a a PHP array, otherwise display an error.
    if ($valid) {
      $properties = conduit_properties($properties);
      if (!is_array($properties)) {
        form_set_error('conduit_properties', t('The properties field must contain valid PHP syntax.'));
      }
      else {
        // Allow plugins to validate the properties of which they are
        // responsible and provide error messages.
        if ($errors = module_invoke_all('conduit_validate', $properties)) {
          form_set_error('conduit_properties', t('The properties field contains the following problems.') . theme('item_list', array('items' => $errors)));
        }
      }
    }
    else {
      form_set_error('conduit_properties', t('The properties field may only contain arrays, numbers, strings, or whitespace.'));
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
    switch ($key) {
      case 'context':
        if (!is_numeric($value) || intval($value) != $value) {
          $errors[] = t('The <em>context</em> property must be an integer.');
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
      case 'plugin':
        if ($value == 'conduit' || !module_exists($value) || !in_array($value, module_implements('conduit_default_properties'))) {
          $errors[] = t('The <em>plugin</em> property is not a valid plugin.');
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
  if (conduit_job_info($node->type)) {
    // Since the node is brand new it will not have the ->conduit property
    // which is used throughout the API, so force a load.
    conduit_node_load(array($node), array($node->type));
    conduit_include('queue');

    // Create the queue items for the job and thus the number of chunks.
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
    module_invoke($node->conduit['properties']['plugin'], 'conduit_init', $node, $chunk_count);
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

    // Add a list of group on client or group nodes.
    if ($node->type == 'conduit_client' || $node->type == 'conduit_group') {
      $view = views_get_view('conduit_group');
      $view->override_path = $_GET['q'];
      $node->content['conduit_group'] = array(
        '#markup' => $view->preview('defualt', array($node->nid)),
        '#weight' => 10,
      );

      $view = views_get_view('conduit_job');
      $view->override_path = $_GET['q'];
      $node->content['conduit_job'] = array(
        '#markup' => $view->preview('defualt', array($node->nid)),
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
  if (!empty($node->conduit['parent'])) {
    $breadcrumb = conduit_view_breadcrumb(node_load($node->conduit['parent']), $breadcrumb);
  }
  return $breadcrumb;
}

/**
 * Implements hook_field_formatter_info().
 */
function conduit_field_formatter_info() {
  return array(
    'conduit_properties' => array(
      'label' => t('Conduit properties'),
      'description' => t('Represent properties in an easy to read manner.'),
      'field types' => array('text_long'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function conduit_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'conduit_properties':
      foreach ($items as $delta => $item) {
        $properties = conduit_properties($item['value']);
        $element[$delta]['#type'] = 'markup';
        $element[$delta]['#markup'] = highlight_string('<?php ' . var_export($properties, TRUE), TRUE);
      }
      break;
  }

  return $element;
}

/**
 * Implements hook_node_view_alter().
 *
 * Add the inherited, or merged, display of properties to the node view.
 */
function conduit_node_view_alter(&$build) {
  if ($build['#view_mode'] == 'full' && isset($build['conduit_properties'])) {
    $properties_inherit = conduit_properties_merge($build['#node']);
    if ($build['#node']->conduit['properties'] != $properties_inherit) {
      $build['conduit_properties_inherit'] = array(
        '#type' => 'markup',
        '#markup' => highlight_string('<?php ' . var_export($properties_inherit, TRUE), TRUE),
        '#weight' => $build['conduit_properties']['#weight'] + 0.1,
      );
    }
  }
}
