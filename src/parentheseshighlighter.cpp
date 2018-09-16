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

#include "parentheseshighlighter.h"

#include <QtCore/QString>

TextBlockData::TextBlockData()
{
}

QVector<ParenthesisInfo *> TextBlockData::parentheses()
{
    return m_parentheses;
}

void TextBlockData::insert(ParenthesisInfo *info)
{
    int i = 0;
    while (i < m_parentheses.size() && info->pos > m_parentheses.at(i)->pos) {
        ++i;
    }
    m_parentheses.insert(i, info);
}

ParenthesesHighlighter::ParenthesesHighlighter(QTextDocument *document, const QVector<ParenthesisPair> &matchablePairs)
    : QSyntaxHighlighter(document)
{
    m_ppairs = matchablePairs;
}

void ParenthesesHighlighter::highlightBlock(const QString &text)
{
    int pos;

    TextBlockData *data = new TextBlockData;
    for (int i = 0; i < m_ppairs.size(); ++i) {
        pos = text.indexOf(m_ppairs.at(i).left);
        while (pos != -1) {
            ParenthesisInfo *mark = new ParenthesisInfo;
            mark->ch =  m_ppairs.at(i).left;
            mark->pos = pos;
            data->insert(mark);

            pos = text.indexOf(m_ppairs.at(i).left, pos + 1);
        }

        pos = text.indexOf(m_ppairs.at(i).right);
        while (pos != -1) {
            ParenthesisInfo *mark = new ParenthesisInfo;
            mark->ch =  m_ppairs.at(i).right;
            mark->pos = pos;
            data->insert(mark);

            pos = text.indexOf(m_ppairs.at(i).right, pos + 1);
        }
    }
    setCurrentBlockUserData(data);
}
