/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
 *
 * matchLeftParenthesis, matchRightParenthesis, _q_match and
 *     createParenthesisSelection functions based on the example published
 *     as part of Qt Quarterly Issue 31 · Q3 2009: Matching Parentheses with
 *     QSyntaxHighlighter · Example Code
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

#ifndef REGEXPTEXTEDIT_P_H
#define REGEXPTEXTEDIT_P_H

#include "regexptextedit.h"
#include "parentheseshighlighter.h"

class RegexpTextEditPrivate {
    Q_DECLARE_PUBLIC(RegexpTextEdit);

public:
    RegexpTextEditPrivate(RegexpTextEdit *q);
    ~RegexpTextEditPrivate() {}

    bool findParenthesisIndex(const char ch, int *index);
    bool isLeftParenthesis(const char ch) const;
    bool isRightParenthesis(const char ch) const;
    bool isSameTypeParenthesis(const char ch, const unsigned int index) const;

    bool matchLeftParenthesis(QTextBlock currentBlock, const int infoPos, const int pIndex);
    bool matchRightParenthesis(QTextBlock currentBlock, const int infoPos, const int pIndex);

    void createParenthesisSelection(const int pos);
    void setHighlightColor(const QColor &c);
    void setHighighlightEnabled(const bool b);
    void resetSelection();
    void setTextOptionFlagsEnabled(const bool enabled, const QTextOption::Flag flag);

    QColor hColor;
    bool highlightEnabled;
    bool showTabsAndSpaces;
    bool showLineAndParagraphSeparators;
    QTextOption textOption;

    QVector<ParenthesisPair> matchPairs;
    ParenthesesHighlighter *highlighter;

private:
    void _q_match();
    RegexpTextEdit *const q_ptr;
};

#endif // REGEXPTEXTEDIT_P_H
