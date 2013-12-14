/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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

#ifndef METADATAMUSICBACKEND_H
#define METADATAMUSICBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>

#include "../abstractmetadatabackend.h"
#include "categoriesmodel.h"

class MetadataBackendCommonModel;
class AlwaysExpandedMetadataModel;
class PlaylistModel;
class CategoriesModel;
class NepomukMusicModel;
class MetadataBackendCommonModel;
class ModelMetadata;

class MetadataMusicBackend : public AbstractMetadataBackend
{
    Q_OBJECT
    Q_PROPERTY(QString artistFilter READ artistFilter WRITE setArtistFilter NOTIFY artistFilterChanged)
    Q_PROPERTY(QString albumFilter READ albumFilter WRITE setAlbumFilter NOTIFY albumFilterChanged)

public:
    MetadataMusicBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataMusicBackend();

    QString artistFilter() const;
    QString albumFilter() const;

    void setArtistFilter(const QString &filter);
    void setAlbumFilter(const QString &filter);

    Q_INVOKABLE void searchArtist(const QString &artist);
    Q_INVOKABLE void searchAlbum(const QString &album);
    Q_INVOKABLE void searchMusic(const QString &music);

    Q_INVOKABLE void addAllSongsToPlaylist( QObject* playlistModel );
    Q_INVOKABLE void stopAddingSongsToPlaylist();

    virtual bool expand(int row, QAbstractItemModel* model);

    virtual QVariantList buttons();
    virtual void handleButtonClick(const QString& buttonName);

    virtual void searchModel(const QString& searchTerm, QAbstractItemModel* model);

Q_SIGNALS:
    void artistFilterChanged();
    void albumFilterChanged();

protected:
    void updateModelAccordingToFilters();
    void clearFilters();
    virtual bool initImpl();

private slots:
    void musicModelDataChanged ( const QModelIndex& startIndex, const QModelIndex& endIndex );
    void musicModelReset();
    void initializeModels();

private:
    AlwaysExpandedMetadataModel* m_artistsModel;
    AlwaysExpandedMetadataModel* m_albumsModel;
    MetadataBackendCommonModel* m_musicModel;
    PlaylistModel* m_playlistModel;

    QString m_artistFilter;
    QString m_albumFilter;
    bool m_shallAddMediaToPlaylist : 1;
    ModelMetadata* m_musicModelMetadata;
    ModelMetadata* m_albumsModelMetadata;
    ModelMetadata* m_artistsModelMetadata;
};

#endif // METADATAMUSICBACKEND_H
