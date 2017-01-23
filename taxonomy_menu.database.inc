<?php
/**
 * @file
 * Database functions
 */

 /**
 *  helper function to insert a menu item
 *
 * @param $mlid
 * @param $tid
 * @param $vid
 */
function _taxonomy_menu_insert_menu_item($mlid, $tid, $vid) {
  $fields = array(
    'mlid' => $mlid,
    'tid' => $tid,
    'vid' => $vid,
  );
  db_insert('taxonomy_menu')->fields($fields)->execute();
}

/**
 * Return the corresponding menu link id.
 *
 * @param $tid
 *   the term's id
 */
function _taxonomy_menu_get_mlid($tid, $vid) {
  $where = array(
    ':tid' => $tid,
    ':vid' => $vid,
  );
  return db_query('SELECT mlid FROM {taxonomy_menu} WHERE tid = :tid AND vid = :vid', $where)->fetchField();
}

/**
 * Retrieve the term / menu relations for a vocab.
 *
 * @param $vid
 *   vocabulary's id
 * @return
 *   array(tid => mlid)
 */
function _taxonomy_menu_get_menu_items($vid) {
  $result = db_query('SELECT tid, mlid FROM {taxonomy_menu} WHERE vid = :vid', array(':vid' => $vid));
  $menu_items = array();
  $menu_items = $result->fetchAllKeyed();
  return $menu_items;
}

 /**
  * Delete all links associated with this vocab from both the taxonomy_menu
  * table and the menu_link table.
  *
  * @param $vid
  *   vocabulary's id
  */
function _taxonomy_menu_delete_all($vid) {
  $menu_items = _taxonomy_menu_get_menu_items($vid);
  if (!empty($menu_items)) {
    // Deletes all the menu items.
    $mlids = array_keys($menu_items);
    foreach ($mlids as $mlid) {
      menu_link_delete($mlid);
    }
    // Deletes all the taxonoy_menu items.
    db_delete('taxonomy_menu')
      ->condition('vid', $vid)
      ->execute();
  }
}

/**
 * Get an array of the tid's related to the node
 *
 * @param $node
 * @return array of tids
 */
function _taxonomy_menu_get_node_terms($node) {
  // Get the taxonomy fields.
  $tids = array();
  $result = db_query("SELECT field_name FROM {field_config} WHERE type = 'taxonomy_term_reference'");
  foreach ($result as $field) {
    $field_name = $field->field_name;

    if (isset($node->$field_name)) {
      $tid_field = $node->$field_name;
      // Loop through all the languages.

      foreach ($tid_field as $tid_field_languages) {
        // Loop through all the tids

        foreach ($tid_field_languages as $tid) {
          $tids[] = $tid['tid'];
        }
      }
    }
  }
  return $tids;
}

/**
 * Get an array of the tid's from the parent
 *
 * @param $tid
 * @return array of tid
 */
function _taxonomy_menu_get_parents($tid) {
  $output = array();
  $result = taxonomy_get_parents($tid);
  foreach ($result as $key => $item) {
    $output[] = $key;
  }
  return $output;
}

/**
  * Delete all rows from {taxomony_menu} associated with this tid
  *
  * @param $vid
  * @param $tid
  */
function _taxonomy_menu_delete_item($vid, $tid) {
  $and = db_and()->condition('vid', $vid)->condition('tid', $tid);
  db_delete('taxonomy_menu')->condition($and)->execute();
}

/**
 * Get all of the tid for a given vid
 *
 * @param $vid
 * @return array of $tid
 */
function _taxonomy_menu_get_terms($vid) {
  $result = db_select('taxonomy_term_data', 'td')
    ->condition('vid', $vid)
    ->fields('td', array('tid'))
    ->execute();
  return $result->fetchAll();
}

/**
 * @TODO Needs Updating since terms are related via node fields
 *
 * used to get the count without children
 *
 * @param int $tid
 *   Taxonomy term ID.
 *
 * @return int
 *  Count of nodes that reference the term.
 */
function _taxonomy_menu_term_count($tid) {
  // Construct a cache ID
  $cid = 'taxonomy_menu:term_count:' . $tid;

  // Try to get the count from the cache.
  $cache = cache_get($cid);
  if ($cache) {
    return $cache->data;
  }

  // It is not in the cache, so issue a query.
  $result = db_select('taxonomy_index', 'tn');
  $result->condition('tid', $tid);
  $result->join('node', 'n', 'n.nid = tn.nid AND n.status = 1');
  $result->addExpression('COUNT(n.nid)', 'term_count');
  $temp = $result->execute();
  $temp = $temp->fetchObject();
  $data = $temp->term_count;

  // Store the count in the cache.
  cache_set($cid, $data, 'cache', REQUEST_TIME + 1215);

  // Return the result.
  return $data;
}

/**
 * Get tid for a given mlid
 *
 * @param $mlid
 * @return $tid
 */
function _taxonomy_menu_get_tid($mlid) {
  $where = array(
    ':mlid' => $mlid,
  );
  return db_query('SELECT tid FROM {taxonomy_menu} WHERE mlid = :mlid', $where)->fetchField();
}

/**
 * Get vid, tid for a given mlid
 *
 * @param $mlid
 * @return array of vid, tid
 */
function _taxonomy_menu_get_item($mlid) {
  $result = db_select('taxonomy_menu', 'tm')
    ->condition('mlid', $mlid, '=')
    ->fields('tm', array('tid', 'vid'))
    ->execute();
  return $result->fetch();
}

/**
 * Get the vocabulary for a tid
 * @param $tid array of tids
 * @return $vid
 */
function _taxonomy_menu_get_vid_by_tid($tids) {
  if ($tids) {
    $result = db_select('term_data')
      ->condition('tid', $tids, 'IN')
      ->fields('term_data', array('vid'))
      ->distinct()
      ->execute();
    $vids = array();
    return $result->fetchAllAssoc('vid');
  }
}

/**
 * Options functions
 */
