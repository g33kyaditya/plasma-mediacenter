/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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


#include "metadatamusicmodel.h"
#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <QtGui/QIcon>
#include <QEvent>


MetadataMusicModel::MetadataMusicModel (QObject* parent)
    : AbstractMetadataModel (parent)
{
    if (metadataModel()) {
        metadataModel()->setProperty("resourceType", "nfo:Audio");
        metadataModel()->setProperty("mimeType", "");
        metadataModel()->setProperty("limit", 500);
    }
    else {
        kDebug() << "WARNING: Constructor called before metadataModel set :/";
    }
}

MetadataMusicModel::~MetadataMusicModel()
{

}

QVariant MetadataMusicModel::data (const QModelIndex& index, int role) const
{
    if (!metadataModel()) {
        return QVariant();
    }

    if(role == MediaCenter::MediaTypeRole) {
        return "audio";
    }
    else {
        return AbstractMetadataModel::data(index, role);
    }
}

#include "metadatamusicmodel.moc"
