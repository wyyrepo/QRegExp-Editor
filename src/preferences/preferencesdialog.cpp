/*
 * Copyright (C) 2011-2012 Luís Pereira <luis.artur.pereira@gmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QRegExp-Editor. If not, see <http://www.gnu.org/licenses/>.
 */

#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "preferencespageinterface.h"
#include "constants.h"

#include <QtGui/QPushButton>
#include <QtCore/QDebug>

PreferencesDialog::PreferencesDialog(QWidget *parent, const QString &title)
    : QDialog(parent),
      m_ui(new Ui::PreferencesDialog())
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    m_ui->setupUi(this);
    setWindowTitle(title);

    m_ui->m_listWidget->setViewMode(QListView::IconMode);
    m_ui->m_listWidget->setIconSize(QSize(32, 32));
    m_ui->m_listWidget->setMovement(QListView::Static);
    m_ui->m_listWidget->setUniformItemSizes(true);
    m_ui->m_listWidget->setSpacing(12);
    m_ui->m_listWidget->setVerticalScrollMode(QListView::ScrollPerPixel);

    QFont boldFont(font());
    boldFont.setBold(true);
    m_ui->m_listWidget->setFont(boldFont);

    connect(m_ui->m_dialogButtonBox, SIGNAL(rejected()),
            this, SLOT(slotRejected()));
    connect(m_ui->m_dialogButtonBox, SIGNAL(accepted()),
            this, SLOT(slotAccepted()));
    connect(applyButton(), SIGNAL(clicked()),
            this, SLOT(slotApply()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete m_ui;
}

void PreferencesDialog::addPage(PreferencesPageInterface *page, const QIcon &icon, const QString &title)
{
    Q_CHECK_PTR(page);
    if (!page) {
        qDebug() << Q_FUNC_INFO << "Trying to add null page, ignoring it";
        return;
    }

    QListWidgetItem *item = new QListWidgetItem(m_ui->m_listWidget);
    Q_CHECK_PTR(item);
    if (!item) {
        qWarning() << Q_FUNC_INFO << "item is null, ignoring it";
        return;
    }

    if (icon.isNull()) {
        item->setIcon(QIcon(QLatin1String(ICON_UNKNOWN)));
    } else {
        item->setIcon(icon);
    }

    item->setText(title);
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_listWidget->addItem(item);

    m_ui->m_stackedWidget->addWidget(page->widget());

    connect(m_ui->m_listWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem *)));

    m_preferencesPages.append(page);
    updateWidth();
}

void PreferencesDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) {
        current = previous;
    }
    m_ui->m_stackedWidget->setCurrentIndex(m_ui->m_listWidget->row(current));
}

void PreferencesDialog::updateWidth()
{
    int rows = m_ui->m_listWidget->count();

    int width = 0;
    for (int i = 0; i < rows; ++i) {
        width = qMax(width, m_ui->m_listWidget->sizeHintForRow(i));
    }
    m_ui->m_listWidget->setFixedWidth(width + 29);
}

QPushButton *PreferencesDialog::applyButton() const
{
    return m_ui->m_dialogButtonBox->button(QDialogButtonBox::Apply);
}

void PreferencesDialog::slotAccepted()
{
    slotApply();
    accept();
}

void PreferencesDialog::slotRejected()
{
    foreach(PreferencesPageInterface * preferencesPage, m_preferencesPages) {
        preferencesPage->updateWidget();
    }
    reject();
}

void PreferencesDialog::slotApply()
{
    foreach(PreferencesPageInterface * preferencesPage, m_preferencesPages) {
        preferencesPage->apply();
    }
}
