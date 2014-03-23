/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#include "media.h"

#include "pmcmedia.h"

class PmcMedia::Private
{
public:
    QSharedPointer<Media> media;
};

PmcMedia::PmcMedia(const QSharedPointer< Media > &media, QObject* parent)
    : QObject(parent)
    , d(new Private())
{
    setMedia(media);
}

void PmcMedia::setMedia(const QSharedPointer< Media > &media)
{
    Q_ASSERT(d->media.isNull());

    if (!media.isNull()) {
        d->media = media;
        connect(d->media.data(), SIGNAL(updated()), SIGNAL(updated()));
    }

    emit updated();
}

QString PmcMedia::sha() const
{
    return !d->media.isNull() ? d->media->sha() : QString();
}

QString PmcMedia::thumbnail() const
{
    return !d->media.isNull() ? d->media->thumbnail() : QString();
}

QString PmcMedia::title() const
{
    return !d->media.isNull() ? d->media->title() : QString();
}

QString PmcMedia::type() const
{
    return !d->media.isNull() ? d->media->type() : QString();
}

QString PmcMedia::url() const
{
    return !d->media.isNull() ? d->media->url() : QString();
}

QString PmcMedia::album() const
{
    return !d->media.isNull() && d->media->album() ? d->media->album()->name() : QString();
}

QString PmcMedia::artist() const
{
    return !d->media.isNull() && d->media->artist() ? d->media->artist()->name() : QString();
}

int PmcMedia::duration() const
{
    return !d->media.isNull() ? d->media->duration() : -1;
}
