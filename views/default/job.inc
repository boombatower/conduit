<?php

$view = new view;
$view->name = 'conduit_job';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '15';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Empty text: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'No jobs';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'No jobs were found.';
$handler->display->display_options['empty']['area']['format'] = 'plain_text';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Fields: conduit_done */
$handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['table'] = 'field_data_conduit_done';
$handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id']['label'] = 'Done';
$handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id']['settings'] = array(
  'prefix_suffix' => 1,
);
/* Field: Fields: conduit_pass */
$handler->display->display_options['fields']['entity_id_1']['id'] = 'entity_id_1';
$handler->display->display_options['fields']['entity_id_1']['table'] = 'field_data_conduit_pass';
$handler->display->display_options['fields']['entity_id_1']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id_1']['label'] = 'Pass';
$handler->display->display_options['fields']['entity_id_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['entity_id_1']['settings'] = array(
  'prefix_suffix' => 1,
);
/* Field: Fields: conduit_summary */
$handler->display->display_options['fields']['entity_id_2']['id'] = 'entity_id_2';
$handler->display->display_options['fields']['entity_id_2']['table'] = 'field_data_conduit_summary';
$handler->display->display_options['fields']['entity_id_2']['field'] = 'entity_id';
$handler->display->display_options['fields']['entity_id_2']['label'] = 'Summary';
$handler->display->display_options['fields']['entity_id_2']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['trim'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['max_length'] = '128';
$handler->display->display_options['fields']['entity_id_2']['alter']['word_boundary'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['ellipsis'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['entity_id_2']['alter']['html'] = 0;
$handler->display->display_options['fields']['entity_id_2']['hide_empty'] = 0;
$handler->display->display_options['fields']['entity_id_2']['empty_zero'] = 0;
/* Argument: Fields: conduit_parent (conduit_parent) - nid */
$handler->display->display_options['arguments']['conduit_parent_nid']['id'] = 'conduit_parent_nid';
$handler->display->display_options['arguments']['conduit_parent_nid']['table'] = 'field_data_conduit_parent';
$handler->display->display_options['arguments']['conduit_parent_nid']['field'] = 'conduit_parent_nid';
$handler->display->display_options['arguments']['conduit_parent_nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['conduit_parent_nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['conduit_parent_nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['conduit_parent_nid']['validate_type'] = 'node';
$handler->display->display_options['arguments']['conduit_parent_nid']['validate_options']['types'] = array(
  'conduit_group' => 'conduit_group',
);
$handler->display->display_options['arguments']['conduit_parent_nid']['validate_options']['access'] = '1';
$handler->display->display_options['arguments']['conduit_parent_nid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['conduit_parent_nid']['not'] = 0;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = drupal_map_assoc(array_filter(array_keys(node_type_get_types()), 'conduit_job_info'));
/* Filter: Node: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'node';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['operator'] = 'contains';
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Job Title';
$handler->display->display_options['filters']['title']['expose']['use_operator'] = FALSE;
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
$handler->display->display_options['filters']['title']['case'] = 0;
