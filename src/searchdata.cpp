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

#include "searchdata.h"

SearchData::SearchData()
    : pattern(QString()),
      syntax(0),
      caseSensitivity(true),
      minimal(false)
{
}

bool SearchData::equals(const SearchData &s) const
{
    return pattern == s.pattern
           && syntax == s.syntax
           && caseSensitivity == s.caseSensitivity
           && minimal == s.minimal;
}

QDebug operator<<(QDebug dbg, const SearchData &p)
{
    dbg.nospace() << "(" << p.pattern << ", " << p.syntax << ", "
                  << p.caseSensitivity << ", " << p.minimal << ")";
    return dbg.space();
}
