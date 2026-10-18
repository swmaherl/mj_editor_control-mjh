/**
 * Gate core editor attachment before behaviors run, including AJAX/format changes.
 * Never detach or hide an editor after it has already transformed the HTML.
 */
(function (Backdrop) {
  'use strict';
  if (!Backdrop.filterEditorAttach || Backdrop.mjEditorControlInstalled) {
    return;
  }
  Backdrop.mjEditorControlInstalled = true;
  var attach = Backdrop.filterEditorAttach;
  var detach = Backdrop.filterEditorDetach;

  function allowed(field, format) {
    var mode = field.getAttribute('data-mj-editor-mode');
    if (!mode) {
      return true;
    }
    // BUEditor attaches through its own behavior, never through the core API.
    if (format.editor === 'bueditor') {
      return false;
    }
    if (format.editor === 'ckeditor5' || format.editor === 'ckeditor') {
      return mode === 'ckeditor';
    }
    return true;
  }

  Backdrop.filterEditorAttach = function (field, format) {
    if (allowed(field, format)) {
      return attach.apply(this, arguments);
    }
  };
  Backdrop.filterEditorDetach = function (field, format, trigger) {
    if (allowed(field, format)) {
      return detach.apply(this, arguments);
    }
  };
})(Backdrop);
