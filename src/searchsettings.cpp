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
 * along with QRegExp-Editor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "searchsettings.h"

#include <QtCore/QLatin1String>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

static const char groupS[] = "search-history";
static const char searchesArray[] = "searches";
static const char patternKeyC[] = "pattern";
static const char syntaxKeyC[] = "syntax";
static const char caseSensitivityKeyC[] = "casesensitivity";
static const char minimalKeyC[] = "minimal";
static const char lengthKeyC[] = "lenght";

static const int defaultSyntax = 0; // QRegExp
static const bool defaultCaseSensitivity = true;
static const bool defaultMinimal = false;

SearchSettings::SearchSettings()
    : m_searches(),
      m_length(1)
{
}

void SearchSettings::append(const SearchData &value)
{
    if (m_searches.contains(value)) { // already there, nothing to do
    } else {
        if (m_searches.size() == m_length) {
            Q_ASSERT(!m_searches.isEmpty());
            m_searches.removeAt(0);
        }
        m_searches.append(value);
    }
}

void SearchSettings::setHistoryLength(int newLength)
{
    int searchDataSize = m_searches.size();
    if (newLength <= 0 || newLength == m_length) {
        return;
    }

    if (newLength < searchDataSize) {
        Q_ASSERT(!m_searches.isEmpty());
        int toRemoveElements = searchDataSize - newLength;
        for (int i = 0; i < toRemoveElements; ++i) {
            m_searches.removeAt(0);
        }
    }
    m_length = newLength;
}

void SearchSettings::toSettings(QSettings *s) const
{
    s->beginGroup(QLatin1String(groupS));
    s->beginWriteArray(QLatin1String(searchesArray));
    int size = m_searches.size();
    for (int i = 0; i < size; ++i) {
        s->setArrayIndex(i);
        s->setValue(QLatin1String(patternKeyC), m_searches.at(i).pattern);
        s->setValue(QLatin1String(syntaxKeyC), m_searches.at(i).syntax);
        s->setValue(QLatin1String(caseSensitivityKeyC),
                    m_searches.at(i).caseSensitivity);
        s->setValue(QLatin1String(minimalKeyC), m_searches.at(i).minimal);
    }
    s->endArray();
    s->setValue(QLatin1String(lengthKeyC), m_length);
    s->endGroup();
}

void SearchSettings::fromSettings(QSettings *s)
{
    s->beginGroup(QLatin1String(groupS));
    int size = s->beginReadArray(QLatin1String(searchesArray));
    if (size > 0) {
        for (int i = 0; i < size; ++i) {
            s->setArrayIndex(i);
            SearchData search;
            search.pattern = s->value(QLatin1String(patternKeyC),
                                      QString()).toString();
            search.syntax = s->value(QLatin1String(syntaxKeyC),
                                     defaultSyntax).toInt();
            search.caseSensitivity = s->value(
                                         QLatin1String(caseSensitivityKeyC),
                                         defaultCaseSensitivity).toBool();
            search.minimal = s->value(QLatin1String(minimalKeyC),
                                      defaultMinimal).toBool();
            m_searches.append(search);
        }
    } else {
        SearchData search;
        m_searches.append(search);
    }
    s->endArray();
    m_length = s->value(QLatin1String(lengthKeyC), 1).toInt();
    s->endGroup();
}

QDebug operator<<(QDebug dbg, const SearchSettings &p)
{
    int length = p.historyLength();
    const QList<SearchData> s = p.searchData();
    int n = s.size();

    dbg.nospace() << "(";
    for (int i = 0; i < n; ++i) {
        dbg.nospace() << s.at(i) << ", ";
    }
    dbg.nospace() << length << ")";
    return dbg.space();
}
