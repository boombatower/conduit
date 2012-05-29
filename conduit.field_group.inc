<?php
/**
 * @file
 * Provide field_group definitions.
 */

/**
 * Implements hook_field_group_info().
 */
function conduit_field_group_info() {
  $field_groups = array();

  // Create a meta fieldset on the form mode for each conduit node type.
  foreach (conduit_types() as $type => $title) {
    $field_group = new stdClass();
    $field_group->api_version = 1;
    $field_group->group_name = ($info = conduit_job_info($type)) ? $info['module'] : $type;
    $field_group->identifier = $field_group->group_name . '|node|' . $type . '|form';
    $field_group->entity_type = 'node';
    $field_group->bundle = $type;
    $field_group->mode = 'form';
    $field_group->parent_name = '';
    $field_group->data = array(
      'label' => 'Meta',
      'weight' => '0',
      'children' => array(
        'conduit_parent',
        'conduit_properties',
        'title',
      ),
      'format_type' => 'fieldset',
    );
    $field_groups[$field_group->identifier] = $field_group;
  }

  return $field_groups;
}
