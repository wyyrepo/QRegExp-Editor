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

#ifndef TREEITEM_H
#define TREEITEM_H

#include "node.h"

class Item;
class TreeItem : public Node {
public:
    enum Type { ROOT = 0x0, STRING, SUB_STRING, POSITION, LENGTH };
    TreeItem(Type type, Node *parent = 0);
    TreeItem(const QVariant &data, Type type, Node *parent = 0);
    Type type() const;
    void setType(Type type);

private:
    Type m_type;
};

#endif // TREEITEM_H
