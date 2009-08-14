/***************************************************************************
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>                    *
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

#include "mediaplayer.h"
#include "pictureviewer.h"
//#include "plasmamediaplayeradaptor.h"
//#include "dbus/playerdbushandler.h"
//#include "dbus/rootdbushandler.h"
//#include "dbus/tracklistdbushandler.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QDBusConnection>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

#include <KMimeType>
#include <KFileDialog>
#include <KConfigDialog>

#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>

#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/slider.h>
#include <plasma/widgets/videowidget.h>
#include <Plasma/Containment>

MediaPlayer::MediaPlayer(QObject *parent, const QVariantList &args)
    : MediaCenter::Player(parent, args),
      m_ticking(false),
      m_raised(false),
      m_fullScreen(false),
      m_active(false),
      m_fullScreenVideo(0),
      m_pviewer(new PictureViewer(this))
{
    m_pviewer->hide();
    setAcceptDrops(true);
    setHasConfigurationInterface(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    resize(200, 200);

    if (args.count()) {
        m_currentUrl = args.value(0).toString();
    }
//    QDBusConnection dbus = QDBusConnection::sessionBus();
//    dbus.registerService("org.mpris.PlasmaMediaPlayer");
//    new PlasmaMediaPlayerAdaptor(this);
//
//    dbus.registerObject("/PlasmaMediaPlayer", this);
}


MediaPlayer::~MediaPlayer()
{
    delete m_video;
}

void MediaPlayer::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget(parent);
    ui.setupUi(widget);

    parent->addPage(widget, i18n("Player settings"), "multimedia-player");

    ui.fullScreenCheckBox->setChecked(m_fullScreen);
    connect (parent, SIGNAL(accepted()), this, SLOT(acceptConfiguration()));
}

void MediaPlayer::acceptConfiguration()
{
    if (m_fullScreen == ui.fullScreenCheckBox->isChecked()) {
        return;
    }
    m_fullScreen = ui.fullScreenCheckBox->isChecked();
    applyConfig();
}

void MediaPlayer::applyConfig()
{
    doFullScreen();
}

void MediaPlayer::doFullScreen()
{
    if (m_fullScreen) {
        m_fullScreenVideo = m_video->nativeWidget();
        m_fullScreenVideo->setParent(0);
        m_video->setWidget(0);
        m_fullScreenVideo->installEventFilter(this);
        QDesktopWidget *desktop = qApp->desktop();
        m_fullScreenVideo->move(desktop->screenGeometry(containment()->screen()).topLeft());
        m_fullScreenVideo->enterFullScreen();
    } else {
        m_fullScreenVideo->exitFullScreen();
        m_fullScreenVideo->removeEventFilter(this);
        m_video->setWidget(m_fullScreenVideo);
    }


}

void MediaPlayer::init()
{
   m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
   m_layout->setContentsMargins(0, 0, 0, 0);

   m_video = new Plasma::VideoWidget(this);
   m_video->setAcceptDrops(false);

   m_layout->addItem(m_video);

//   connect(m_video->audioOutput(), SIGNAL(volumeChanged(qreal)), SLOT(volumeChanged(qreal)));


//   m_video->setUrl(m_currentUrl);
   Phonon::MediaObject *media = m_video->mediaObject();

//   connect(media, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
//   connect(media, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChanged(bool)));

   media->setTickInterval(200);

//   connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
//   connect(media, SIGNAL(totalTimeChanged(qint64)), SLOT(totalTimeChanged(qint64)));

//   media->play();

//   m_video->setUsedControls(Plasma::VideoWidget::DefaultControls);

//   m_hideTimer = new QTimer(this);
//   m_hideTimer->setSingleShot(true);
//   connect(m_hideTimer, SIGNAL(timeout()), this, SLOT(hideControls()));

   SetControlsVisible(false);

   connect (m_video->mediaObject(), SIGNAL(finished()), this, SLOT(playNextMedia()));
   connect (m_pviewer, SIGNAL(showFinished()), this, SLOT(playNextMedia()));

   m_pviewer->setShowTime(slideShowInterval());
   connect (this, SIGNAL(slideShowTimeChanged(qint64)), m_pviewer, SLOT(setShowTime(qint64)));
//   new PlayerDBusHandler(this, media, m_video->audioOutput());
//   new TrackListDBusHandler(this, media);
//   new RootDBusHandler(this);
}

void MediaPlayer::playNextMedia()
{
    int nextMedia = m_medias.indexOf(m_currentMedia) + 1;
    if (nextMedia >= m_medias.count()) {
        m_active = false;
        return;
    }

    playMedia(m_medias[nextMedia]);

}

void MediaPlayer::constraintsEvent(Plasma::Constraints constraints)
{
    setBackgroundHints(NoBackground);
}

void MediaPlayer::SetControlsVisible(bool visible)
{
    m_video->setControlsVisible(visible);
}

bool MediaPlayer::ControlsVisible() const
{
    return m_video->controlsVisible();
}

void MediaPlayer::ToggleControlsVisibility()
{
    SetControlsVisible(!m_video->controlsVisible());
}

void MediaPlayer::playPause()
{
    Phonon::MediaObject *media = m_video->mediaObject();

    if (media->state() == Phonon::PlayingState) {
        media->pause();
    } else {
        m_active = true;
        media->play();
    }
}

void MediaPlayer::RaiseLower()
{
    if (m_raised) {
        Lower();
        m_raised = false;
    } else {
        Raise();
        m_raised = true;
    }
}

void MediaPlayer::Raise()
{
    Raise();
    m_raised = true;
}

void MediaPlayer::Lower()
{
    Lower();
    m_raised = true;
}

void MediaPlayer::seek(int progress)
{
    if (!m_ticking) {
        m_video->mediaObject()->seek(progress);
    }
}

void MediaPlayer::setVolume(qreal value)
{
     m_video->audioOutput()->setVolume(value);
}

void MediaPlayer::dropEvent(QGraphicsSceneDragDropEvent *event)
{

    if (event->mimeData()->urls().isEmpty()) {
        return;
    }

    QList<MediaCenter::Media> medias;
    foreach (const KUrl &url, event->mimeData()->urls()) {
        if (!QFile::exists(url.path())) {
            continue;
        }

        MediaCenter::MediaType type = MediaCenter::getType(url.path());
        if (type != MediaCenter::Invalid) {
            MediaCenter::Media media;
            media.first = type;
            media.second = url.path();
            medias << media;
        }
    }

    emit mediaReceived(medias);
}

void MediaPlayer::ShowOpenFileDialog()
{
    OpenUrl(KFileDialog::getOpenFileName());
}

void MediaPlayer::OpenUrl(const QString &url)
{
    m_currentUrl = url;
    m_video->setUrl(m_currentUrl);
    m_video->mediaObject()->play();
}

void MediaPlayer::hideControls()
{
    SetControlsVisible(false);
}

void MediaPlayer::keyPressEvent(QKeyEvent *event)
{
    Phonon::MediaObject *media = m_video->mediaObject();
    Phonon::AudioOutput *audio = m_video->audioOutput();
    const qreal step = 30;

    switch (event->key())
    {
    case Qt::Key_Left:
        media->seek(media->currentTime() - media->totalTime()/step);
        break;
    case Qt::Key_Right:
        media->seek(media->currentTime() + media->totalTime()/step);
        break;
    case Qt::Key_Space:
        playPause();
        break;
    case Qt::Key_Up:
        audio->setVolume(qMin(qreal(1.0), audio->volume() + 0.1));
        break;
    case Qt::Key_Down:
        audio->setVolume(qMax(qreal(0.0), audio->volume() - 0.1));
        break;
    case Qt::Key_R:
        RaiseLower();
        break;
    case Qt::Key_V:
        ToggleControlsVisibility();
        break;
    default:
        break;
    }

    if (event->key() != Qt::Key_V) {
        SetControlsVisible(true);
        m_hideTimer->start(2000);
    }
}

bool MediaPlayer::eventFilter(QObject *o, QEvent *e)
{
    if (o != m_fullScreenVideo) {
        return false;
    }

    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_Escape) {
            m_fullScreen = false;
            doFullScreen();
            return true;
        }
    }

    return false;
}

void MediaPlayer::stop()
{
    m_video->mediaObject()->stop();
    m_pviewer->stop();
    m_active = false;
}

Phonon::MediaObject* MediaPlayer::mediaObject()
{
    if (m_video) {
        return m_video->mediaObject();
    }
    return 0;
}

void MediaPlayer::skipBackward()
{
    int previous = m_medias.indexOf(m_currentMedia) - 1;
    if (previous < 0) {
        return;
    }
    m_active = true;
    playMedia(m_medias[previous]);
}

void MediaPlayer::skipForward()
{
    playNextMedia();
}

void MediaPlayer::playMedia(const MediaCenter::Media &media)
{
    kDebug() << "trying to play" << media.second;
    foreach (MediaCenter::Media mediaSource, m_medias)  {
        if (mediaSource.second == media.second) {
            if (media.first == MediaCenter::Audio ||
                media.first == MediaCenter::Video ||
                media.first == MediaCenter::OpticalDisc) {
                // check whether there is the right widget in the layout
                if (!m_video->isVisible()) {
                    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
                    layout->removeItem(m_pviewer);
                    m_video->show();
                    m_pviewer->hide();
                    layout->addItem(m_video);
                    setLayout(layout);
                }

                m_video->mediaObject()->setCurrentSource(media.second);
                if (m_video->mediaObject()->state() != Phonon::PlayingState) {
                    kDebug() << "playpaused";
                    playPause();
                }
                m_currentMedia = media;
            } else {
                slideShow(media);
            }
        }
    }
}

void MediaPlayer::slideShow(const MediaCenter::Media &media)
{
    if (!m_pviewer->isVisible()) {
        QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
        layout->removeItem(m_video);
        m_video->hide();
        m_pviewer->show();
        layout->addItem(m_pviewer);
        setLayout(layout);
    }

    if (m_video->mediaObject()->state() == Phonon::PlayingState) {
        stop();
    }

    m_pviewer->loadPicture(media.second);
    m_currentMedia = media;
}

void MediaPlayer::append(const QStringList &medias)
{
    foreach (const QString mediaString, medias) {
        MediaCenter::Media media;
        media.first = MediaCenter::getType(mediaString);
        media.second = mediaString;

        kDebug() << "appending" << mediaString;
        m_medias << media;
    }
}

void MediaPlayer::enqueue(const QList<MediaCenter::Media> &medias)
{
    m_medias << medias;
}

MediaCenter::Media MediaPlayer::currentMedia()
{
    return m_currentMedia;
}

bool MediaPlayer::isActive()
{
    return m_active;
}

K_EXPORT_PLASMA_APPLET(mcplayer, MediaPlayer)

#include "mediaplayer.moc"
