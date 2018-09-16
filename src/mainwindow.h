/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
 * Copyright (C) 2010-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegExp-Editor.
 *
 * QRegExp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "searchsettings.h"
#include "patternfilter.h"

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QSettings>

#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class PreferencesDialog;
class RegexpSettings;
class AboutDialog;
class EscapedPatternDialog;
class RegExpModel;
class QAction;
class QLabel;
class QToolBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::MainWindow *ui;
    RegExpModel *m_model;
    AboutDialog *m_aboutDialog;
    EscapedPatternDialog *m_escapedPatternDialog;
    QLabel *m_statusLabel;
    QRegExp m_rx;
    QStringList m_recentFiles;
    QList<QAction *> m_recentFileActions;
    const int m_maxRecentFiles;
    QSettings m_settings;
    SearchSettings m_searchSettings;
    QList<QToolBar *> toolbars;
    RegexpSettings *m_regexpSettings;
    PatternFilter patternFilter;
    PreferencesDialog *m_preferencesDialog;

    bool loadFile(const QString &filename);
    void updateRecentFileActions();
    void writeSettings();
    void readSettings();
    void populateComboBoxes();
    void populateToolbarMenu();
    void setIcons();
    void makeSignalConnections();
    void createStatusBar();
    void createPreferencesDialog();
    void createRegExpModel();
    void createRecentFileActions();
    bool isSearchPossible();
    SearchData searchSettings() const;
    void setSearchSettings(SearchSettings *s) const;

private slots:
    void open();
    void openRecentFile();
    void clearAllRecentFiles();
    void match();
    void about();
    void escapedPattern();
    void updateUiStatus();
    void updateRegExpOptions(); // Updates everyting except pattern
    void updateRegExpPattern();
    void clearInputEdit();
    void clearRegExpEdit();
    void updateStatus(const QString &);
    void toggleWarningWidget(bool);
    void showPreferencesDialog();
    void updateRegexpSettingsUi();
    void showTabsAndSpaces(bool);
    void showNewlines(bool);
    void showParenthesesMatch(bool);
    void filterNewlines(bool);
    void filterTrailingWhitespaces(bool);
    void updateRegexpDialogSettingsUi();
};

#endif // MAINWINDOW_H
