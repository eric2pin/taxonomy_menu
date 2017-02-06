<?php
/**
 * @file
 * Database functions
 */

/**
 * Inserts a menu item.
 *
 * @param int $mlid
 *   Menu link ID.
 * @param int $tid
 *   Taxonomy term ID.
 * @param string $vocabulary
 *   Vocabulary machine name
 */
function _taxonomy_menu_insert_menu_item($mlid, $tid, $vocabulary) {
  $fields = array(
    'mlid' => $mlid,
    'tid' => $tid,
    'vocabulary' => $vocabulary,
  );
  db_insert('taxonomy_menu')->fields($fields)->execute();
}

/**
 * Returns the corresponding menu link id.
 *
 * @param int $tid
 *   Taxonomy term ID.
 * @param string $vocabulary
 *   Vocabulary machine name
 *
 * @return int
 *   Menu link ID for that taxonomy term.
 */
function _taxonomy_menu_get_mlid($tid, $vocabulary) {
  $where = array(
    ':tid' => $tid,
    ':vocabulary' => $vocabulary,
  );
  return db_query('SELECT mlid FROM {taxonomy_menu} WHERE tid = :tid AND vocabulary = :vocabulary', $where)->fetchField();
}

/**
 * Retrieves the term / menu relations for a vocab.
 *
 * @param $vocabulary
 *   Vocabulary machine name.
 *
 * @return array
 *   Relations to menu link ID as an array keyed by taxonomy term ID.
 *     array(tid => mlid)
 */
function _taxonomy_menu_get_menu_items($vocabulary) {
  return db_query('SELECT tid, mlid FROM {taxonomy_menu} WHERE vocabulary = :vocab', array(':vocab' => $vocabulary))->fetchAllKeyed();
}

 /**
  * Deletes all links associated with this vocab from both the taxonomy_menu
  * table and the menu_link table.
  *
  * @param string $vocabulary
  *   Vocabulary machine name.
  */
function _taxonomy_menu_delete_all($vocabulary) {
  $menu_items = _taxonomy_menu_get_menu_items($vocabulary);
  if (!empty($menu_items)) {
    // Delete all the menu items.
    foreach ($menu_items as $tid => $mlid) {
      menu_link_delete($mlid);
    }
    // Deletes all the taxonoy_menu items.
    db_delete('taxonomy_menu')
      ->condition('vocabulary', $vocabulary)
      ->execute();
  }
}

/**
 * Gets an array of the tid's related to the node
 *
 * @param object $node
 *   Node object.
 *
 * @return array
 *   Array of taxonomy term IDs.
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

        // Loop through all the terms.
        foreach ($tid_field_languages as $tid) {
          $tids[] = $tid['tid'];
        }
      }
    }
  }
  return $tids;
}

/**
 * Gets the parent tids for a taxonomy term.
 *
 * @param int $tid
 *   Taxonomy term ID.
 *
 * @return array
 *   Array of taxonomy term IDs.
 */
function _taxonomy_menu_get_parents($tid) {
  $output = array();
  $result = taxonomy_term_load_parents($tid);
  foreach ($result as $key => $item) {
    $output[] = $key;
  }
  return $output;
}

/**
  * Deletes all rows from {taxomony_menu} associated with this tid
  *
  * @param string $vocabulary
  *   Vocabulary machinen name.
  * @param int $tid
  *   Taxonomy term ID.
  */
function _taxonomy_menu_delete_item($vocabulary, $tid) {
  $and = db_and()->condition('vocabulary', $vocabulary)->condition('tid', $tid);
  db_delete('taxonomy_menu')->condition($and)->execute();
}

/**
 * Gets the count of nodes for each term (without children).
 *
 * @param int $tid
 *   Taxonomy term ID.
 *
 * @return int
 *   Count of nodes that reference the term.
 *
 * @todo Needs updating since terms are related via fields now.
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
 * Gets tid for a given mlid.
 *
 * @param int $mlid
 *   Menu link ID.
 *
 * @return int $tid
 *   Taxonomy term ID.
 *
 * @todo remove if not in use.
 */
function _taxonomy_menu_get_tid($mlid) {
  return db_query('SELECT tid FROM {taxonomy_menu} WHERE mlid = :mlid', array(':mlid' => $mlid))->fetchField();
}

/**
 * Gets the vocabulary and taxonomy term ID for a given menu link ID.
 *
 * @param int $mlid
 *   Menu link ID.
 *
 * @return array
 *   array(tid, vocabulary)
 */
function _taxonomy_menu_get_item($mlid) {
  $result = db_select('taxonomy_menu', 'tm')
    ->condition('mlid', $mlid, '=')
    ->fields('tm', array('tid', 'vocabulary'))
    ->execute();

  return $result->fetch();
}
