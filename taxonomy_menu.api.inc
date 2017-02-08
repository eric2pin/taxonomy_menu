<?php
/**
 * @file
 * API functions.
 */

/**
 * Add additional configuration options for taxonomy menu.
 *
 * Settings will be saved in the config file with the key of the form element,
 * so don't include the module's name.
 *
 * @return array
 *   Form element definitions for the vocab settings form. If form element type
 *   is not provided, it is assumed to be a checkbox.
 */
function hook_taxonomy_menu_options() {
  $options['expanded'] = array(
    '#type' => 'checkbox', // This will be assumed if omitted.
    '#title' => t('Automatically show all menu items as expanded.'),
    '#default_value' => TRUE,
  );
  $options['max_depth'] = array(
    '#type' => 'select',
    '#title' => t('Max depth'),
    '#description' => t('Limit how many levels of the taxonomy tree to process. Useful if you have a very large tree of taxonomy terms, and only want to provide a menu for the first several levels.'),
    '#options' => array(0 => t('All'), 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7 => 7, 8 => 8, 9 => 9),
    '#default_value' => 0,
  );

  return $options
}

/**
 * Update the taxonomy menu entry when being inserted.
 *
 * @param array $item
 *   Taxonomy menu item array with the following key/value pairs:
 *     'tid': The term id.
 *     'name': New menu name.
 *     'description': New menu description, used for the title attribute.
 *     'weight': New menu weight.
 *     'vocabulary': Vocabulary machine name.
 *     'ptid': The new parent tid.
 *     'remove': If this is set to TRUE then the $item is not added as a menu.
 */
function hook_taxonomy_menu_insert(&$item) {

}

/**
 * Update the taxonomy menu entry when being updated.
 *
 * @param array $item
 *   Taxonomy menu item array with the following key/value pairs:
 *     'tid': The term id.
 *     'name': New menu name.
 *     'description': New menu description, used for the title attribute.
 *     'weight': New menu weight.
 *     'vocabulary': Vocabulary machine name.
 *     'ptid': The new parent tid.
 *     'remove': If this is set to TRUE then the $item is not added as a menu.
 */
function hook_taxonomy_menu_update(&$item) {

}

/**
 * Update the taxonomy menu entry when being deleted.
 *
 * @param array $item
 *   Taxonomy menu item array with the following key/value pairs:
 *     'tid': The term id.
 *     'name': New menu name.
 *     'description': New menu description, used for the title attribute.
 *     'weight': New menu weight.
 *     'vocabulary': Vocabulary machine name.
 *     'ptid': The new parent tid.
 *     'remove': If this is set to TRUE then the $item is not added as a menu.
 */
function hook_taxonomy_menu_delete(&$item) {

}
