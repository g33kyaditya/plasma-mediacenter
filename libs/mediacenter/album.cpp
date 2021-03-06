/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "album.h"

#include "media.h"

#include "artist.h"

Album::Album(const QString& name, const QSharedPointer<Artist>& albumArtist)
    : m_albumArtist(albumArtist)
    , m_name(name)
{
}

const QString& Album::name() const
{
    return m_name;
}

QString Album::albumArtist() const
{
    return m_albumArtist->name();
}

void Album::addMedia(const QSharedPointer<Media>& media)
{
    m_media.append(media);
    emit updated();
}

void Album::removeMedia(const QSharedPointer<Media>& media)
{
    m_media.removeOne(media);
    emit updated();
}

QList< QSharedPointer<Media> > Album::media() const
{
    return m_media;
}
