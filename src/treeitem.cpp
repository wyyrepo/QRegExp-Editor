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

#include "treeitem.h"

TreeItem::TreeItem(Type type, Node *parent)
    : Node(parent)
    , m_type(type)
{
}

TreeItem::TreeItem(const QVariant &data, Type type, Node *parent)
    : Node(data, parent)
    , m_type(type)
{
}

TreeItem::Type TreeItem::type() const
{
    return m_type;
}

void TreeItem::setType(Type type)
{
    m_type = type;
}
