# MJ Editor Control — 1.x-1.0.0-MJH

Backdrop-CMS-Modul zur Auswahl der CKEditor-Felder mit BUEditor-Vorrang.
Keine Änderungen am Core, an BUEditor, an gespeicherten Inhalten oder an den
HTML-Filtern. Die .info-Datei enthält bewusst keinen project-Eintrag.

## Installation und Einrichtung

1. Ordner `mj_editor_control` nach `modules` kopieren und Modul aktivieren.
2. `admin/config/content/mj-editor-control` aufrufen:
   Konfiguration → Inhaltserstellung → MJ Editor Control.
3. Administratorrolle bei „Rollen mit BUEditor“ auswählen. Benutzer 1 ist
   zusätzlich standardmäßig berücksichtigt.
4. Gäste und authentifizierte Benutzer bei CKEditor auswählen.
5. Beschreibungsfelder auswählen; optional Inhaltstypen und Seiten begrenzen.
6. „Editor-Steuerung aktivieren“ einschalten und speichern.
7. In BUEditor weiterhin Administrator-/Benutzer-1-Zuordnung, Seitenfreigaben
   und ausgeschlossene Textfelder prüfen. Das Modul erzeugt keine BUEditor-
   Profile und übersteuert deren Ausschlüsse nicht.
8. Für die betreffenden Felder gefilterten Text und ein erlaubtes HTML-Format
   mit zugeordnetem CKEditor 5 verwenden. Gäste benötigen Zugriff auf dieses
   Format. Die Toolbar wird weiterhin beim Textformat konfiguriert.

Direkt nach Installation ist die Steuerung noch ausgeschaltet. Die Vorschläge
`body` und `field_beschreibung` müssen an die eigene Installation angepasst
werden. Bestehendes „HTML mit CKEditor“ kann unverändert weiterverwendet werden.

## Verhalten

- BUEditor-Rollen haben Vorrang vor CKEditor-Rollen, auch bei mehreren Rollen.
- Für diese Benutzer wird CKEditor 4/5 in allen formatierten Textfeldern
  unterdrückt. BUEditor läuft nach seiner eigenen Konfiguration weiter.
- Für andere Benutzer wird die reguläre BUEditor-Textarea-Einbindung verhindert.
- CKEditor 4/5 wird dort nur bei passender Rolle, Feld-, Inhaltstyp- und
  Seitenauswahl zugelassen. Mehrere Angaben innerhalb einer Gruppe gelten als
  ODER; ausgefüllte Gruppen untereinander als UND.
- Außerhalb der Auswahl bleibt ein normales Textfeld; das Textformat wirkt
  weiterhin bei der Ausgabe. Diese Steuerung ist keine HTML-Sicherheitsprüfung.
- Ein Textformat ohne CKEditor-Zuordnung erhält keinen CKEditor; Klartextfelder
  erhalten ebenfalls keinen Editor. Es werden keine Formate automatisch
  gewechselt, keine Zugriffsrechte erweitert und keine Texte konvertiert.
- Haupttext und Zusammenfassung werden gemeinsam gesteuert.
- Formatwechsel, AJAX und das Zurückschreiben beim Absenden verwenden die
  normalen Core-Editorfunktionen. Andere Editorprodukte werden nicht gesteuert.
- Seitenregeln beziehen sich auf die Formular-URL, nicht auf die Anzeige-URL.
  Beispiel: `node/add/*` und `node/*/edit`. Bei AJAX-Formularen wird der vom
  Backdrop-Formularsystem beibehaltene ursprüngliche Formular-Action verwendet.

## Prüfen auf der eigenen Website

Als Administrator und anschließend in einer privaten Sitzung als Gast bzw.
Testbenutzer ein Formular öffnen. Prüfen: nur BUEditor für Administrator,
CKEditor nur in ausgewählten Feldern für Kunden, Formatwechsel, neue und
bestehende Inhalte, Vorschau, Speichern und per AJAX ergänzte Feldwerte.
Bei aktiviertem Seitenfilter auch die tatsächlichen AJAX-Abläufe prüfen.

Wenn ein vorhandener Feldinhalt ein nicht zugängliches Format verwendet,
gelten weiterhin die Core-Sperren. Das Modul hebt diese bewusst nicht auf.

## Deaktivieren

Die Checkbox ausschalten und speichern oder das Modul deaktivieren.
Anschließend gelten wieder die bisherigen Editor-Konfigurationen.

## Validierung des Pakets

30 automatisierte PHP-Prüfungen der Auswahlregeln und Rollenpriorität sowie
JavaScript-Prüfungen der Anbindung an den originalen Filter-Code aus Backdrop
1.30.0 und dem aktuellen 1.x-Zweig (Anfügen, erneutes Anfügen nach Formatwechsel,
Absenden, Serialisieren und wiederholtes Laden des Skripts).
PHP-Syntaxprüfung mit PHP 8.3; Quelltext kompatibel mit PHP 7.4.
Eine vollständige
Browserprüfung mit einer laufenden Backdrop-Installation ist nicht enthalten.

Lizenz: GPL-2.0-or-later (wie Backdrop CMS).
