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

#ifndef SEARCHSETTINGS_H
#define SEARCHSETTINGS_H

#include "searchdata.h"

#include <QtCore/QList>

class QSettings;

class SearchSettings {
public:
    SearchSettings();
    void setHistoryLength(int newLength);
    inline int historyLength() const { return m_length; }
    void append(const SearchData &value);
    inline const QList<SearchData> searchData() const { return m_searches; }
    void toSettings(QSettings *s) const;
    void fromSettings(QSettings *s);

private:
    QList<SearchData> m_searches;
    int m_length;
};

QDebug operator<<(QDebug dbg, const SearchSettings &p);

#endif // SEARCHSETTINGS_H
