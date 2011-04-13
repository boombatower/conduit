<?php

$view = new view;
$view->name = 'conduit_usage';
$view->description = '';
$view->tag = '';
$view->base_table = 'node_revision';
$view->human_name = 'Conduit usage';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Usage';
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title_1' => 'title_1',
  'conduit_duration_value_1' => 'conduit_duration_value_1',
  'timestamp' => 'timestamp',
);
$handler->display->display_options['style_options']['default'] = 'timestamp';
$handler->display->display_options['style_options']['info'] = array(
  'title_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'conduit_duration_value_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['order'] = 'desc';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'No usage';
$handler->display->display_options['empty']['area']['empty'] = FALSE;
$handler->display->display_options['empty']['area']['content'] = 'No usage history was found.';
$handler->display->display_options['empty']['area']['format'] = 'plain_text';
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['required'] = 0;
/* Field: Content revision: Title */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'node_revision';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['title_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['title_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['title_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['title_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['title_1']['link_to_node'] = 0;
$handler->display->display_options['fields']['title_1']['link_to_node_revision'] = 1;
/* Field: Fields (historical data): Duration (conduit_duration) */
$handler->display->display_options['fields']['conduit_duration_value_1']['id'] = 'conduit_duration_value_1';
$handler->display->display_options['fields']['conduit_duration_value_1']['table'] = 'field_revision_conduit_duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['field'] = 'conduit_duration_value';
$handler->display->display_options['fields']['conduit_duration_value_1']['label'] = 'Duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['empty_zero'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['granularity'] = '2';
/* Field: Content revision: Created date */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'node_revision';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'Completed';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
$handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['date_format'] = 'time ago';
/* Contextual filter: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['specify_validation'] = 1;
$handler->display->display_options['arguments']['uid']['validate']['type'] = 'user_conduit';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = 0;
$handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
  3 => '3',
);
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Filter criterion: Content: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = drupal_map_assoc(array_filter(array_keys(node_type_get_types()), 'conduit_job_info'));
/* Filter criterion: Fields (historical data): Done (conduit_done) */
$handler->display->display_options['filters']['conduit_done_value']['id'] = 'conduit_done_value';
$handler->display->display_options['filters']['conduit_done_value']['table'] = 'field_revision_conduit_done';
$handler->display->display_options['filters']['conduit_done_value']['field'] = 'conduit_done_value';
$handler->display->display_options['filters']['conduit_done_value']['value']['value'] = '1';
/* Filter criterion: Content revision: Created date */
$handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['table'] = 'node_revision';
$handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['operator'] = '>=';
$handler->display->display_options['filters']['timestamp']['value']['min'] = '-1 day';
$handler->display->display_options['filters']['timestamp']['value']['max'] = 'now';
$handler->display->display_options['filters']['timestamp']['value']['value'] = '-1 month';
$handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
$handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
$handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
$handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Completed';
$handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
$handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['expose']['multiple'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['exposed_block'] = FALSE;
$handler->display->display_options['exposed_block'] = TRUE;
$handler->display->display_options['path'] = 'user/%/usage';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Usage';
$handler->display->display_options['menu']['weight'] = '0';

/* Display: Summary */
$handler = $view->new_display('attachment', 'Summary', 'summary');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Fields (historical data): Duration (conduit_duration) */
$handler->display->display_options['fields']['conduit_duration_value']['id'] = 'conduit_duration_value';
$handler->display->display_options['fields']['conduit_duration_value']['table'] = 'field_revision_conduit_duration';
$handler->display->display_options['fields']['conduit_duration_value']['field'] = 'conduit_duration_value';
$handler->display->display_options['fields']['conduit_duration_value']['group_type'] = 'count';
$handler->display->display_options['fields']['conduit_duration_value']['label'] = 'Count';
$handler->display->display_options['fields']['conduit_duration_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['element_label_type'] = 'strong';
$handler->display->display_options['fields']['conduit_duration_value']['element_label_colon'] = 1;
$handler->display->display_options['fields']['conduit_duration_value']['element_default_classes'] = 1;
$handler->display->display_options['fields']['conduit_duration_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['conduit_duration_value']['empty_zero'] = 0;
/* Field: Fields (historical data): Duration (conduit_duration) */
$handler->display->display_options['fields']['conduit_duration_value_1']['id'] = 'conduit_duration_value_1';
$handler->display->display_options['fields']['conduit_duration_value_1']['table'] = 'field_revision_conduit_duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['field'] = 'conduit_duration_value';
$handler->display->display_options['fields']['conduit_duration_value_1']['group_type'] = 'sum';
$handler->display->display_options['fields']['conduit_duration_value_1']['label'] = 'Duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_label_type'] = 'strong';
$handler->display->display_options['fields']['conduit_duration_value_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['empty_zero'] = 0;
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'default' => 0,
);

/* Display: Summary type */
$handler = $view->new_display('attachment', 'Summary type', 'summary_type');
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'type' => 'type',
  'conduit_duration_value_1' => 'conduit_duration_value_1',
);
$handler->display->display_options['style_options']['default'] = 'type';
$handler->display->display_options['style_options']['info'] = array(
  'type' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'conduit_duration_value_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['footer'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['link_to_node'] = 0;
$handler->display->display_options['fields']['type']['machine_name'] = 0;
/* Field: Content: Type */
$handler->display->display_options['fields']['type_1']['id'] = 'type_1';
$handler->display->display_options['fields']['type_1']['table'] = 'node';
$handler->display->display_options['fields']['type_1']['field'] = 'type';
$handler->display->display_options['fields']['type_1']['group_type'] = 'count';
$handler->display->display_options['fields']['type_1']['label'] = 'Count';
$handler->display->display_options['fields']['type_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['type_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['type_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['type_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['type_1']['empty_zero'] = 0;
/* Field: Fields (historical data): Duration (conduit_duration) */
$handler->display->display_options['fields']['conduit_duration_value_1']['id'] = 'conduit_duration_value_1';
$handler->display->display_options['fields']['conduit_duration_value_1']['table'] = 'field_revision_conduit_duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['field'] = 'conduit_duration_value';
$handler->display->display_options['fields']['conduit_duration_value_1']['group_type'] = 'sum';
$handler->display->display_options['fields']['conduit_duration_value_1']['label'] = 'Duration';
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['external'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['trim'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['nl2br'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['alter']['html'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_label_colon'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['element_default_classes'] = 1;
$handler->display->display_options['fields']['conduit_duration_value_1']['hide_empty'] = 0;
$handler->display->display_options['fields']['conduit_duration_value_1']['empty_zero'] = 0;
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'default' => 0,
);
$translatables['conduit_usage'] = array(
  t('Defaults'),
  t('Usage'),
  t('more'),
  t('Filter'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('No usage'),
  t('No usage history was found.'),
  t('user'),
  t('Title'),
  t('Duration'),
  t('Completed'),
  t('All'),
  t('Page'),
  t('Summary'),
  t('Count'),
  t('Summary type'),
  t('Type'),
);
