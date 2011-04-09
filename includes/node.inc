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
    'description' => t('A <em>group</em> is requried in order to create jobs which must have a parent group. Groups can also be used to create hierarchies for job organization.'),
  );

  return $info;
}

/**
 * Generate job node type definition for a plugin.
 *
 * @param $plugin
 *   Plugin name.
 * @param $description
 *   (Optional) Description of job type.
 * @return
 *   Node type definition to be used with hook_node_info().
 * @see hook_node_info()
 */
function conduit_node_info_job($plugin, $description = NULL) {
  $title = ucfirst(str_replace('_', ' ', $plugin));
  return array(
    'conduit_job_' . $plugin => array(
      'name' => t('Conduit job - @plugin', array('@plugin' => $title)),
      'base' => 'conduit',
      'description' => $description ? $description : t('Job, of type @plugin, to be performed.', array('@plugin' => $title)),
    ),
  );
}

/**
 * Implements hook_node_access().
 */
function conduit_node_access($node, $op, $account) {
  // Only alter node access if $op is view and conduit type.
  if ($op == 'view' && conduit_node_type(is_string($node) ? $node : $node->type)) {
    $author = user_load($node->uid);
    return conduit_user_access($author, $account) ? NODE_ACCESS_ALLOW : NODE_ACCESS_DENY;
  }
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
    foreach (element_children($form) as $key) {
      if (strpos($key, 'conduit') === 0 && !in_array($key, array('conduit_parent', 'conduit_properties'))) {
        $form[$key]['#access'] = FALSE;
      }
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
      if (conduit_node_type($node->type)) {
        $node->conduit_properties_merged = !empty($node->conduit_properties) ? conduit_properties_merge($node) : array();
      }
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
  if ($node->nid && !conduit_node_repeat_access($node)) {
    form_set_error('op', t('This job is currently not done and thus cannot be repeated.'));
    return;
  }

  if (!is_array($node->conduit_properties[LANGUAGE_NONE][0]['value'])) {
    // Any errors with the properties definition will be dealt with by the
    // serial field validation.
    return;
  }

  // Merge properties so each key is assigned a value.
  $properties = conduit_properties_merge($node);

  // Add base defaults if node is a group.
  if ($node->type == 'conduit_group') {
    $base_defaults = module_invoke('conduit', 'conduit_default_properties');
    $properties = array_replace_recursive($base_defaults, $properties);
  }

  // Conduit validates base properties and invoke general validation hook.
  module_invoke('conduit', 'conduit_validate', $properties);
  module_invoke_all('conduit_validate_all', $properties);

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
    foreach ($patch as $url => $apply) {
      if (is_numeric($url)) {
        conduit_validate_error('patch', t('key must be a patch'), $url);
      }
      elseif (!parse_url($url)) {
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
    $node->conduit_status[LANGUAGE_NONE][0]['value'] = 0;
    $node->conduit_duration[LANGUAGE_NONE][0]['value'] = 0;
    $node->conduit_summary[LANGUAGE_NONE][0]['value'] = t('The job has not yet been performed.');
    for ($i = 0; $i < $chunk_count; $i++) {
      $node->conduit_chunk_status[LANGUAGE_NONE][$i]['value'] = -1;
      $node->conduit_log[LANGUAGE_NONE][$i]['value'] = '[placeholder]';
    }

    // Since the plugin stores its results it must initialize its field(s).
    module_invoke($info['module'], 'conduit_init', $node, $chunk_count);

    // Allow all modules to react to a job being queued.
    module_invoke_all('conduit_queued', $node, $chunk_count);

    // Disable field suppression so that initialized states are saved.
    field_suppress_state(FALSE);
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
  if ($form['#node']->type == 'conduit_group') {
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
    // The breadcrumbs are order top-down, but drupal_set_breadcrumb() will
    // display them left-to-right so we need to reverse order and remove the
    // current node from the list.
    $breadcrumb = array_reverse(conduit_view_breadcrumb($node));
    array_pop($breadcrumb);
    drupal_set_breadcrumb($breadcrumb);

    // Add conduit css to all conduit node types.
    drupal_add_css(drupal_get_path('module', 'conduit') . '/conduit.css');

    if ($node->type == 'conduit_group') {
      // Add a list of groups and jobs on group nodes.
      $view = views_get_view('conduit_group');
      $view->override_path = $_GET['q'];
      $view->execute();
      $node->content['conduit_group'] = array(
        '#markup' => $view->preview('default', array($node->nid)),
        '#access' => (bool) $view->total_rows || !empty($_GET['group_title']),
        '#weight' => 10,
      );

      $view = views_get_view('conduit_job');
      $view->override_path = $_GET['q'];
      $view->execute();
      $node->content['conduit_job'] = array(
        '#markup' => $view->preview('default', array($node->nid)),
        '#access' => (bool) $view->total_rows || !empty($_GET['job_title']),
        '#weight' => 11,
      );

      // Print help text when there are no jobs or groups contained by a group.
      if (!$node->content['conduit_group']['#access'] && !$node->content['conduit_job']['#access'] && node_access('update', $node)) {
        drupal_set_message(t('Use the links to create jobs or sub-groups.'));
      }
    }
    else {
      // Add meta information to job node.
      conduit_view_meta($node);
    }
  }
  return $node;
}

/**
 * Add meta data information to job node.
 *
 * @param $node
 *   Job node.
 */
function conduit_view_meta(&$node) {
  $node->content['meta'] = array(
    '#prefix' => '<div class="conduit-meta">',
    '#suffix' => '</div>',
  );

  $status = conduit_status($node->conduit_status[LANGUAGE_NONE][0]['value']);
  $node->content['meta']['status'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'conduit-meta-status',
      'class' => array('conduit-icon', 'conduit-' . $status),
      'alt' => ucfirst($status),
      'title' => ucfirst($status)
    ),
  );

  // While incomplete show chunk progress bars.
  if (!$node->conduit_done[LANGUAGE_NONE][0]['value']) {
    // Determine the number of chunks in the three states.
    $queued = $processing = $complete = 0;
    foreach ($node->conduit_chunk_status[LANGUAGE_NONE] as $chunk) {
      if ($chunk['value'] == -1) $queued++;
      elseif ($chunk['value'] == 0) $processing++;
      else $complete++;
    }

    $node->content['meta']['chunks'] = array(
      '#type' => 'container',
      '#attributes' => array('id' => 'conduit-chunks'),
    );
    $chunks = &$node->content['meta']['chunks'];
    $chunks['complete'] = $chunks['active'] = array(
      '#type' => 'container',
      '#attributes' => array(),
      array(
        '#type' => 'container',
        '#attributes' => array(),
        array('#markup' => ''),
      ),
      array(
        '#type' => 'container',
        '#attributes' => array('style' => 'width: '),
      ),
    );

    $chunks['active'][0][0]['#markup'] = t('Active');
    $chunks['active'][1]['#attributes']['style'] .= round($processing / ($queued + $processing) * 100) . '%;';

    $chunks['complete'][0][0]['#markup'] = t('Complete');
    $chunks['complete'][1]['#attributes']['style'] .= round($complete / $node->conduit_chunk_count[LANGUAGE_NONE][0]['value'] * 100) . '%;';
  }

  $node->content['meta']['updated'] = array(
    '#type' => 'item',
    '#title' => t('Updated'),
    '#markup' => t('%time ago', array('%time' => format_interval(REQUEST_TIME - $node->revision_timestamp))),
  );
  $node->content['meta']['duration'] = array(
    '#type' => 'item',
    '#title' => t('Duration'),
    '#markup' => format_interval($node->conduit_duration[LANGUAGE_NONE][0]['value']),
    '#access' => (bool) $node->conduit_duration[LANGUAGE_NONE][0]['value'],
  );

  $node->content['meta']['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'conduit-actions'),
  );

  // If there is more then one revision then display relevant action buttons.
  // When on the current revision display a previous button; when in the middle
  // display current, previous and next buttons; and when on last revision
  // display current and next buttons.
  if (count($revisions = node_revision_list($node)) > 1) {
    if (current($revisions)->vid != $node->vid) {
      $node->content['meta']['actions']['revision']['current'] = conduit_view_meta_link(t('Current revision'), 'node/' . $node->nid, 'revision-current');
    }
    list($previous, $next) = conduit_view_meta_siblings_revisions($node, $revisions);
    if ($previous) {
      $node->content['meta']['actions']['revision']['previous'] = conduit_view_meta_link(t('Previous revision'), 'node/' . $node->nid . '/revisions/' . $previous->vid, 'revision-previous');
    }
    if ($next && current($revisions)->vid != $next->vid) {
      $node->content['meta']['actions']['revision']['next'] = conduit_view_meta_link(t('Next revision'), 'node/' . $node->nid . '/revisions/' . $next->vid, 'revision-next');
    }
  }

  // If the job has siblings, there are more then one job in group, then
  // display next and previous links.
  if ($siblings = conduit_view_meta_siblings_group($node->nid, $node->conduit_parent[LANGUAGE_NONE][0]['nid'])) {
    $node->content['meta']['actions']['group']['previous'] = conduit_view_meta_link(t('Previous job in group'), 'node/' . $siblings[0]->nid, 'group-previous');
    $node->content['meta']['actions']['group']['next'] = conduit_view_meta_link(t('Next job in group'), 'node/' . $siblings[1]->nid, 'group-next');
  }
  $node->content['meta']['actions']['repeat'] = drupal_get_form('conduit_view_repeat_form', $node, isset($node->content['meta']['actions']['revision']['current']));
}

/**
 * Create a meta icon link.
 *
 * @param $title
 *   The title of link.
 * @param $href
 *   The link href path.
 * @param $id
 *   The link ID.
 * @return
 *   A renderable array defining a meta icon link.
 */
function conduit_view_meta_link($title, $href, $id) {
  $container = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'conduit-' . $id,
      'class' => array('conduit-icon'),
    ),
  );
  return array(
    '#type' => 'link',
    '#title' => drupal_render($container),
    '#href' => $href,
    '#options' => array('html' => TRUE),
    '#attributes' => array(
      'title' => $title,
    ),
  );
}

/**
 * Find the siblings of the current revision.
 *
 * @param $current
 *   The node object of the current revision.
 * @param $revisions
 *   An array of revisions objects from node_revision_list().
 * @return
 *   A two element array contining the two sibling reivision objects. If a
 *   sibling does not existing, aka on the end, then it will be FALSE.
 */
function conduit_view_meta_siblings_revisions($current, $revisions) {
  foreach ($revisions as $index => $revision) {
    if ($revision->vid == $current->vid) {
      return array(isset($revisions[$i = $index - 1]) ? $revisions[$i] : FALSE,
        isset($revisions[$i = $index + 1]) ? $revisions[$i] : FALSE);
    }
  }
}

/**
 * Determine the siblings (previous and next) jobs within the parent group.
 *
 * @param $self
 *   Current job node ID.
 * @param $parent
 *   Parent group ID.
 * @return
 *   A two element array containing the two sibling nodes or FALSE if the
 *   parent group only contains a single job.
 */
function conduit_view_meta_siblings_group($self, $parent) {
  $view = views_get_view('conduit_job');
  $view->set_current_page(0); // Ensure that any pagers do not interfere.
  $view->set_items_per_page(200); // @TODO Probably want to make util and have it cycle through page or something.
  $view->set_arguments(array($parent));
  $view->execute();
  if (($count = count($view->result)) == 1) {
    return FALSE;
  }
  foreach ($view->result as $index => $result) {
    if ($result->nid == $self) {
      return array(isset($view->result[$i = $index - 1]) ? $view->result[$i] : $view->result[$count - 1],
        isset($view->result[$i = $index + 1]) ? $view->result[$i] : $view->result[0]);
    }
  }
}

/**
 * Provide a repeat image button form.
 *
 * @param $node
 *   Job node to repeat.
 * @param $revision
 *   TRUE if node represents a revision, otherwise FALSE.
 */
function conduit_view_repeat_form($form, &$form_state, $node, $revision) {
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['submit'] = array(
    '#type' => 'image_button',
    '#src' => drupal_get_path('module', 'conduit') . '/images/repeat.png',
    '#attributes' => array(
      'alt' => t('Repeat job'),
      'title' => t('Repeat job'),
    ),
    '#access' => !$revision && conduit_node_repeat_access($node),
  );
  return $form;
}

/**
 * Check user preferences and either redirect to confirmation or repeat.
 */
function conduit_view_repeat_form_submit($form, &$form_state) {
  // If the user preference is to be asked for confirmation then redirect to
  // the repeat confirmation form.
  global $user;
  if (user_load($user->uid)->conduit_ask[LANGUAGE_NONE][0]['value']) {
    drupal_goto('node/' . $form_state['values']['nid'] . '/repeat');
  }

  // If no confirmation is required then execute repeat the job.
  conduit_view_repeat_confirm_form_submit($form, $form_state);
}

/**
 * Ask for confirmation before repeating a job.
 *
 * @param $node
 *   The job node to be repeated.
 */
function conduit_view_repeat_confirm_form($form, &$form_state, $node) {
  global $user;
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['ask'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always ask for confirmation before repeating a job'),
    '#description' => t('If unchecked the next time the repeat button is pressed the job will be repeated without a confirmation. This setting may be changed in your <a href="@profile_url">profile<a>.',
      array('@profile_url' => url('user/' . $user->uid . '/edit', array('fragment' => 'edit-conduit-ask-und')))),
    '#default_value' => TRUE,
    '#weight' => 1,
  );
  return confirm_form($form, t('Are you sure you want to repeat %title job?', array('%title' => $node->title)), 'node/' . $node->nid, FALSE, t('Repeat'));
}

/**
 * Repeat a job and check for user preference change.
 */
function conduit_view_repeat_confirm_form_submit($form, &$form_state) {
  // If the user no longer wishes to be asked for confirmation then update the
  // user's account with the new preference.
  if (isset($form_state['values']['ask']) && !$form_state['values']['ask']) {
    global $user;
    $edit = array();
    $edit['conduit_ask'][LANGUAGE_NONE][0]['value'] = (int) FALSE;
    user_save(user_load($user->uid), $edit);
  }

  // Attempt to repeat the job and if successful display a message.
  conduit_include('api');
  if (conduit_api_repeat($form_state['values']['nid'])) {
    drupal_set_message(t('A new revision has been created.'));
  }

  // Always redirect back to the job node.
  $form_state['redirect'] = 'node/' . $form_state['values']['nid'];
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
    $breadcrumb[] = l(format_username(user_load($node->uid)), 'user/' . $node->uid . '/groups');
  }
  else {
    $breadcrumb = conduit_view_breadcrumb(node_load($node->conduit_parent[LANGUAGE_NONE][0]['nid']), $breadcrumb);
  }
  return $breadcrumb;
}

/**
 * Determine if user has access to repeat node.
 *
 * The node must be a job and conduit_done must be TRUE, the user must have
 * view access to the node and the 'repeat conduit job' permission.
 *
 * @param $node
 *   A node object or NID.
 * @param $revision
 *   (Optional) A boolean flag to indicate if the node object represents one of
 *   the previous revisions instead of the current revision.
 * @return
 *   TRUE if access is granted, otherwise FALSE.
 */
function conduit_node_repeat_access($node) {
  if (is_numeric($node)) {
    $node = node_load($node);
  }

  $access = &drupal_static(__FUNCTION__, array());
  if (!isset($access[$node->nid])) {
    $access[$node->nid] = conduit_node_properties_access($node) && $node->type != 'conduit_group' && user_access('repeat conduit job') && $node->conduit_done[LANGUAGE_NONE][0]['value'];
  }
  return $access[$node->nid];
}

/**
 * Determine if node is a conduit type and the user has view access.
 *
 * @param $node
 *   A node object.
 * @param $revision
 *   (Optional) A boolean flag to indicate if the node object represents one of
 *   the previous revisions instead of the current revision.
 * @return
 *   TRUE if access is granted, otherwise FALSE.
 */
function conduit_node_properties_access($node, $revision = FALSE) {
  $access = &drupal_static(__FUNCTION__, array());
  if (!isset($access[$node->vid])) {
    $access[$node->vid] = conduit_node_type($node->type) && ($revision ? _node_revision_access($node) : node_access('view', $node));
  }
  return $access[$node->vid];
}

/**
 * Display properties, both 'self' and 'merged'.
 */
function conduit_node_properties($node) {
  $page = array();
  drupal_set_title(t('Properties'));

  if ($node->type != 'conduit_group') {
    $page['job_id'] = array(
      '#type' => 'item',
      '#title' => t('Job ID'),
      '#markup' => $node->nid,
    );
  }
  $page['conduit_properties'] = array(
    '#type' => 'item',
    '#title' => t('Self'),
    '#markup' => serial_display($node->conduit_properties[LANGUAGE_NONE][0]['value']),
    '#access' => (bool) $node->conduit_properties[LANGUAGE_NONE][0]['value'],
  );
  $page['conduit_properties_merged'] = array(
    '#type' => 'item',
    '#title' => t('Merged'),
    '#markup' => serial_display($node->conduit_properties_merged),
    '#access' => $node->conduit_properties[LANGUAGE_NONE][0]['value'] != $node->conduit_properties_merged,
  );
  return $page;
}
