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

#include "patternfilter.h"

#include <QtCore/QStringList>
#include <QtCore/QDebug>

class PatternFilter::Private {

public:
    QString rightTrim(QString &p);
};

QString PatternFilter::Private::rightTrim(QString &p)
{
    if (p.size() == 0) {
        return p;
    }

    const QChar *s = (const QChar *)p.data();
    if (!s[p.size() - 1].isSpace()) { // is last string char an whitespace ?
        return p;
    }

    int i = p.size() - 1;
    while (i >= 0 && s[i].isSpace()) {
        --i;
    }
    p.truncate(++i);
    return QString(p);
}

PatternFilter::PatternFilter()
    : f(NoFilters)
{
}


QString PatternFilter::filtered(QString &pattern)
{
    QString s;
    const PatternFilter::Filters all(FilterNewlines | FilterTrailingWhitespaces);

    if (f == NoFilters) { // no filters set
        return QString(pattern);
    }

    if (f == all) {
        QStringList lines = pattern.split(QChar('\n'));
        QStringList::Iterator iter = lines.begin();
        while (iter != lines.end()) {
            s.append(d->rightTrim(*iter));
            ++iter;
        }
        return s;
    }

    if (f.testFlag(FilterNewlines)) {
        s = pattern.remove(QChar('\n'));
    }

    if (f.testFlag(FilterTrailingWhitespaces)) {
        s = d->rightTrim(pattern);
    }

    return s;
}
