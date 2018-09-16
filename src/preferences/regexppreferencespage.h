/*
 * Copyright (C) 2012 Luís Pereira <luis.artur.pereira@gmail.com>
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
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REGEXPPREFERENCESPAGE_H
#define REGEXPPREFERENCESPAGE_H

#include "preferencespageinterface.h"
#include "regexpsettings.h"

#include <QtCore/QObject>
#include <QtCore/QPointer>

#include <QtGui/QWidget>
#include <QtGui/QPixmap>

namespace Ui {
    class RegexpPreferencesWidget;
}

class RegexpDialogSettings;
class SettingsInterface;
class QAction;

class RegexpPreferencesWidget : public QWidget {
    Q_OBJECT

public:
    RegexpPreferencesWidget(QWidget *parent = 0);
    ~RegexpPreferencesWidget();

    RegexpOptions regexpOptions() const;
    void setRegexpOptions(const RegexpOptions &opt);

private Q_SLOTS:
    void highlightMatchColor();

private:
    void highlightMatchColorChanged(const QColor &color);

    Ui::RegexpPreferencesWidget *m_ui;
    QAction *actionHighlightMatchColor;
    RegexpOptions rc;
};


class RegexpPreferencesPage : public QObject, public PreferencesPageInterface {
    Q_OBJECT

public Q_SLOTS:
    void updateSettings();

public:
    RegexpPreferencesPage(QWidget *parent, SettingsInterface *s);
    QWidget *widget() const;
    virtual void apply();
    virtual void finish();
    virtual void updateWidget();

private:
    void setSettings();

    RegexpOptions m_initialOptions;
    QPointer<RegexpPreferencesWidget> m_widget;
    QPointer<RegexpSettings> settings;
};

#endif // REGEXPPREFERENCESPAGE_H
