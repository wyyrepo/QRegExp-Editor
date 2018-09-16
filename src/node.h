/*
 * Copyright (C) 2010-2012 M. Mehdi Salem Naraghi <momesana@googlemail.com>
 *
 * This file is part of Regexp-Editor.
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

#ifndef NODE_H
#define NODE_H

#include <QtCore/QList>
#include <QtCore/QVariant>

class Node {
public:
    Node(Node *parent = 0);
    Node(const QVariant &data, Node *parent = 0);
    virtual ~Node();
    Node *parent() const;
    void setParent(Node *);
    int childCount() const;
    Node *child(int row) const;
    int row() const;
    int childRow(const Node *node) const;
    void addChild(Node *node);
    void removeChild(const Node *node);
    QVariant data() const ;
    void setData(const QVariant &data);

private:
    Node *m_parent;
    QList<Node *> m_children;
    QVariant m_data;

};

#endif // NODE_H
