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

  $form['#validate'][] = 'conduit_client_node_form_validate';
}
