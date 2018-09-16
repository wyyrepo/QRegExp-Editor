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

#ifndef REGEXPTEXTEDIT_H
#define REGEXPTEXTEDIT_H

#include <QtGui/QColor>
#include <QtGui/QPlainTextEdit>

class RegexpTextEditPrivate;

class RegexpTextEdit : public QPlainTextEdit {
    Q_OBJECT
    Q_DECLARE_PRIVATE(RegexpTextEdit);

public:
    explicit RegexpTextEdit(QWidget *parent = 0);
    explicit RegexpTextEdit(const QString &text, QWidget *parent = 0);
    virtual ~RegexpTextEdit();

    QColor highlightColor() const;
    bool highlightEnabled() const;
    bool showTabsAndSpacesEnabled() const;
    bool showLineAndParagraphSeparatorsEnabled() const;

public Q_SLOTS:
    void setHighlightColor(const QColor &c);
    void setHighlightEnabled(const bool enabled);
    void setShowTabsAndSpacesEnabled(const bool enabled);
    void setShowLineAndParagraphSeparatorsEnabled(const bool s);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_match());
    RegexpTextEditPrivate *const d_ptr;
};

#endif // REGEXPTEXTEDIT_H
