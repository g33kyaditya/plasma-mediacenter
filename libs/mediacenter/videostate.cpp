/***************************************************************************
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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
#include "videostate.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>
#include <mediacenter/infodisplay.h>
#include <mediacenter/medialayout.h>
#include <mediacenter/private/sharedlayoutcomponentsmanager.h>

#include <nepomuk/kratingwidget.h>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>

#include <QtCore/QFileInfo>


using namespace MediaCenter;

VideoState::VideoState (QState *parent)
    : MediaCenterState(parent),
    m_videoVolumeSlider(new Plasma::Slider()),
    m_videoSeekSlider(new Plasma::Slider()),
    m_videoSkipBack(new Plasma::IconWidget()),
    m_videoPlayPause(new Plasma::IconWidget()),
    m_videoSkipForward(new Plasma::IconWidget()),
    m_videoStop(new Plasma::IconWidget()),
    m_videoTogglePlaylist(new Plasma::IconWidget()),
    m_selectedMediasLabel(new Plasma::IconWidget()),
    m_currentlyPlayingLabel(new Plasma::IconWidget()),
    m_videoObject(new Phonon::MediaObject()),
    m_lastDirectory(KUrl()),
    m_resource(0),
    m_ratingWidget(new KRatingWidget()),
    m_ratingProxyWidget(new QGraphicsProxyWidget()),
    m_nepomuk(false)
{
    m_nepomuk = Nepomuk::ResourceManager::instance()->initialized();
}

VideoState::~VideoState()
{
}

void VideoState::onExit(QEvent* event)
{
    Q_UNUSED(event);

    m_lastDirectory = m_browser->directory();

    if (m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState) {
        SharedLayoutComponentsManager::self()->setBackgroundVideoMode(true);
    } else {
        SharedLayoutComponentsManager::self()->setBackgroundVideoMode(false);;
    }

    disconnect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueueVideos(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playVideoMedia(MediaCenter::Media)));
    disconnect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterPlayingState()));
}

void VideoState::onEntry(QEvent* event)
{
    Q_UNUSED(event);

    emit state(MediaCenter::VideoMode);

    showBackgroundStates();
    //Do not show the current state's background state icon
    SharedLayoutComponentsManager::self()->backgroundVideoWidget()->setVisible(false);

    if (m_lastDirectory.hasPath()) {
         m_browser->openDirectory(m_lastDirectory);
         //TODO: Have the browser load the correct modelpackage
    }

    connect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueueVideos(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playVideoMedia(const MediaCenter::Media&)));
    connect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterPlayingState()));
}

QList<QGraphicsWidget*> VideoState::subComponents() const
{
    QList<QGraphicsWidget*> list;

    m_videoSkipBack->setIcon("media-skip-backward");
    list << m_videoSkipBack;
    m_control->addToLayout(m_videoSkipBack, MediaCenter::MiddleZone);

    m_videoPlayPause->setIcon("media-playback-start");
    list << m_videoPlayPause;
    m_control->addToLayout(m_videoPlayPause, MediaCenter::MiddleZone);

    m_videoStop->setIcon("media-playback-stop");
    list << m_videoStop;
    m_control->addToLayout(m_videoStop, MediaCenter::MiddleZone);

    m_videoSkipForward->setIcon("media-skip-forward");
    list << m_videoSkipForward;
    m_control->addToLayout(m_videoSkipForward, MediaCenter::MiddleZone);

    m_videoSeekSlider->setRange(0, 100);
    m_videoSeekSlider->setOrientation(Qt::Horizontal);
    list << m_videoSeekSlider;
    m_control->addToLayout(m_videoSeekSlider, MediaCenter::MiddleZone);

    m_videoVolumeSlider->setRange(0, 100);
    m_videoVolumeSlider->setOrientation(Qt::Vertical);
    list << m_videoVolumeSlider;
    m_control->addToLayout(m_videoVolumeSlider, MediaCenter::MiddleZone);

    m_videoTogglePlaylist->setIcon("view-media-playlist");
    list << m_videoTogglePlaylist;
    m_control->addToLayout(m_videoTogglePlaylist, MediaCenter::RightZone);


    list << m_currentlyPlayingLabel;
    m_currentlyPlayingLabel->setMinimumSize(230,20);
    m_currentlyPlayingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_currentlyPlayingLabel, MediaCenter::LeftZone);

    m_ratingProxyWidget->setWidget(m_ratingWidget);
    m_ratingWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_ratingWidget->setMaximumSize(170,35);
    list << m_ratingProxyWidget;
    m_infoDisplay->addToLayout(m_ratingProxyWidget, MediaCenter::MiddleZone);

    list << m_selectedMediasLabel;
    m_selectedMediasLabel->setMinimumSize(230,20);
    m_selectedMediasLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_selectedMediasLabel, MediaCenter::MiddleZone);

    return list;
}

void VideoState::configure()
{
    //Show/hide correct applets
    m_control->setVisible(true);
    m_infoDisplay->setVisible(true);
    if (m_player->videoPlayerPlaybackState() != MediaCenter::PlayingState) {
        m_player->setVisible(false);
        m_playlist->setVisible(true);
        m_browser->setVisible(true);
        m_layout->setControlAutohide(false);
        m_layout->setPlaylistVisible(true);

        enterBrowsingState();
   } else {
        m_player->setVisible(true);
        m_playlist->setVisible(true);
        m_browser->setVisible(false);
        m_layout->setControlAutohide(true);
        m_layout->setPlaylistVisible(false);

        enterPlayingState();
    }

    m_browser->clearViewModes();
    m_browser->addViewMode("Videomode 1");
    m_browser->addViewMode("Videomode 2");
    m_browser->setShowingBrowsingWidgets(true);

    //connect seek slider to media object
    receivedMediaObject();
    if (m_videoObject) {
        m_videoSeekSlider->setValue(m_videoObject->currentTime());
    }
    m_videoVolumeSlider->setValue(int(m_player->volume()*100));

    m_player->setPlayerType(MediaCenter::Video);
    m_player->setCurrentVideoMedia(m_videoMedia); //the player needs to know what the user wants to control with the buttons
    m_playlist->setPlaylistMediaType(MediaCenter::Video); //TODO: Find a better solution for playlists
    m_player->clearVideoQueue();
    m_player->enqueueVideos(m_playlist->medias(MediaCenter::Video));

    m_browser->clearSelectedMedias();
    updateInfoDisplay();
}

void VideoState::initConnections()
{
    connect (m_videoTogglePlaylist, SIGNAL(clicked()), m_layout, SLOT(togglePlaylistVisible()));
    connect (m_videoSeekSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(seekVideo(int)));
    connect (m_videoStop, SIGNAL(clicked()), this, SLOT(enterBrowsingState()));
    connect (m_videoPlayPause, SIGNAL(clicked()), this, SLOT(enterPlayingState()));
    connect (m_videoSkipForward, SIGNAL(clicked()), m_player, SLOT(skipVideoForward()));
    connect (m_videoSkipBack, SIGNAL(clicked()), m_player, SLOT(skipVideoBackward()));
    connect (m_videoVolumeSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(setVolume(int)));
    connect (m_player, SIGNAL(newVideoMedia(MediaCenter::Media)), this, SLOT(setMedia(MediaCenter::Media)));
    connect (m_player, SIGNAL(newVideoObject(Phonon::MediaObject*)), this, SLOT(setMediaObject(Phonon::MediaObject*)));
    connect (m_player, SIGNAL(videoPlaybackStateChanged(MediaCenter::PlaybackState)), this, SLOT(onPlaybackStateChanged(MediaCenter::PlaybackState)));
    connect (m_ratingWidget, SIGNAL(ratingChanged(int)), this, SLOT(slotRatingChanged(int)));
}

void VideoState::updateTotalTime(const qint64 time)
{
    m_videoSeekSlider->setRange(0, time);
}

void VideoState::updateCurrentTick(const qint64 time)
{
    m_videoSeekSlider->setValue(time);
}

void VideoState::onPlaybackStateChanged(const MediaCenter::PlaybackState &state)
{
    updateInfoDisplay();

    if (state == MediaCenter::PlayingState) {
        m_videoPlayPause->setIcon("media-playback-pause");
    } else {
        m_videoPlayPause->setIcon("media-playback-start");
    }
}

void VideoState::setMedia(const MediaCenter::Media &media)
{
    m_videoMedia = media;
}

void VideoState::setMediaObject(Phonon::MediaObject *object)
{
    m_videoObject = object;
    receivedMediaObject();
}

Phonon::MediaObject* VideoState::mediaObject() const
{
    return m_videoObject;
}

void VideoState::receivedMediaObject() const
{
    if (!mediaObject()) {
        kDebug() << "Media object error in VideoState";
        return;
    }
    m_videoSeekSlider->setRange(0, mediaObject()->totalTime());
    connect (mediaObject(), SIGNAL(totalTimeChanged(const qint64)), this, SLOT(updateTotalTime(const qint64)));
    connect (mediaObject(), SIGNAL(tick(const qint64)), this, SLOT(updateCurrentTick(const qint64)));

    if (m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState) {
        m_videoPlayPause->setIcon("media-playback-pause");
    }
}

void VideoState::selectedMediasChanged(const QList<MediaCenter::Media > &list)
{
    Q_UNUSED(list)

    updateInfoDisplay();
}

void VideoState::updateInfoDisplay()
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    //Update rating
    if (list.size() > 1) {
        QString number = QString::number(list.size());
        m_selectedMediasLabel->setText("Rating of " + number + " selected items");
        m_ratingWidget->setEnabled(true);
    }
    if (list.size() == 1) {
        m_selectedMediasLabel->setText("Rating of selected item");
        m_ratingWidget->setEnabled(true);
        m_resource = new Nepomuk::Resource(list[0].second);
        m_ratingWidget->setRating(m_resource->rating());
    }
    if (list.isEmpty()) {
        if (m_player->videoPlayerPlaybackState() == MediaCenter::PausedState ||
            m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState) {
            m_selectedMediasLabel->setText("Rating of active item");
            m_ratingWidget->setEnabled(true);
            m_resource = new Nepomuk::Resource(m_videoMedia.second);
            m_ratingWidget->setRating(m_resource->rating());
        }
        if (m_player->videoPlayerPlaybackState() == MediaCenter::StoppedState) {
            m_selectedMediasLabel->setText("");
            m_ratingWidget->setRating(0);
            m_ratingWidget->setEnabled(false);
        }
    }

    //Update title label
    QString file = m_videoMedia.second;
    //TODO: check if we really have a file
    QString label = QFileInfo(file).fileName();

    if (m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState) {
        m_currentlyPlayingLabel->setText(label);
    }
    if (m_player->videoPlayerPlaybackState() == MediaCenter::PausedState) {
        m_currentlyPlayingLabel->setText("(Paused) " + label);
    }
    if (m_player->videoPlayerPlaybackState() == MediaCenter::StoppedState) {
        m_currentlyPlayingLabel->setText("No video playing");
    }
}

void VideoState::slotRatingChanged(const int rating)
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    if (list.size() > 1) {
        foreach (MediaCenter::Media media, list) {
            m_resource = new Nepomuk::Resource(media.second);
            m_resource->setRating(rating);
        }
    }
    if (list.size() == 1) {
        m_resource = new Nepomuk::Resource(list[0].second);
        m_resource->setRating(rating);
    }
    if (list.isEmpty()) {
        if (m_player->videoPlayerPlaybackState() == MediaCenter::PausedState ||
            m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState) {
            m_resource = new Nepomuk::Resource(m_videoMedia.second);
            m_resource->setRating(rating);
        }
    }
}

void VideoState::enterBrowsingState() const
{
    m_player->stopVideo();
    m_layout->showBrowser();
    m_layout->controlAutohideOff();
    m_layout->showPlaylist();
}

void VideoState::enterPlayingState() const
{
    if (!SharedLayoutComponentsManager::self()->isBackgroundVideoMode()) {
        m_player->playAllVideoMedia();
    }

    //This function is called every time the play/pause button is clicked
    //We only want to turn autohide on, and hide buttons when the user enters fullscreen player,
    //not when he only pauses or plays when previously paused
    if (m_player->videoPlayerPlaybackState() == MediaCenter::PlayingState ||
        m_player->videoPlayerPlaybackState() == MediaCenter::StoppedState) {
        m_layout->controlAutohideOn();
        m_layout->hidePlaylist();
        m_layout->showPlayer();
    }
    m_layout->layoutPlayer();
}
