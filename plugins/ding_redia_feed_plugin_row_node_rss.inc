<?php
/**
 * @file
 * Contains the node RSS row style plugin.
 */

/**
 * Plugin which performs a node_view on the resulting object
 * and formats it as an RSS item.
 */
class ding_redia_feed_plugin_row_node_rss extends views_plugin_row_node_rss {
  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['item_length'] = array(
      '#type' => 'select',
      '#title' => t('Display type'),
      '#options' => array(
        'default' => t('Redia special RSS markup'),
      ),
      '#default_value' => $this->options['item_length'],
    );
  }

  function render($row) {
    // For the most part, this code is taken from node_feed() in node.module
    global $base_url;

    $nid = $row->{$this->field_alias};
    if (!is_numeric($nid)) {
      return;
    }

    $item_length = $this->options['item_length'];
    if ($item_length == 'default') {
      $item_length = variable_get('feed_item_length', 'teaser');
    }

    // Load the specified node:
    $node = node_load($nid);
    if (empty($node)) {
      return;
    }
    if (!empty($node->field_library_ref[0]['nid'])) {
      $library = node_load($node->field_library_ref[0]['nid']);
    }

    $node->build_mode = NODE_BUILD_RSS;

    if ($item_length != 'title') {
      $teaser = ($item_length == 'teaser') ? TRUE : FALSE;

      // Filter and prepare node teaser
      if (node_hook($node, 'view')) {
        $node = node_invoke($node, 'view', $teaser, FALSE);
      }
      else {
        $node = node_prepare($node, $teaser);
      }

      // Allow modules to change $node->teaser before viewing.
      node_invoke_nodeapi($node, 'view', $teaser, FALSE);
    }

    // Set the proper node part, then unset unused $node part so that a bad
    // theme can not open a security hole.
    $content = drupal_render($node->content);
    if ($teaser) {
      $node->teaser = $content;
      unset($node->body);
    }
    else {
      $node->body = $content;
      unset($node->teaser);
    }

    // Allow modules to modify the fully-built node.
    node_invoke_nodeapi($node, 'alter', $teaser, FALSE);
    $item = new stdClass();
    $item->title = $node->title;
    $item->link = url("node/$row->nid", array('absolute' => TRUE));
    $item->nid = $node->nid;
    $item->readmore = $node->readmore;

    // Allow modules to add additional item fields and/or modify $item
    $extra = node_invoke_nodeapi($node, 'rss item');
    $item->elements = array_merge($extra,
      array(
        array('key' => 'pubDate', 'value' => gmdate('r', $node->created)),
        array(
          'key' => 'dc:creator',
          'value' => $node->name,
          'namespace' => array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/'),
        ),
        array(
          'key' => 'guid',
          'value' => $node->nid . ' at ' . $base_url,
          'attributes' => array('isPermaLink' => 'false')
        ),
      )
    );

    // Add custom Redia properties.
    if (!empty($node->field_list_image[0]['fid'])) {
      $file = $node->field_list_image[0];
      $item->elements[] = array(
        'key' => 'media:content',
        'namespace' => array('xmlns:media' => 'http://search.yahoo.com/mrss/'),
        'attributes' => array(
          'url' => url($file['filepath'], array('absolute' => TRUE)),
          'fileSize' => $file['filesize'],
          'type' => $file['filemime'],
          'medium' => 'image',
        ),
      );
    }
    if ($node->type == 'event' && isset($library)) {
      $item->elements[] = array(
        'key' => 'redia-rss:arrangement-location',
        'namespace' => array('xmlns:redia-rss' => 'http://redia.dk'),
        'value' => $library->location['street'] . ' ' . $library->location['postal_code'] . ' ' . $library->location['city'],
      );
    }
    if ($node->type == 'article' && !empty($node->ding_redia_feed_cat)) {
      $item->elements[] = array(
        'key' => 'redia-rss:category-id',
        'namespace' => array('xmlns:redia-rss' => 'http://redia.dk'),
        'value' => $node->ding_redia_feed_cat,
      );
    }
    if (!empty($node->field_datetime[0])) {
      $item->elements[] = array(
        'key' => 'redia-rss:arrangement-starttime',
        'namespace' => array('xmlns:redia-rss' => 'http://redia.dk'),
        'value' => strtotime($node->field_datetime[0]['value']),
      );
      $item->elements[] = array(
        'key' => 'redia-rss:arrangement-endtime',
        'namespace' => array('xmlns:redia-rss' => 'http://redia.dk'),
        'value' => strtotime($node->field_datetime[0]['value2']),
      );
    }

    foreach ($item->elements as $element) {
      if (isset($element['namespace'])) {
        $this->view->style_plugin->namespaces = array_merge($this->view->style_plugin->namespaces, $element['namespace']);
      }
    }

    // Prepare the item description
    switch ($item_length) {
      case 'fulltext':
        $item->description = $node->body;
        break;
      case 'teaser':
        $item->description = $node->teaser;
        if (!empty($item->readmore)) {
          $item->description .= '<p>' . l(t('read more'), 'node/' . $item->nid, array('absolute' => TRUE, 'attributes' => array('target' => '_blank'))) . '</p>';
        }
        break;
      case 'title':
        $item->description = '';
        break;
    }

    return theme($this->theme_functions(), $this->view, $this->options, $item);
  }
}

