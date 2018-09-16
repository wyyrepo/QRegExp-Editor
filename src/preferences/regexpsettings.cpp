/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
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

#include "regexpsettings.h"
#include "patternfilter.h"

#include <QtCore/QLatin1String>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

static const char showTabsAndSpacesKeyC[] = "showTabsAndSpaces";
static const char showNewlinesKeyC[] = "showNewlines";
static const char showParenthesesMatchKeyC[] = "showParenthesesMatch";
static const char regexpFiltersKeyC[] = "filters";
static const char highlightMatchColorKeyC[] = "highlightMatchColor";

RegexpSettings::RegexpSettings(QSettings *s, const QString &name)
    : SettingsInterface(s, name)
{
}

RegexpSettings::~RegexpSettings()
{
}

void RegexpSettings::toSettings() const
{
    QPointer<QSettings> s = SettingsInterface::settings();
    QString group = SettingsInterface::group();

    s->beginGroup(group);
    s->setValue(QLatin1String(showTabsAndSpacesKeyC),
                m_options.showTabsAndSpaces);
    s->setValue(QLatin1String(showNewlinesKeyC), m_options.showNewlines);
    s->setValue(QLatin1String(showParenthesesMatchKeyC), m_options.showParenthesesMatch);
    s->setValue(QLatin1String(regexpFiltersKeyC), (uint)m_options.filters);
    s->setValue(QLatin1String(highlightMatchColorKeyC), m_options.highlightMatchColor);
    s->endGroup();
}

void RegexpSettings::fromSettings()
{
    QPointer<QSettings> s = SettingsInterface::settings();
    QString group = SettingsInterface::group();

    s->beginGroup(group);
    m_options.showTabsAndSpaces = s->value(QLatin1String(
            showTabsAndSpacesKeyC)).toBool();
    m_options.showNewlines = s->value(QLatin1String(showNewlinesKeyC)).toBool();
    m_options.showParenthesesMatch = s->value(QLatin1String(showParenthesesMatchKeyC)).toBool();
    m_options.filters = (PatternFilter::Filters) s->value(QLatin1String(regexpFiltersKeyC)).toUInt();
    m_options.highlightMatchColor = s->value(QLatin1String(highlightMatchColorKeyC), Qt::yellow).value<QColor>();
    s->endGroup();
}

RegexpOptions RegexpSettings::options() const
{
    return m_options;
}

void RegexpSettings::setOptions(const RegexpOptions &ao)
{
    if (m_options != ao) {
        m_options = ao;
        toSettings();
        emit settingsChanged(SettingsInterface::group());
    }
}

void RegexpSettings::updateSettings()
{
}

QString RegexpSettings::group() const
{
    return SettingsInterface::group();
}
