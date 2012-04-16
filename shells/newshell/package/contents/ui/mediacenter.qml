/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

import QtQuick 1.1
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents

Rectangle {
    id: mediaCenterRootItem
    property int totalTimeHr
    property int totalTimeMin
    property int totalTimeSec
    property int currentTimeHr
    property int currentTimeMin
    property int currentTimeSec

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#000000" }
        GradientStop { position: 0.5; color: "#222222" }
        GradientStop { position: 1.0; color: "#000000" }
    }

    MediaCenterComponents.RuntimeData {
        id: runtimeData

        totalTime: mediaPlayer.totalTime

        onCurrentTimeChanged: {
            if (currentTimeDirty) {
                currentTimeDirty = false
                mediaPlayer.currentTime = currentTime
            }
        }
    }

    MediaCenterComponents.MediaPlayer {
        id: mediaPlayer
        anchors.fill: parent
        focus: !mediaBrowser.activeFocus && !mediaWelcome.activeFocus

        playing: runtimeData.playing
        paused: runtimeData.paused
        stopped: runtimeData.stopped
        volume: runtimeData.volume

        onClicked: mediaController.visible = mediaController.visible ? false : true
        onEscapePressed: mediaBrowser.visible = true
        onCurrentTimeChanged: {
            runtimeData.currentTime = currentTime
            currentTimeSec = currentTime /1000;
            currentTimeHr = Math.floor(currentTimeSec / 3600);
            currentTimeSec %= 3600;
            currentTimeMin = Math.floor(currentTimeSec / 60);
            currentTimeSec = Math.floor(currentTimeSec % 60);
            mediaController.curMediaTime = currentTimeHr + ":"  + currentTimeMin + ":" + currentTimeSec;
        }
        
        Keys.onPressed: {
            if(event.key == 16777344) { //Media Play key
                if(mediaPlayer.playing) {
                    mediaPlayer.playing = false;
                    mediaPlayer.paused = true;
                } else {
                    mediaPlayer.playing = true;
                    mediaPlayer.paused = false;
                }
            }
        }

        onTotalTimeChanged: {
             totalTimeSec = totalTime / 1000;
             totalTimeHr = Math.floor(totalTimeSec / 3600);
             totalTimeSec %= 3600;
             totalTimeMin = Math.floor(totalTimeSec / 60);
             totalTimeSec = Math.floor(totalTimeSec % 60);
             mediaController.totalMediaTime = totalTimeHr + ":" + totalTimeMin + ":" + totalTimeSec;
        }

        onMediaFinished: runtimeData.stopped = true
        onMediaStarted: runtimeData.playing = true
    }

    MediaCenterComponents.MediaController {
        id: mediaController
        height: parent.height * 0.08
        width: parent.width * 0.8

        anchors {
            horizontalCenter: parent.horizontalCenter; top: parent.top
        }

        runtimeDataObject: runtimeData

        onRequestToggleBrowser: mediaBrowser.visible = !mediaBrowser.visible
    }

    MediaCenterComponents.MediaWelcome {
        id: mediaWelcome
        width: parent.width
        focus: visible

        model: backendsModel
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom: parent.bottom
        }

        onBackendSelected: { runtimeData.currentBrowsingBackend = selectedBackend; visible = false }
        onVisibleChanged: mediaController.visible = !visible
    }

    MediaCenterComponents.MediaBrowser {
        id: mediaBrowser
        anchors {
            left: parent.left; right: parent.right; top: mediaController.bottom; bottom:parent.bottom
        }
        visible: false
        focus: visible

        z: 1

        currentBrowsingBackend: runtimeData.currentBrowsingBackend
        onCurrentBrowsingBackendChanged: visible = true
        onVisibleChanged: {
            if (visible) { loadModel(); focus = true }
        }

        Keys.onPressed: {
            if(event.key == 16777344) { //Media Play key
                if(mediaPlayer.playing) {
                    mediaPlayer.playing = false;
                    mediaPlayer.paused = true;
                } else {
                    mediaPlayer.playing = true;
                    mediaPlayer.paused = false;
                }
            }   
        }
        onPlayRequested: {
            mediaPlayer.visible = true
            runtimeData.playing = true
            mediaPlayer.url = url
            mediaPlayer.play()
            mediaBrowser.visible = false
            mediaPlayer.focus = true
        }

        onBackStoppedChanged: {
            if(backStopped) {
                runtimeData.currentBrowsingBackend = null
                visible = false
                mediaWelcome.visible = true
                backStopped = false
            }
        }
    }

    MediaCenterComponents.AboutPMC {
        id: aboutPmc
        listWidth: parent.width*0.8; listHeight: parent.height*0.8
        anchors.centerIn: parent
    }

    PlasmaComponents.ToolButton {
        anchors.right: parent.right; anchors.bottom: parent.bottom
        width: 64
        height: 64
        visible: mediaWelcome.visible

        iconSource: "plasma"
        onClicked: aboutPmc.open()
    }
}
