<?php

$view = new view;
$view->name = 'conduit_job';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = 'Conduit job';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '15';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '1';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['row_class'] = 'conduit-status-[entity_id_3]';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'entity_id_3' => 'entity_id_3',
  'entity_id_2' => 'entity_id_2',
);
$handler->display->display_options['style_options']['default'] = 'title';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entity_id_3' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'entity_id_2' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['exposed_block'] = TRUE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'No jobs';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'No jobs were found.';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Status */
$handler->display->display_options['fields']['entity_id_3']['id'] = 'entity_id_3';
$handler->display->display_options['fields']['entity_id_3']['table'] = 'field_data_conduit_status';
$handler->display->display_options['fields']['entity_id_3']['field'] = 'conduit_status';
$handler->display->display_options['fields']['entity_id_3']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id_3']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id_3']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id_3']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['entity_id_3']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['entity_id_3']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id_3']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id_3']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id_3']['element_label_colon'] = 1;
$handler->display->display_options['fields']['entity_id_3']['element_default_classes'] = 1;
$handler->display->display_options['fields']['entity_id_3']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id_3']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id_3']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Field: Content: Summary */
$handler->display->display_options['fields']['entity_id_2']['id'] = 'entity_id_2';
$handler->display->display_options['fields']['entity_id_2']['table'] = 'field_data_conduit_summary';
$handler->display->display_options['fields']['entity_id_2']['field'] = 'conduit_summary';
$handler->display->display_options['fields']['entity_id_2']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['max_length'] = '128';
$handler->display->display_options['fields']['entity_id_2']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id_2']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id_2']['empty_zero'] = 0;
/* Contextual filter: Content: Parent (conduit_parent) */
$handler->display->display_options['arguments']['conduit_parent_target_id']['id'] = 'conduit_parent_target_id';
$handler->display->display_options['arguments']['conduit_parent_target_id']['table'] = 'field_data_conduit_parent';
$handler->display->display_options['arguments']['conduit_parent_target_id']['field'] = 'conduit_parent_target_id';
$handler->display->display_options['arguments']['conduit_parent_target_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['conduit_parent_target_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['conduit_parent_target_id']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['conduit_parent_target_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['conduit_parent_target_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['conduit_parent_target_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['conduit_parent_target_id']['specify_validation'] = 1;
$handler->display->display_options['arguments']['conduit_parent_target_id']['validate']['type'] = 'node';
$handler->display->display_options['arguments']['conduit_parent_target_id']['validate_options']['types'] = array(
  'conduit_group' => 'conduit_group',
);
$handler->display->display_options['arguments']['conduit_parent_target_id']['validate_options']['access'] = '1';
$handler->display->display_options['arguments']['conduit_parent_target_id']['break_phrase'] = 0;
$handler->display->display_options['arguments']['conduit_parent_target_id']['not'] = 0;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = drupal_map_assoc(array_filter(array_keys(node_type_get_types()), 'conduit_job_info'));
/* Filter criterion: Content: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'node';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['operator'] = 'contains';
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Job';
$handler->display->display_options['filters']['title']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'job_title';
