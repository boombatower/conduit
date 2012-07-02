<?php

/**
 * @file
 * Bulk export of relation_default objects generated by Bulk export module.
 */

/**
 * Implementation of hook_relation_default_relation_types()
 */
function conduit_coverage_relation_default_relation_types() {
  $relation_types = array();

  $relation_type = new stdClass;
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'conduit_coverage';
  $relation_type->label = 'Conduit coverage';
  $relation_type->reverse_label = 'Conduit coverage';
  $relation_type->directional = 0;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 1;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'conduit_coverage:*',
    1 => 'node:*',
  );
  $relation_type->target_bundles = array();
  $relation_types['conduit_coverage'] = $relation_type;

  return $relation_types;
}