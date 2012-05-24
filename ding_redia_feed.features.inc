<?php

/**
 * @file
 * Missing description.
 */

/**
 * Implementation of hook_views_api().
 */
function ding_redia_feed_views_api() {
  return array(
    'api' => '2',
  );
}

/**
 * Implementation of hook_flag_default_flags().
 */
function ding_redia_feed_flag_default_flags() {
  $flags = array();
  // Exported flag: "Mobil-app indhold".
  $flags['ding_redia_feed_mobileapp'] = array(
    'content_type' => 'node',
    'title' => 'Mobil-app indhold',
    'global' => '1',
    'types' => array(
      '0' => 'article',
      '1' => 'event',
    ),
    'flag_short' => 'Vis i mobil-app',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Fjern Fra mobil-app',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 5,
        '1' => 6,
      ),
      'unflag' => array(
        '0' => 5,
        '1' => 6,
      ),
    ),
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'api_version' => 2,
    'module' => 'ding_redia_feed',
    'locked' => array(
      '0' => 'name',
    ),
  );
  return $flags;
}

/**
 * Implementation of hook_rules_defaults().
 */
function ding_redia_feed_rules_defaults() {
  return array(
    'rules' => array(
      'rules_ding_redia_feed_mobileapp_flagged' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_flagged_ding_redia_feed_mobileapp',
        '#label' => 'A node was flagged for display in the mobile app.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'ding_redia_feed',
          'ding_redia_feed' => 'ding_redia_feed',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Clear the cache of a specific view.',
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#settings' => array(
              'view' => 'ding_redia_feed_mobileapp',
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'view' => array(
                    '0' => ':global',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'rules_ding_redia_feed_mobileapp_unflagged' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_unflagged_ding_redia_feed_mobileapp',
        '#label' => 'A node was unflagged from display in the mobile app.',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'ding_redia_feed',
          'ding_redia_feed' => 'ding_redia_feed',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' => array(
              'view' => 'ding_redia_feed_mobileapp',
              '#eval input' => array(
                'token_rules_input_evaluator' => array(
                  'view' => array(
                    '0' => ':global',
                  ),
                ),
              ),
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#info' => array(
              'label' => 'Clear the cache of a specific view.',
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}
