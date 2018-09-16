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

#include "regexpmodel.h"
#include "treeitem.h"

// Qt
#include <QtCore/QDebug>

#include <QtGui/QFont>

RegExpModel::RegExpModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(0)
{
    m_colors << Qt::red << Qt::yellow << Qt::cyan << Qt::magenta
             << Qt::blue << Qt::green << Qt::darkGray << Qt::white;
}

RegExpModel::~RegExpModel()
{
    delete m_rootNode;
}

void RegExpModel::evaluate(const QString &text, const QRegExp &regExp)
{
    beginResetModel();
    delete m_rootNode;
    m_rootNode = new TreeItem(TreeItem::ROOT, 0);
    unsigned int matchesCnt = 0;
    const int count = regExp.captureCount() + 1;
    int pos = 0;
    bool empty = false; // is the empty string being matched? (leads to an infinite loop)

    while ((pos = regExp.indexIn(text, pos)) != -1) {
        const int len = regExp.matchedLength();
        if (len == 0) {
            qWarning() << "In RegExpModel::evaluate() -> matchedLength() == 0";
            empty = true;
            break;
        }

        qDebug() << tr("************** Match **************\n%1").arg(regExp.cap(0));
        TreeItem *stringNode = new TreeItem(regExp.cap(0), TreeItem::STRING, m_rootNode);
        for (int i = 1; i < count; ++i) {
            const QString subStr = regExp.cap(i);
            qDebug() << tr("--> Capture %1: %2").arg(QString::number(i)).arg(subStr);
            TreeItem *subStringNode = new TreeItem(TreeItem::SUB_STRING, stringNode);
            subStringNode->setData(subStr);

            if (!subStr.isEmpty()) {
                qDebug() << tr("------> Pos:%1").arg(QString::number(regExp.pos(i)));
                new TreeItem(regExp.pos(i), TreeItem::POSITION, subStringNode);
            }
        }

        new TreeItem(regExp.pos(), TreeItem::POSITION, stringNode);
        new TreeItem(len, TreeItem::LENGTH, stringNode);
        pos += len;
        ++matchesCnt;
    }
    endResetModel();

    // updating information
    bool cs = regExp.caseSensitivity() == Qt::CaseSensitive;
    bool min = regExp.isMinimal();
    QString ps;
    switch (regExp.patternSyntax()) {
    case QRegExp::RegExp:
        ps = "RegExp";
        break;
    case QRegExp::Wildcard:
        ps = "Wildcard";
        break;
    case QRegExp::FixedString:
        ps = "FixedString";
        break;
    case QRegExp::RegExp2:
        ps = "RegExp2";
        break;
    case QRegExp::WildcardUnix:
        ps = "WildcardUnix";
        break;
    case QRegExp::W3CXmlSchema11:
        ps = "W3CXmlSchema11";
        break;
    }
    emit statusChanged(tr("Matches: %1  Case-sensitive: %2  Minimal: %3  Style: %4")
                       .arg(QString::number(matchesCnt))
                       .arg(cs ? tr("On") : tr("Off"))
                       .arg(min ? tr("On") : tr("Off"))
                       .arg(ps));
    emit emptyStringMatched(empty);
}

TreeItem *RegExpModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_rootNode;
    }

    return static_cast<TreeItem *>(index.internalPointer());
}

QModelIndex RegExpModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootNode || !hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    Node *parentNode = nodeFromIndex(parent);
    Node *childNode = parentNode->child(row);
    if (!childNode) {
        return QModelIndex();
    }

    return createIndex(row, column, childNode);
}

QModelIndex RegExpModel::parent(const QModelIndex &index) const
{
    if (!m_rootNode || !index.isValid()) {
        return QModelIndex();
    }

    Node *node = nodeFromIndex(index);
    Node *parentNode = node->parent();
    if (parentNode == m_rootNode) {
        return QModelIndex();
    }

    return createIndex(parentNode->row(), 0, parentNode);
}

int RegExpModel::rowCount(const QModelIndex &parent) const
{
    if (!m_rootNode || parent.column() > 0) {
        return 0;
    }

    Node *n = nodeFromIndex(parent);
    return n->childCount();
}

int RegExpModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant RegExpModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootNode) {
        return QVariant();
    }

    TreeItem *t = nodeFromIndex(index);
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            switch (t->type()) {
            case TreeItem::STRING:
                return t->data();
            case TreeItem::SUB_STRING:
                return QString("Cap (%1)").arg(t->row() + 1);
            case TreeItem::POSITION:
                return tr("Position");
            case TreeItem::LENGTH:
                return tr("Length");
            default:
                return QVariant();
            }
        }

        if (index.column() == 1) {
            switch (t->type()) {
            case TreeItem::SUB_STRING: {
                QString str = t->data().toString();
                str = str.isEmpty() ?  tr("(Empty)") : str;
                return str;
            }
            case TreeItem::POSITION:
            case TreeItem::LENGTH:
                return t->data();
            default:
                return QVariant();
            }
        }
    }

    if (role == Qt::BackgroundRole && t->type() == TreeItem::STRING) {
        return m_colors.at(t->row() % m_colors.count()).lighter(175);
    }


    if (role == Qt::ForegroundRole && t->type() == TreeItem::SUB_STRING && index.column() == 1) {
        if (t->data().toString().isEmpty()) {
            return Qt::gray;
        }
    }

    if (role == Qt::FontRole && t->type() == TreeItem::SUB_STRING && index.column() == 1) {
        if (t->data().toString().isEmpty()) {
            QFont f;
            f.setItalic(true);
            return f;
        }
    }

    return QVariant();
}

QVariant RegExpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Type");
        case 1:
            return tr("Value");
        }
    }

    return QVariant();
}
