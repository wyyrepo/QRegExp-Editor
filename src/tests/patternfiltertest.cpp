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

#include "patternfiltertest.h"

#include <QtTest/QtTest>

#include "../patternfilter.h"

PatternFilterTest::PatternFilterTest()
    : QObject()
{
    pf = new PatternFilter();
}

void PatternFilterTest::init()
{
    pf->setFilters(0); // no filters
}

void PatternFilterTest::cleanupTestCase()
{
    delete pf;
    pf = 0;
}

void PatternFilterTest::testNoFilters_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("expected");

    QTest::newRow("trimmed") << QString("(\\d +)") << QString("(\\d +)");
    QTest::newRow("whitespace at ends") << QString("  (\\d+)  ")
                                        << QString("  (\\d+)  ");
}

void PatternFilterTest::testNoFilters()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    pf->setFilters(0);
    QCOMPARE(pf->filtered(pattern), expected);
}

void PatternFilterTest::testNewlines_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("expected");

    QTest::newRow("trimmed") << QString("(\\d +)") << QString("(\\d +)");
    QTest::newRow("whitespace at ends") << QString("  (\\d+)  ")
                                        << QString("  (\\d+)  ");
    QTest::newRow("newline") << QString("(\\d\n+)")
                             << QString("(\\d+)");
}

void PatternFilterTest::testNewlines()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    pf->setFilters(PatternFilter::FilterNewlines);
    QCOMPARE(pf->filtered(pattern), expected);
}


void PatternFilterTest::testTrailingWhitespaces_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("expected");

    QTest::newRow("trimmed") << QString("(\\d +)") << QString("(\\d +)");
    QTest::newRow("whitespace at ends") << QString("  (\\d+)  ")
                                        << QString("  (\\d+)");
    QTest::newRow("newline and trailing whitespaces") << QString("(\\d \n+)  ")
            << QString("(\\d \n+)");
}

void PatternFilterTest::testTrailingWhitespaces()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    pf->setFilters(PatternFilter::FilterTrailingWhitespaces);
    QCOMPARE(pf->filtered(pattern), expected);
}

void PatternFilterTest::testNewlinesAndTrailingWhitespaces_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QString>("expected");

    QTest::newRow("trimmed") << QString("(\\d +)") << QString("(\\d +)");
    QTest::newRow("whitespace at ends") << QString("  (\\d+)  ")
                                        << QString("  (\\d+)");
    QTest::newRow("newline and trailing whitespaces") << QString("(\\d+)  ")
            << QString("(\\d+)");
}

void PatternFilterTest::testNewlinesAndTrailingWhitespaces()
{
    QFETCH(QString, pattern);
    QFETCH(QString, expected);

    const PatternFilter::Filters f(PatternFilter::FilterNewlines |
                                   PatternFilter::FilterTrailingWhitespaces);
    pf->setFilters(f);
    QCOMPARE(pf->filtered(pattern), expected);
}

QTEST_MAIN(PatternFilterTest)
