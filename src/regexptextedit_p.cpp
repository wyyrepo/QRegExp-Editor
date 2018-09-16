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

#include "regexptextedit_p.h"
#include "regexptextedit.h"

#include <QtCore/QDebug>

RegexpTextEditPrivate::RegexpTextEditPrivate(RegexpTextEdit *q)
    : q_ptr(q)
{
    hColor = Qt::yellow;
    highlightEnabled = false;
    showTabsAndSpaces = false;
    showLineAndParagraphSeparators = false;
    textOption = q->document()->defaultTextOption();

    ParenthesisPair p;
    p.left = '(';
    p.right = ')';
    matchPairs.append(p);

    p.left = '{';
    p.right = '}';
    matchPairs.append(p);

    p.left = '[';
    p.right = ']';
    matchPairs.append(p);

    highlighter = new ParenthesesHighlighter(q->document(), matchPairs);
}

bool RegexpTextEditPrivate::isLeftParenthesis(const char ch) const
{
    const int size = matchPairs.size();
    for (int i = 0; i < size; ++i) {
        if (matchPairs.at(i).left == ch) {
            return true;
        }
    }
    return false;
}

bool RegexpTextEditPrivate::isRightParenthesis(const char ch) const
{
    const int size = matchPairs.size();
    for (int i = 0; i < size; ++i) {
        if (matchPairs.at(i).right == ch) {
            return true;
        }
    }
    return false;
}

bool RegexpTextEditPrivate::isSameTypeParenthesis(const char ch, const unsigned int index) const
{
    if (ch == matchPairs.at(index).left || ch == matchPairs.at(index).right) {
        return true;
    } else {
        return false;
    }
}

bool RegexpTextEditPrivate::findParenthesisIndex(const char ch, int *index)
{
    for (int i = 0; i < matchPairs.size(); ++i) {
        if (ch == matchPairs.at(i).left || ch == matchPairs.at(i).right) {
            *index = i;
            return true;
        }
    }
    return false;
}

bool RegexpTextEditPrivate::matchLeftParenthesis(QTextBlock currentBlock, const int infoPos, const int pIndex)
{
    Q_Q(RegexpTextEdit);

    int i = infoPos;
    bool firstime = true;

    for (; ;) {
        if (!currentBlock.isValid()) {
            return false;
        }

        TextBlockData *data =
            static_cast<TextBlockData *>(currentBlock.userData());
        QVector<ParenthesisInfo *> infos = data->parentheses();
        int pos = currentBlock.position();

        if (firstime) {
            ++i;
            firstime = false;
        } else {
            i = 0;
        }

        int nestLevel = 0;
        for (; i < infos.size(); ++i) {
            ParenthesisInfo *info = infos.at(i);
            if (isSameTypeParenthesis(info->ch, pIndex)) {
                if (info->ch == matchPairs.at(pIndex).left) {
                    ++nestLevel;
                    continue;
                }
                if (info->ch == matchPairs.at(pIndex).right && nestLevel == 0) {
                    createParenthesisSelection(pos + info->pos);
                    return true;
                } else {
                    --nestLevel;
                }
            }
        }
        currentBlock = currentBlock.next();
    } // infinite loop
    return false;
}

bool RegexpTextEditPrivate::matchRightParenthesis(QTextBlock currentBlock, const int infoPos, const int pIndex)
{
    Q_Q(RegexpTextEdit);

    int i = infoPos;
    bool firstime = true;

    for (; ;) {
        if (!currentBlock.isValid()) {
            return false;
        }

        TextBlockData *data =
            static_cast<TextBlockData *>(currentBlock.userData());
        QVector<ParenthesisInfo *> infos = data->parentheses();
        int pos = currentBlock.position();

        if (firstime) {
            --i;
            firstime = false;
        } else {
            i = infos.size() - 1;
        }

        int nestLevel = 0;
        for (; i >= 0; --i) {
            ParenthesisInfo *info = infos.at(i);
            if (isSameTypeParenthesis(info->ch, pIndex)) {
                if (info->ch == matchPairs.at(pIndex).right) {
                    ++nestLevel;
                    continue;
                }
                if (info->ch == matchPairs.at(pIndex).left && nestLevel == 0) {
                    createParenthesisSelection(pos + info->pos);
                    return true;
                } else {
                    --nestLevel;
                }
            }
        }
        currentBlock = currentBlock.previous();
    } // infinite loop
    return false;
}

void RegexpTextEditPrivate::createParenthesisSelection(const int pos)
{
    Q_Q(RegexpTextEdit);
    QList<QTextEdit::ExtraSelection> selections = q->extraSelections();

    QTextEdit::ExtraSelection selection;
    QTextCharFormat format = selection.format;
    format.setBackground(hColor);
    selection.format = format;

    QTextCursor cursor = q->textCursor();
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;

    selections.append(selection);
    q->setExtraSelections(selections);
}

void RegexpTextEditPrivate::setHighlightColor(const QColor &c)
{
    resetSelection();
    _q_match();
}

void RegexpTextEditPrivate::setHighighlightEnabled(const bool b)
{
    Q_Q(RegexpTextEdit);

    if (b) {
        _q_match();
        QObject::connect(q, SIGNAL(cursorPositionChanged()),
                         q, SLOT(_q_match()));
    } else {
        QObject::disconnect(q, SIGNAL(cursorPositionChanged()),
                            q, SLOT(_q_match()));
        resetSelection();
    }
}

void RegexpTextEditPrivate::setTextOptionFlagsEnabled(const bool enabled, const QTextOption::Flag flag)
{
    Q_Q(RegexpTextEdit);

    if (enabled) {
        textOption.setFlags(textOption.flags() | flag);
    } else {
        textOption.setFlags(textOption.flags() & ~flag);
    }

    q->document()->setDefaultTextOption(textOption);
}

void RegexpTextEditPrivate::resetSelection()
{
    Q_Q(RegexpTextEdit);

    QList<QTextEdit::ExtraSelection> selections;
    q->setExtraSelections(selections);
}

void RegexpTextEditPrivate::_q_match()
{
    Q_Q(RegexpTextEdit);

    QList<QTextEdit::ExtraSelection> selections;
    q->setExtraSelections(selections);

    TextBlockData *data =
        static_cast<TextBlockData *>(q->textCursor().block().userData());

    if (data) {
        QVector<ParenthesisInfo *> infos = data->parentheses();
        ParenthesisInfo *info;

        if (infos.isEmpty()) {
            return;
        }

        // absolute position of the cursor within the document
        // The cursor is positioned between characters.
        int absolutePos = q->textCursor().position();

        // index of the block's first character within the block
        int firstBlockCharacterPos = q->textCursor().block().position();

        // cursor position within the block
        int currentPos = absolutePos - firstBlockCharacterPos;

        int direction = 0;
        char par;
        const int size = infos.size();
        int i = 0;
        for (; i < size; ++i) {
            info = infos.at(i);
            if (info->pos == currentPos - 1) {
                par = info->ch;
                if (isLeftParenthesis(info->ch)) { // search right;
                    direction = +1;
                    break;
                } else if (isRightParenthesis(info->ch)) { // search left
                    direction = -1;
                    break;
                }
            }
        }
        if (direction == 0) { // cursor character isn't a parenthesis
            return;
        }

        int pIndex = 0;
        bool ok = findParenthesisIndex(par, &pIndex);
        if (ok) {
            bool found;
            if (direction == -1) {
                found = matchRightParenthesis(q->textCursor().block(),
                                              i, pIndex);
            } else {
                found = matchLeftParenthesis(q->textCursor().block(),
                                             i, pIndex);
            }
            if (found) {
                createParenthesisSelection(firstBlockCharacterPos + info->pos);
            }
        } else {
            qWarning("RegexpTextEditPrivate::_q_match(): '%c' not matchable ",
                     par);
        }
    }
}
