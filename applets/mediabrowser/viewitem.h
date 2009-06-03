/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef VIEWITEM_H
#define VIEWITEM_H

#include <QGraphicsWidget>
#include <QStyleOptionViewItemV4>
#include <QModelIndex>

namespace Plasma {
    class FrameSvg;
}

class ViewItem : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum ItemType { LocalFileItem, RemoteFileItem };

    ViewItem(QGraphicsItem *parent = 0);
    ~ViewItem();

    void setStyleOption(const QStyleOptionViewItemV4 &);
    QStyleOptionViewItemV4 styleOption() const;

    void setModelIndex(const QModelIndex &index);

    void setItemType(ItemType);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QStyleOptionViewItemV4 m_option;
    QModelIndex m_index;
    ItemType m_type;
    Plasma::FrameSvg *m_frameSvg;
};

#endif // VIEWITEM_H
