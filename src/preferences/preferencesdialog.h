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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QListWidget>

class PreferencesPageInterface;

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0, const QString &title = QString());
    ~PreferencesDialog();

    void addPage(PreferencesPageInterface *page, const QIcon &icon, const QString &title = QString());

public Q_SLOTS:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private Q_SLOTS:
    void slotAccepted();
    void slotRejected();
    void slotApply();

    void updateWidth();
private:

    Ui::PreferencesDialog *m_ui;
    QList<PreferencesPageInterface *> m_preferencesPages;
    QListWidget m_listWidget;

    QPushButton *applyButton() const;
};

#endif // PREFERENCESDIALOG_H
