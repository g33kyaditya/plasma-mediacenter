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

#include "artist.h"

#include "media.h"

Artist::Artist(const QString& name)
    : m_name(name)
{
}

const QString& Artist::name() const
{
    return m_name;
}

void Artist::addMedia(const QSharedPointer<Media>& media)
{
    m_media.append(media);
    emit updated();
}

void Artist::removeMedia(const QSharedPointer<Media>& media)
{
    m_media.removeOne(media);
    emit updated();
}

QList< QSharedPointer<Media> > Artist::media() const
{
    return m_media;
}
