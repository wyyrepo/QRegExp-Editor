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

#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include <QtCore/QString>
#include <QtCore/QDebug>

struct SearchData {
    QString pattern;
    int syntax;
    bool caseSensitivity;
    bool minimal;

    SearchData();
    bool equals(const SearchData &s) const;
};

inline bool operator==(const SearchData &p1, const SearchData &p2)
{ return p1.equals(p2); }

QDebug operator<<(QDebug dbg, const SearchData &p);

#endif // SEARCHDATA_H
