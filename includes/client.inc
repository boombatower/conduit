<?php

/**
 * Implements hook_form(): conduit_client.
 */
function conduit_client_form($node, $form_state) {
  return node_content_form($node, $form_state);
}

/**
 * Implements hook_form_FORM_ID_alter(): conduit_client_node_form.
 */
function conduit_form_conduit_client_node_form_alter(&$form, &$form_state) {
  $form['title']['#description'] = t('Displayed when referencing client.');

  // Move all client fields into a fieldset.
  $form['client'] = array(
    '#type' => 'fieldset',
    '#title' => t('Client'),
  );
  foreach (array('title', 'conduit_key') as $key) {
    $form['client'][$key] = $form[$key];
    unset($form[$key]);
  }
}

/**
 * Implements hook_validate(): conduit_client.
 */
function conduit_client_validate($node, $form = array()) {
  if (form_get_errors()) {
    return;
  }

  $key = $node->conduit_key[LANGUAGE_NONE][0]['value'];
  if (drupal_strlen($key) < 32 || drupal_strlen($key) > 64) {
    form_set_error('conduit_key', t('Key must be at least 32 and no more then 64 characters in length.'));
  }
}
