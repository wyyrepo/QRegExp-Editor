/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * Based on the example published as part of Qt Quarterly Issue 31: Q3 2009
 *     Matching Parentheses with QSyntaxHighlighter · Example Code
 *     http://doc.qt.nokia.com/qq/qq31-parenthesis.zip
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * Contact: Nokia Corporation (qt-info@nokia.com)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegexp-Editor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PARENTHESESHIGHLIGHTER_H
#define PARENTHESESHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>
#include <QtCore/QVector>

class QString;
class QTextDocument;

struct ParenthesisInfo {
    char ch;
    int pos;
};

struct ParenthesisPair {
    char left;
    char right;
};

class TextBlockData : public QTextBlockUserData {

public:
    TextBlockData();

    QVector<ParenthesisInfo *> parentheses();
    void insert(ParenthesisInfo *info);

private:
    QVector<ParenthesisInfo *> m_parentheses;
};


class ParenthesesHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    ParenthesesHighlighter(QTextDocument *document, const QVector<ParenthesisPair> &matchablePairs);

protected:
    void highlightBlock(const QString &text);

private:
    QVector<ParenthesisPair> m_ppairs;
};

#endif // PARENTHESESHIGHLIGHTER_H
