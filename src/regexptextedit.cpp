/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
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

#include "regexptextedit.h"
#include "regexptextedit_p.h"

#include <QtGui/QTextEdit>


RegexpTextEdit::RegexpTextEdit(QWidget *parent)
    : QPlainTextEdit(parent),
      d_ptr(new RegexpTextEditPrivate(this))
{
}

RegexpTextEdit::RegexpTextEdit(const QString &text, QWidget *parent)
    : QPlainTextEdit(text, parent),
      d_ptr(new RegexpTextEditPrivate(this))

{
}

RegexpTextEdit::~RegexpTextEdit()
{
    delete d_ptr;
}

QColor RegexpTextEdit::highlightColor() const
{
    Q_D(const RegexpTextEdit);
    return d->hColor;
}

bool RegexpTextEdit::highlightEnabled() const
{
    Q_D(const RegexpTextEdit);
    return d->highlightEnabled;
}

bool RegexpTextEdit::showTabsAndSpacesEnabled() const
{
    Q_D(const RegexpTextEdit);
    return d->showTabsAndSpaces;
}

bool RegexpTextEdit::showLineAndParagraphSeparatorsEnabled() const
{
    Q_D(const RegexpTextEdit);
    return d->showLineAndParagraphSeparators;
}

void RegexpTextEdit::setHighlightColor(const QColor &c)
{
    Q_D(RegexpTextEdit);
    if (c != d->hColor) {
        d->hColor = c;
        if (d->highlightEnabled) {
            // setHighlightColor() changes the color even when highlight is
            // disabled. Only call it if highlight is enabled.
            d->setHighlightColor(c);
        }
    }
}

void RegexpTextEdit::setHighlightEnabled(const bool b)
{
    Q_D(RegexpTextEdit);
    if (b != d->highlightEnabled) {
        d->highlightEnabled = b;
        d->setHighighlightEnabled(b);
    }
}

void RegexpTextEdit::setShowTabsAndSpacesEnabled(const bool enabled)
{
    Q_D(RegexpTextEdit);
    if (enabled != d->showTabsAndSpaces) {
        d->showTabsAndSpaces = enabled;
        d->setTextOptionFlagsEnabled(enabled, QTextOption::ShowTabsAndSpaces);
    }
}

void RegexpTextEdit::setShowLineAndParagraphSeparatorsEnabled(const bool enabled)
{
    Q_D(RegexpTextEdit);
    if (enabled != d->showLineAndParagraphSeparators) {
        d->showLineAndParagraphSeparators = enabled;
        d->setTextOptionFlagsEnabled(enabled, QTextOption::ShowLineAndParagraphSeparators);
    }
}

#include "regexptextedit.moc"
