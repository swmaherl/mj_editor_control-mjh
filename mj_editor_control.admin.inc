<?php
/**
 * @file
 * Administration for MJ Editor Control.
 */

/** Settings form. */
function mj_editor_control_settings_form($form, &$form_state) {
  $settings = config_get('mj_editor_control.settings');
  $form['intro'] = array('#markup' => '<p>Dieses Modul steuert die Anzeige der Editoren. Textformate, gespeicherte Inhalte und HTML-Filter bleiben unverändert. CKEditor 5 muss aktiviert und dem gewünschten HTML-Textformat zugeordnet sein.</p>');
  if (!module_exists('bueditor')) {
    $form['warning_bue'] = array('#markup' => '<div class="messages warning">BUEditor ist nicht aktiviert. Benutzer mit BUEditor-Vorrang erhalten deshalb ein Textfeld ohne Editor.</div>');
  }
  if (!module_exists('ckeditor5') && !module_exists('ckeditor')) {
    $form['warning_cke'] = array('#markup' => '<div class="messages warning">CKEditor ist nicht aktiviert. Bitte zuerst das Core-Modul aktivieren und ein geeignetes Textformat konfigurieren.</div>');
  }
  $form['enabled'] = array('#type' => 'checkbox', '#title' => 'Editor-Steuerung aktivieren', '#default_value' => $settings['enabled']);
  $roles = user_roles();
  $form['priority'] = array('#type' => 'fieldset', '#title' => 'BUEditor hat Vorrang');
  $form['priority']['bueditor_roles'] = array(
    '#type' => 'checkboxes', '#title' => 'Rollen mit BUEditor', '#options' => $roles,
    '#default_value' => $settings['bueditor_roles'],
    '#description' => 'Für diese Rollen wird CKEditor in allen formatierten Textfeldern verhindert. In BUEditor müssen die Rollen-/Benutzerzuordnung und die gewünschten Seiten weiterhin freigegeben sein. Dieser Vorrang gilt auch bei weiteren Rollen des Benutzers.',
  );
  $form['priority']['user_one'] = array('#type' => 'checkbox', '#title' => 'Benutzer 1 immer mit BUEditor-Vorrang behandeln', '#default_value' => $settings['user_one']);
  $form['selection'] = array('#type' => 'fieldset', '#title' => 'CKEditor für andere Benutzer');
  $form['selection']['ckeditor_roles'] = array(
    '#type' => 'checkboxes', '#title' => 'Rollen mit CKEditor', '#options' => $roles,
    '#default_value' => $settings['ckeditor_roles'],
    '#description' => 'BUEditor wird für Benutzer ohne BUEditor-Vorrang unterdrückt. Ohne passende CKEditor-Regel bleibt ein normales Texteingabefeld.',
  );
  $field_options = array();
  foreach (field_info_fields() as $name => $field) {
    if (in_array($field['type'], array('text_long', 'text_with_summary'))) {
      $field_options[$name] = $name;
    }
  }
  ksort($field_options);
  $selected = array_filter(array_map('trim', preg_split('/\r\n|\r|\n/', $settings['fields'])), 'strlen');
  $form['selection']['all_fields'] = array(
    '#type' => 'checkbox', '#title' => 'Alle formatierten mehrzeiligen Textfelder zulassen',
    '#default_value' => trim($settings['fields']) === '',
    '#description' => 'Ohne diese Option muss mindestens ein Feld oder Feldmuster angegeben sein.',
  );
  $form['selection']['selected_fields'] = array(
    '#type' => 'checkboxes', '#title' => 'CKEditor-Felder', '#options' => $field_options,
    '#default_value' => array_values(array_intersect($selected, array_keys($field_options))),
    '#description' => 'Die Auswahl gilt für Haupttext und ein gegebenenfalls vorhandenes Zusammenfassungsfeld. Reine Klartextfelder werden nicht in HTML-Felder umgewandelt.',
  );
  $form['selection']['custom_fields'] = array(
    '#type' => 'textarea', '#title' => 'Weitere Feldnamen oder Muster', '#rows' => 3,
    '#default_value' => implode("\n", array_diff($selected, array_keys($field_options))),
    '#description' => 'Optional eine Angabe je Zeile, z. B. field_beschreibung_* oder bei einem eigenen Formular der Elementname. * ist ein Platzhalter.',
  );
  $bundle_options = array();
  if (module_exists('node')) {
    foreach (node_type_get_types() as $type => $info) {
      $bundle_options[$type] = check_plain($info->name) . ' (' . check_plain($type) . ')';
    }
  }
  $form['selection']['bundles'] = array(
    '#type' => 'checkboxes', '#title' => 'Auf Inhaltstypen begrenzen', '#options' => $bundle_options,
    '#default_value' => $settings['bundles'],
    '#description' => 'Keine Auswahl: alle Inhaltstypen und eigene Formulare. Bei einer Auswahl werden ausschließlich passende Node-Felder zugelassen.',
  );
  $form['selection']['path_mode'] = array(
    '#type' => 'radios', '#title' => 'Seitenregeln', '#default_value' => $settings['path_mode'],
    '#options' => array('include' => 'Nur auf den angegebenen Seiten', 'exclude' => 'Außer auf den angegebenen Seiten'),
  );
  $form['selection']['paths'] = array(
    '#type' => 'textarea', '#title' => 'Formularseiten', '#rows' => 4,
    '#default_value' => $settings['paths'],
    '#description' => 'Leer: keine Seitenbegrenzung. Eine Angabe je Zeile ohne führenden Schrägstrich, z. B. node/add/* oder node/*/edit. Für Bearbeitungsformulare deren tatsächlichen Pfad verwenden; node/add/event gilt nicht automatisch für node/123/edit. Systempfad und Alias werden berücksichtigt. Feld-, Inhaltstyp- und Seitenbedingungen müssen gemeinsam erfüllt sein.',
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/** Validate settings without allowing an accidental unrestricted field list. */
function mj_editor_control_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (empty($values['all_fields']) && !array_filter($values['selected_fields']) && trim($values['custom_fields']) === '') {
    form_set_error('selected_fields', 'Bitte mindestens ein Feld auswählen oder ausdrücklich alle formatierten Textfelder zulassen.');
  }
  foreach (preg_split('/\r\n|\r|\n/', trim($values['paths'])) as $path) {
    if (substr(trim($path), 0, 1) === '/' || strpos($path, '://') !== FALSE) {
      form_set_error('paths', 'Bitte interne Pfade ohne Domain und ohne führenden Schrägstrich angeben.');
      break;
    }
  }
}

/** Save only declared configuration values. */
function mj_editor_control_settings_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $config = config('mj_editor_control.settings');
  foreach (array('enabled', 'user_one') as $key) {
    $config->set($key, (bool) $values[$key]);
  }
  foreach (array('bueditor_roles', 'ckeditor_roles', 'bundles') as $key) {
    $config->set($key, array_values(array_filter($values[$key])));
  }
  $fields = array_values(array_filter($values['selected_fields']));
  $fields = array_merge($fields, array_filter(array_map('trim', preg_split('/\r\n|\r|\n/', $values['custom_fields'])), 'strlen'));
  $config->set('fields', !empty($values['all_fields']) ? '' : implode("\n", array_unique($fields)));
  $config->set('paths', trim($values['paths']));
  $config->set('path_mode', $values['path_mode']);
  $config->save();
  // Element definitions depend on these settings and are cached by core.
  backdrop_static_reset('element_info');
  cache_clear_all(NULL, 'cache');
  backdrop_set_message(t('The configuration options have been saved.'));
}
