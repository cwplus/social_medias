<?php
/**
 * Created by PhpStorm.
 * User: khalid
 * Date: 27/07/16
 * Time: 6:42
 */

function social_medias_admin_form()
{
    drupal_add_css(drupal_get_path('module', 'social_medias') . '/css/social-medias-admin.css');
    $social_medias = unserialize(SOCIAL_MEDIAS);
    $sm_has_counter = null;

    $form = array();
    $t = get_t();
    $settings = social_medias_settings();

    $sm_enabled  = variable_get('sm_enabled',array_keys($social_medias));
    if($sm_enabled){
        foreach($sm_enabled as $key =>$enabled){
            $enabled = intval($enabled);
            if($enabled) $sm_has_counter[$enabled] = $social_medias[$enabled];
        }
    }
    $font_awesome = intval(variable_get('sm_font_awesome', 1));
    $hide_counter = intval(variable_get('sm_hide_counter', 0));

    $form['sm_settings'] = array(
        '#type'  => 'fieldset',
        '#title' => $t('Social medias enabled'),
        '#collapsible'  => TRUE,
        '#collapsed'    => FALSE,
    );

    $form['sm_settings']['sm_font_awesome'] = array(
        '#type' => 'radios',
        '#title' => t('Add font awesome css'),
        '#options' => array(0 => 'No', 1 => 'Yes'),
        '#default_value' => $font_awesome,
    );

    $form['sm_settings']['sm_enabled'] = array(
        '#type' => 'checkboxes',
        '#title' => $t('Social medias enabled'),
        '#options' => $social_medias,
        '#default_value' => $sm_enabled,
        '#required' => TRUE,
    );

    if(!is_null($sm_has_counter)){
        $form['sm_settings']['sm_has_counter'] = array(
            '#type' => 'checkboxes',
            '#title' => $t('The social medias you want them to displayed their counters'),
            '#options' => $sm_has_counter,
            '#default_value' => variable_get('sm_has_counter',array_keys($sm_has_counter)),
            '#required' => false,
        );
    }

    if(isset($sm_enabled[7]) && $sm_enabled[7]){
        $form['sm_settings']['xing_follow'] = array(
            '#type' => 'textfield',
            '#title' => $t('Xing page to follow'),
            '#required' => false,
        );
    }

    if(isset($sm_enabled[1]) && $sm_enabled[1]){
        $form['sm_settings']['via'] = array(
            '#type' => 'textfield',
            '#title' => $t('Via Twitter username'),
            '#description' => $t('Example: @CWPlus, You must write the username without "@".'),
            '#default_value' => 'CWPlus',
            '#required' => false,
        );
    }

    $form['sm_settings']['sm_hide_counter'] = array(
        '#type' => 'radios',
        '#title' => t('Hide the counter for each social media'),
        '#options' => array(0 => 'No', 1 => 'Yes'),
        '#default_value' => $hide_counter,
    );

    $form['sm_placements'] = array(
        '#type' => 'fieldset',
        '#title' => $t('Placements'),
        '#collapsible' => TRUE,
        '#collapsed' => true
    );

// Add checkboxes for each view mode of each bundle.
    $entity_info = entity_get_info('node');
    $modes = array();
    foreach ($entity_info['view modes'] as $mode => $mode_info) {
        $modes[$mode] = $mode_info['label'];
    }
    // Get a list of content types and view modes.
    $view_modes_selected = $settings['view_modes'];
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
        $form['sm_placements']['content']['sm_' . $bundle . '_options'] = array(
            '#title' => $t('%label View Modes', array('%label' => $bundle_info['label'])),
            '#description' => $t('Select which view modes the Social Share Counter buttons should appear on for %label nodes.', array('%label' => $bundle_info['label'])),
            '#type' => 'checkboxes',
            '#options' => $modes,
            '#default_value' => $view_modes_selected[$bundle],
            '#required' => false,
        );
    }
    $options = drupal_map_assoc(array(-100, -50, -25, -10, 0, 10, 25, 50, 100));
    $form['sm_placements']['sm_display_weight'] = array(
        '#type' => 'select',
        '#title' => t('Content weight'),
        '#required' => FALSE,
        '#options' => $options,
        '#default_value' => variable_get('sm_display_weight', -10),
        '#description' => $t('Optional weight value determines the location on the page where it will appeared in the <strong>content</strong> section.'),
        '#states' => array(
            // Show only if placement in "content" selected.
            'visible' => array(
                ':input[name="sm_display_in_nodecont"]' => array('checked' => TRUE),
            ),
        ),
    );

    return system_settings_form($form);
}

function social_medias_settings()
{
    //$t = get_t();
    $default_nodetypes = array('full');
    $view_modes = array();
    foreach (array_keys(node_type_get_types()) as $type) {
        $view_modes[$type] = variable_get('sm_' . $type . '_options', $default_nodetypes);
    }
    $settings['view_modes'] = $view_modes;
    return $settings;
}

function social_medias_admin_form_validate($form, &$form_state)
{
    $entity_info = entity_get_info('node');
    $selected_values = array();
    foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
        $values = $form_state['values']['sm_'.$bundle.'_options'];
        //$selected_values = array();
        foreach($values as $key=>$value){
            if($value) $selected_values[$key] = $value;
        }
    }
    if(empty($selected_values)){
        form_set_error('sm_options', 'You must check only one for placements .');
    }
}