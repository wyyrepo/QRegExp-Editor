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

#include "node.h"

Node::Node(Node *parent)
    : m_parent(parent)
{
    if (m_parent) {
        m_parent->addChild(this);
    }
}

Node::Node(const QVariant &data, Node *parent)
    : m_parent(parent)
    , m_data(data)
{
    if (m_parent) {
        m_parent->addChild(this);
    }
}

Node::~Node()
{
    qDeleteAll(m_children);
}

Node *Node::parent() const
{
    return m_parent;
}

void Node::setParent(Node *parent)
{
    if (m_parent == parent) {
        return;
    }

    m_parent = parent;
    if (parent) {
        parent->addChild(this);
    }
}

int Node::childCount() const
{
    return m_children.count();
}

Node *Node::child(int row) const
{
    if (row < 0 || row >= m_children.count()) {
        return 0;
    }

    return m_children.at(row);
}

int Node::row() const
{
    if (!m_parent) {
        return 0;
    }

    return m_parent->childRow(this);
}

int Node::childRow(const Node *node) const
{
    if (m_children.contains(const_cast<Node *>(node))) {
        return m_children.indexOf(const_cast<Node *>(node));
    }

    return -1;
}

void Node::addChild(Node *node)
{
    if (!node) {
        return;
    }

    if (!m_children.contains(const_cast<Node *>(node))) {
        m_children.append(node);
    }

    node->setParent(this);
}

void Node::removeChild(const Node *node)
{
    m_children.removeAll(const_cast<Node *>(node));
}

QVariant Node::data() const
{
    return m_data;
}

void Node::setData(const QVariant &data)
{
    if (data != m_data) {
        m_data = data;
    }
}
