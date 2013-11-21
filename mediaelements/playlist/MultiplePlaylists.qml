/***************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                     *
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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements

FocusScope {
    Row {
        id: multiplePlaylistRow
        anchors.fill: parent

        ListView {
            id: multiplePlaylistList
            width: parent.width - createPlaylistTextField.width - createPlaylistButton.width - removePlaylistButton.width
            height: parent.height
            orientation: ListView.Horizontal
            spacing: 3
            clip: true
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 500
            highlightResizeDuration: 500
            focus: true
            highlight: PlasmaComponents.Highlight { }
            opacity: activeFocus ? 0.5 : 1

            model: MediaCenterElements.MultiplePlaylistModel {
                playlistModelAddress: playlistModel
            }

            delegate:
            Item {
                id: playlistText
                property string currentPlaylist: display
                height: parent.height
                width: 130
                anchors.verticalCenter: parent.verticalCenter
                Text {
                    id: itemText
                    anchors.fill: parent
                    maximumLineCount : 1
                    color: theme.textColor
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: display
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: playlistText.ListView.view.currentIndex = index
                }
            }

            onCurrentIndexChanged: {
                multiplePlaylistList.model.switchToPlaylist(
                    currentItem.currentPlaylist);
            }
        }

        PlasmaComponents.TextField {
            id: createPlaylistTextField

            visible: false
            anchors.verticalCenter: parent.verticalCenter
            height: 30
            width: visible ? 300 : 1
            clearButtonShown: true
            placeholderText: i18n("Create New Playlist")

            Keys.onEscapePressed: {
                text = "";
                createPlaylistButton.click();
            }
        }

        PlasmaComponents.ToolButton {
            id: createPlaylistButton
            iconSource: "list-add"
            height: parent.height
            width: height
            anchors.verticalCenter: parent.verticalCenter

            onClicked: click()
            function click() {
                if (!createPlaylistTextField.visible) {
                    createPlaylistTextField.visible = true
                } else {
                    if (createPlaylistTextField.text != "") {
                        multiplePlaylistList.model.createNewPlaylist (createPlaylistTextField.text)
                        createPlaylistTextField.text = ""
                    }
                    createPlaylistTextField.visible = false
                }
            }

            Keys.onRightPressed: removePlaylistButton.focus = true
        }

        PlasmaComponents.ToolButton {
            id: removePlaylistButton
            iconSource: "list-remove"
            height: parent.height
            width: height
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                multiplePlaylistList.model.removeCurrentPlaylist ()
            }

            Keys.onLeftPressed: createPlaylistButton.focus = true
        }
    }
}