/***************************************************************************
 *   Copyright 2014 by Shantanu Tushar <shantanu@kde.org>                  *
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

#include "metadatamusicsongsmodel.h"

#include <mediacenter/pmcimagecache.h>
#include <mediacenter/pmccoverartprovider.h>
#include <mediacenter/singletonfactory.h>

#include <KFormat>

MetadataMusicSongsModel::MetadataMusicSongsModel(QObject* parent): PmcMetadataModel(parent)
{

}

MetadataMusicSongsModel::~MetadataMusicSongsModel()
{

}

QVariant MetadataMusicSongsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        const QString songName = PmcMetadataModel::data(index, role).toString();
        const QString artistName = PmcMetadataModel::data(index, MediaCenter::ArtistRole).toString();
        const QString albumName = PmcMetadataModel::data(index, MediaCenter::AlbumRole).toString();

        const int duration = PmcMetadataModel::data(index, MediaCenter::DurationRole).toInt();

        QVariantList list;
        list << songName << albumName << artistName;
        if (duration > 0) {
            list << KFormat().formatDuration(duration * 1000);
        }

        return list;
    } else if (role == Qt::DecorationRole) {
        PmcImageCache *imageCache = SingletonFactory::instanceFor<PmcImageCache>();
        const QString albumName = PmcMetadataModel::data(index, MediaCenter::AlbumRole).toString();

        if (imageCache->containsAlbumCover(albumName)) {
            return PmcCoverArtProvider::qmlImageUriForAlbumCover(albumName);
        }
    }

    return PmcMetadataModel::data(index, role).toString();
}