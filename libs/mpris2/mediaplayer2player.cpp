/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
 *   Copyright 2014 Ashish Madeti <ashishmadeti@gmail.com>                 *
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

#include "mpris2/mediaplayer2player.h"
#include "../mediacenter/medialibrary.h"
#include "../mediacenter/pmcmedia.h"
#include "../mediacenter/singletonfactory.h"

#include <QCryptographicHash>
#include <QDBusConnection>
#include <QMetaClassInfo>
#include <QDBusMessage>
#include <QStringList>
#include <QVariant>

#include <KUrl>
#include <KStandardDirs>

static const double MAX_RATE = 32.0;
static const double MIN_RATE = 0.0;

MediaPlayer2Player::MediaPlayer2Player(QObject* parent)
    : QObject(parent),
      m_paused(false),
      m_stopped(true)
{
    QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this,
                                                 QDBusConnection::ExportAllSlots |
                                                 QDBusConnection::ExportAllProperties |
                                                 QDBusConnection::ExportAllSignals);
}

MediaPlayer2Player::~MediaPlayer2Player()
{
}

QString MediaPlayer2Player::PlaybackStatus() const
{
    if (stopped()) {
        return QLatin1String("Stopped");
    } else if (paused()) {
        return QLatin1String("Paused");
    } else
        return QLatin1String("Playing");
}

bool MediaPlayer2Player::CanGoNext() const
{
    return (mediaPlayerPresent()) ? true : false;
}

void MediaPlayer2Player::Next() const
{
    emit next();
}

bool MediaPlayer2Player::CanGoPrevious() const
{
    return (mediaPlayerPresent()) ? true : false;
}

void MediaPlayer2Player::Previous() const
{
    emit previous();
}

bool MediaPlayer2Player::CanPause() const
{
    return (mediaPlayerPresent()) ? true : false;
}

void MediaPlayer2Player::Pause() const
{
    emit pause();
}

void MediaPlayer2Player::PlayPause()
{
    emit playPause();
}

bool MediaPlayer2Player::stopped() const
{
    return m_stopped;
}

void MediaPlayer2Player::setStopped(bool newVal)
{
    if (mediaPlayerPresent()) {
        m_stopped = newVal;

        QVariantMap properties;
        properties["PlaybackStatus"] = PlaybackStatus();
        MediaPlayer2Player::signalPropertiesChange(properties);
    }
}

bool MediaPlayer2Player::paused() const
{
    return m_paused;
}

void MediaPlayer2Player::setPaused(bool newVal)
{
    if (mediaPlayerPresent()) {
        m_paused = newVal;

        QVariantMap properties;
        properties["PlaybackStatus"] = PlaybackStatus();
        MediaPlayer2Player::signalPropertiesChange(properties);
    }
}

void MediaPlayer2Player::Stop() const
{
    emit stop();
}

bool MediaPlayer2Player::CanPlay() const
{
    return (mediaPlayerPresent()) ? true : false;
}

void MediaPlayer2Player::Play() const
{
    emit play();
}

double MediaPlayer2Player::Volume() const
{
    return m_volume;
}

void MediaPlayer2Player::setVolume(double volume)
{
    m_volume= qBound(0.0, volume, 1.0);
    emit volumeChanged(m_volume);

    QVariantMap properties;
    properties["Volume"] = Volume();
    MediaPlayer2Player::signalPropertiesChange(properties);
}

QVariantMap MediaPlayer2Player::Metadata() const
{
    return m_metadata;
}

qlonglong MediaPlayer2Player::Position() const
{
    return m_position;
}

void MediaPlayer2Player::setPropertyPosition(int newPositionInMs)
{
    m_position = qlonglong(newPositionInMs)*1000;
    //PMC stores postion in milli-seconds, Mpris likes it in micro-seconds
}

double MediaPlayer2Player::Rate() const
{
    return m_rate;
}

void MediaPlayer2Player::setRate(double newRate)
{
    m_rate = qBound(MinimumRate(), newRate, MaximumRate());
    emit rateChanged(newRate);

    QVariantMap properties;
    properties["Rate"] = Rate();
    MediaPlayer2Player::signalPropertiesChange(properties);
}

double MediaPlayer2Player::MinimumRate() const
{
    return double(MIN_RATE);
}

double MediaPlayer2Player::MaximumRate() const
{
    return double(MAX_RATE);
}

bool MediaPlayer2Player::CanSeek() const
{
    return (mediaPlayerPresent()) ? true : false;
}

bool MediaPlayer2Player::CanControl() const
{
    return true;
}

void MediaPlayer2Player::Seek(qlonglong Offset) const
{
    int offset = Offset/1000;
    emit seek(offset);
}

void MediaPlayer2Player::emitSeeked(int pos)
{
    emit Seeked(qlonglong(pos)*1000);
}

void MediaPlayer2Player::SetPosition(const QDBusObjectPath& trackId, qlonglong pos)
{
    if (trackId.path() == getTrackID())
        seek((pos - m_position)/1000);
}

void MediaPlayer2Player::OpenUri(QString uri) const
{
    KUrl url(uri);
    if (url.isLocalFile()) {
        //TODO: Play the url
    }
}

QUrl MediaPlayer2Player::currentTrack() const
{
    return m_currentTrack;
}

void MediaPlayer2Player::setCurrentTrack(QUrl newTrack)
{
    m_currentTrack = newTrack;
    loadMetadata();

    QVariantMap properties;
    properties["Metadata"] = Metadata();
    MediaPlayer2Player::signalPropertiesChange(properties);
}

bool MediaPlayer2Player::mediaPlayerPresent() const
{
    return m_mediaPlayerPresent;
}

void MediaPlayer2Player::setMediaPlayerPresent(bool status)
{
    if (m_mediaPlayerPresent != status) {
        m_mediaPlayerPresent = status;

        QVariantMap properties;
        properties["CanGoNext"] = CanGoNext();
        properties["CanGoPrevious"] = CanGoPrevious();
        properties["CanPause"] = CanPause();
        properties["CanPlay"] = CanPlay();
        properties["CanSeek"] = CanSeek();
        MediaPlayer2Player::signalPropertiesChange(properties);
    }
}

void MediaPlayer2Player::loadMetadata()
{
    QSharedPointer<PmcMedia> media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(m_currentTrack.toString());
    if (media)
    {
        m_metadata["mpris:trackid"] = QVariant::fromValue<QDBusObjectPath>(QDBusObjectPath(getTrackID()));
        m_metadata["mpris:length"] = qlonglong(media->duration())*1000000;
        //convert seconds into micro-seconds
        m_metadata["xesam:title"] = media->title();
        m_metadata["xesam:url"] = media->url();
        m_metadata["xesam:album"] = media->album();
        m_metadata["xesam:artist"] = QStringList(media->artist());
        m_metadata["xesam:genre"] = QStringList(media->genre());

    }
}

QString MediaPlayer2Player::getTrackID()
{
    QSharedPointer<PmcMedia> media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(m_currentTrack.toString());
    if (media) {
        return QString("/org/kde/plasmamediacenter/tid_") + media->sha();
    } else {
        return QString("/org/mpris/MediaPlayer2/TrackList/NoTrack");
    }
    //TODO: In future if "Tracklist" interface is implemented,
    //consider the playlist postion also in assigning the TrackID
}

void MediaPlayer2Player::signalPropertiesChange(const QVariantMap& properties) const
{
    const int ifaceIndex = metaObject()->indexOfClassInfo("D-Bus Interface");
    QDBusMessage msg = QDBusMessage::createSignal("/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties", "PropertiesChanged");

    msg << metaObject()->classInfo(ifaceIndex).value();
    msg << properties;
    msg << QStringList();

    QDBusConnection::sessionBus().send(msg);
}