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
#include "playlistjob.h"
#include "playlist.h"

PlaylistJob::PlaylistJob(PlaylistEngine *engine,
        const QString &destination, const QString &operation, const QMap<QString, QVariant> &params, QObject *parent)
            : Plasma::ServiceJob(destination, operation, params, parent), m_engine(engine)
{
}

void PlaylistJob::start()
{
    if (destination().isEmpty()) {
        kDebug() << "destination is empty";
        return;
    }

    if (destination() == "currentPlaylist") { // no need for recursive setting
        return;
    }

    if (operationName() == "setCurrent") {
        m_engine->setCurrentPlaylist(destination());
    } else if (operationName() == "add") {
        m_engine->addToPlaylist(destination(), parameters()["path"].toString());
    } else if (operationName() == "remove") {
        m_engine->removeFromPlaylist(destination(), parameters()["path"].toString());
    } else if (operationName() == "addAt") {
        m_engine->addToPlaylist(destination(), parameters()["path"].toString(), parameters()["index"].toInt());
    }

    emitResult();
}