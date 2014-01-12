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

#ifndef PMCMEDIA_H
#define PMCMEDIA_H

#include "media.h"
#include <QString>
#include <QtCore/QSharedPointer>

class PmcMedia
{
public:
    PmcMedia(QSharedPointer<Media> media);
    ~PmcMedia();

    const QString& sha() const;
    const QString& title () const;
    const QString& url() const;
    const QString& thumbnail () const;
    const QString& type() const;

private:
    QSharedPointer<Media> m_media;

};

#endif // PMCMEDIA_H
