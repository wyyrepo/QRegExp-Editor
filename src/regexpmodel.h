/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
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

#ifndef REGEXPMODEL_H
#define REGEXPMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QList>

#include <QtGui/QColor>

class TreeItem;
class QRegExp;

class RegExpModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit RegExpModel(QObject *parent = 0);
    ~RegExpModel();
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void evaluate(const QString &text, const QRegExp &regExp);
    TreeItem *nodeFromIndex(const QModelIndex &index) const;

signals:
    void statusChanged(const QString &);
    void emptyStringMatched(bool);

private:
    TreeItem *m_rootNode;
    QList<QColor> m_colors;
};

#endif // REGEXPMODEL_H
