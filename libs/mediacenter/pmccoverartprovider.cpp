/***********************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
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

/* Part of this code was taken from project Taglib, originally licensed under
 * the BSD-2-clause license. You can find the original version at
 * https://github.com/taglib/taglib/tree/master/examples
*/

#include "pmccoverartprovider.h"

#include <id3v2tag.h>
#include <mpegfile.h>
#include <attachedpictureframe.h>

#include <QtCore/QUrl>

const char *PmcCoverArtProvider::identificationString = "pmccoverart";

PmcCoverArtProvider::PmcCoverArtProvider()
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
}

QImage PmcCoverArtProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    const QUrl url = id;
    const QString file = url.toLocalFile();
    TagLib::MPEG::File f(file.toUtf8().constData());

    if(!f.isValid() || !f.ID3v2Tag())
        return QImage();

    TagLib::ID3v2::FrameList l = f.ID3v2Tag()->frameList("APIC");
    QImage image;

    if(l.isEmpty()) {
        return image;
    }
    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
    image.loadFromData((const uchar *) frame->picture().data(), frame->picture().size());

    if (size) {
        *size = image.size();
    }
    return image;
}
