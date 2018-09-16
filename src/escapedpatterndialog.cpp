/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
 * Copyright (C) 2010-2012 Luís Pereira <luis.artur.pereira@gmail.com>
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

#include "escapedpatterndialog.h"
#include "ui_escapedpatterndialog.h"

EscapedPatternDialog::EscapedPatternDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EscapedPatternDialog)
{
    ui->setupUi(this);
}

EscapedPatternDialog::~EscapedPatternDialog()
{
    delete ui;
}

void EscapedPatternDialog::setPattern(const QString &pattern)
{
    QString str = escapePattern(pattern);
    ui->textBrowser->setPlainText(str);
}

void EscapedPatternDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString EscapedPatternDialog::escapePattern(const QString &pattern)
{
    QString str(pattern);

    // escape backslashs and double quotes
    str.replace(QString(QLatin1Char('\\')), QLatin1String("\\\\"));
    str.replace(QString(QLatin1Char('"')), QLatin1String("\\\""));
    str.prepend(QLatin1String("\""));
    str.append(QLatin1String("\""));

    return str;
}
