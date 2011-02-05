<?php
$view = new view;
$view->name = 'conduit_log';
$view->description = '';
$view->tag = '';
$view->base_table = 'field_data_conduit_log';
$view->human_name = 'Conduit Log';
$view->core = 7;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'view conduit log';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'delta_raw' => 'delta_raw',
  'conduit_log_value_raw' => 'conduit_log_value_raw',
);
$handler->display->display_options['style_options']['default'] = 'delta_raw';
$handler->display->display_options['style_options']['info'] = array(
  'delta_raw' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'conduit_log_value_raw' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: Field tables: conduit_log=>Delta */
$handler->display->display_options['fields']['delta_raw']['id'] = 'delta_raw';
$handler->display->display_options['fields']['delta_raw']['table'] = 'field_data_conduit_log';
$handler->display->display_options['fields']['delta_raw']['field'] = 'delta_raw';
$handler->display->display_options['fields']['delta_raw']['label'] = 'Delta';
$handler->display->display_options['fields']['delta_raw']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['delta_raw']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['delta_raw']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['delta_raw']['alter']['trim'] = 0;
$handler->display->display_options['fields']['delta_raw']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['delta_raw']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['delta_raw']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['delta_raw']['alter']['html'] = 0;
$handler->display->display_options['fields']['delta_raw']['element_label_colon'] = 1;
$handler->display->display_options['fields']['delta_raw']['element_default_classes'] = 1;
$handler->display->display_options['fields']['delta_raw']['hide_empty'] = 0;
$handler->display->display_options['fields']['delta_raw']['empty_zero'] = 0;
/* Field: Field tables: conduit_log=>Value */
$handler->display->display_options['fields']['conduit_log_value_raw']['id'] = 'conduit_log_value_raw';
$handler->display->display_options['fields']['conduit_log_value_raw']['table'] = 'field_data_conduit_log';
$handler->display->display_options['fields']['conduit_log_value_raw']['field'] = 'conduit_log_value_raw';
$handler->display->display_options['fields']['conduit_log_value_raw']['label'] = 'Log';
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['trim'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['alter']['html'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['element_type'] = 'div';
$handler->display->display_options['fields']['conduit_log_value_raw']['element_label_colon'] = 1;
$handler->display->display_options['fields']['conduit_log_value_raw']['element_default_classes'] = 1;
$handler->display->display_options['fields']['conduit_log_value_raw']['hide_empty'] = 0;
$handler->display->display_options['fields']['conduit_log_value_raw']['empty_zero'] = 0;
/* Argument: Field tables: conduit_log=>Entity id */
$handler->display->display_options['arguments']['entity_id_raw']['id'] = 'entity_id_raw';
$handler->display->display_options['arguments']['entity_id_raw']['table'] = 'field_data_conduit_log';
$handler->display->display_options['arguments']['entity_id_raw']['field'] = 'entity_id_raw';
$handler->display->display_options['arguments']['entity_id_raw']['default_action'] = 'not found';
$handler->display->display_options['arguments']['entity_id_raw']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['entity_id_raw']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['entity_id_raw']['validate_type'] = 'node';
$handler->display->display_options['arguments']['entity_id_raw']['validate_options']['types'] = drupal_map_assoc(array_filter(array_keys(node_type_get_types()), 'conduit_job_info'));;
$handler->display->display_options['arguments']['entity_id_raw']['validate_options']['access'] = 0;
$handler->display->display_options['arguments']['entity_id_raw']['break_phrase'] = 0;
$handler->display->display_options['arguments']['entity_id_raw']['not'] = 0;

/* Display: Log page */
$handler = $view->new_display('page', 'Log page', 'page_log');
$handler->display->display_options['path'] = 'node/%/log';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Log';
$handler->display->display_options['menu']['weight'] = '1';
$translatables['conduit_log'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort By'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Delta'),
  t('Log'),
  t('All'),
  t('Log page'),
);
