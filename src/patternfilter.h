/*
 * Copyright (C) 2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * This file is part of QRegexp-Editor.
 *
 * QRegexp-Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QRegExp-Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegexp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PATTERNFILTER_H
#define PATTERNFILTER_H

#include <QtCore/QString>


class PatternFilter {

public:
    PatternFilter();

    enum Filter {
        NoFilters = 0x0,
        FilterNewlines = 0x1,
        FilterTrailingWhitespaces = 0x2
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    inline void setFilters(Filters filters);
    inline Filters filters() const { return Filters(f); };

    QString filtered(QString &pattern);

private:
    class Private;
    Private *d;
    Filters f;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PatternFilter::Filters)

inline void PatternFilter::setFilters(Filters filters) { f = filters; };

#endif // PATTERNFILTER_H
