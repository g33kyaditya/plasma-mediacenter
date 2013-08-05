/*
    Copyright (C) 2011  Shantanu Tushar shantanu@kde.org
    Copyright (C) 2013  Akshay Ratan <akshayratan@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef LOCALPICTURESMODEL_H
#define LOCALPICTURESMODEL_H

#include "../localfilesabstractmodel.h"

#include "../localthumbnailprovider.h"

class LocalPicturesModel : public LocalFilesAbstractModel
{
  Q_OBJECT
public:
    explicit LocalPicturesModel (QObject* parent);
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

private slots:
    void processThumbnail (const QString& url);

private:
    ThumbnailProvider *m_thumbProvider;
    mutable QHash<QString, QPersistentModelIndex> m_pendingThumbs;
};

#endif // LOCALPICTURESMODEL_H
