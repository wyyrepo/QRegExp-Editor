/*
 * Copyright (C) 2012 Luís Pereira <luis.artur.pereira@gmail.com>
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
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#include "regexppreferencespage.h"
#include "ui_regexppreferencespage.h"
#include "settingsinterface.h"
#include "regexpsettings.h"

#include <QtGui/QAction>
#include <QtGui/QColorDialog>

RegexpPreferencesWidget::RegexpPreferencesWidget(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::RegexpPreferencesWidget)
{
    m_ui->setupUi(this);

    QPixmap pix;
    actionHighlightMatchColor = new QAction(pix,
                                            tr("Choose highlight color"), this);
    connect(actionHighlightMatchColor, SIGNAL(triggered()),
            this, SLOT(highlightMatchColor()));
    m_ui->highlightMatchColor->setDefaultAction(actionHighlightMatchColor);

}

RegexpPreferencesWidget::~RegexpPreferencesWidget()
{
    delete m_ui;
}

void RegexpPreferencesWidget::highlightMatchColor()
{
    QColor c = QColorDialog::getColor(rc.highlightMatchColor, this);
    if (!c.isValid()) {
        return;
    }
    highlightMatchColorChanged(c);
}

void RegexpPreferencesWidget::highlightMatchColorChanged(const QColor &color)
{
    QPixmap pix(22, 22);
    pix.fill(color);
    actionHighlightMatchColor->setIcon(pix);
    rc.highlightMatchColor = color;
}

RegexpOptions RegexpPreferencesWidget::regexpOptions() const
{
    return rc;
}

void RegexpPreferencesWidget::setRegexpOptions(const RegexpOptions &opt)
{
    rc = opt;
    highlightMatchColorChanged(opt.highlightMatchColor);

}

RegexpPreferencesPage::RegexpPreferencesPage(QWidget *parent, SettingsInterface *s)
{
    m_widget = new RegexpPreferencesWidget(parent);
    Q_CHECK_PTR(m_widget);
    if (!m_widget) {
        qFatal("RegexpPreferencesPage::RegexpPreferencesPage(): null m_widget");
    }

    if (RegexpSettings *is = dynamic_cast<RegexpSettings *>(s)) {
        settings = is;
        setSettings();
    } else {
        qFatal("RegexpPreferencesPage::RegexpPreferencesPage(): "\
               "s doesn't point to an RegexpSettings");
    }
}


QWidget *RegexpPreferencesPage::widget() const
{
    return m_widget;
}

void RegexpPreferencesPage::apply()
{
    const RegexpOptions newOptions = m_widget->regexpOptions();
    if (m_initialOptions != newOptions) {
        m_initialOptions = newOptions;
        settings->setOptions(newOptions);
    }
}

void RegexpPreferencesPage::finish()
{
}

void RegexpPreferencesPage::setSettings()
{
    settings->fromSettings();
    m_initialOptions = settings->options();
    m_widget->setRegexpOptions(m_initialOptions);
}

void RegexpPreferencesPage::updateSettings()
{
    m_initialOptions = settings->options();
    m_widget->setRegexpOptions(m_initialOptions);
}

void RegexpPreferencesPage::updateWidget()
{
    m_widget->setRegexpOptions(m_initialOptions);
}
