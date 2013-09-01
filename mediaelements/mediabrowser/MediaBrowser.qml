/*
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
 *   Copyright 2010 Lukas Appelhans <l.appelhans@gmx.de>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.mediacenter.elements 0.1 as MediaCenterElements
import org.kde.qtextracomponents 0.1 as QtExtra
import org.kde.plasma.mediacenter.components 0.1 as MediaCenterComponents

FocusScope {
    id: mediaBrowser
    property QtObject currentBrowsingBackend
    property QtObject previousBrowsingBackend

    signal backRequested
    signal playRequested(int index, string url, string currentMediaType)
    signal popupMenuRequested(int index, string mediaUrl, string mediaType, string display)

    Item {
        id: mediaBrowserSidePanel
        property QtObject child
        property bool enabled

        enabled: currentBrowsingBackend ? (currentBrowsingBackend.mediaBrowserSidePanel == "" ? false : true ) : false
        anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
        width: enabled ? parent.width*0.2 : 0
        clip: true

        Behavior on width {
            NumberAnimation {
                duration: 500
            }
        }
    }

    QtExtra.MouseEventListener {
        id: mediaBrowserViewItem
        property QtObject mediaBrowserGridView

        anchors {
            top: parent.top; right: parent.right; bottom: parent.bottom;
            left: mediaBrowserSidePanel.right
        }

        onWheelMoved: {
            if (wheel.delta < 0) {
                if(mediaBrowserGridView.moveCurrentIndexRight) mediaBrowserGridView.moveCurrentIndexRight();
            } else {
                if(mediaBrowserGridView.moveCurrentIndexLeft) mediaBrowserGridView.moveCurrentIndexLeft();
            }
        }
    }

    Component {
        id: mediaBrowserGridBrowserComponent
        MediaCenterComponents.GridBrowser {
            anchors { fill: parent; topMargin: 10; bottomMargin: 10 + bottomPanel.height }
            model: mediaBrowser.currentBrowsingBackend.models[0]
            currentBrowsingBackend: mediaBrowser.currentBrowsingBackend
            bottomSibling: searchMedia

            onMediaSelected: mediaBrowser.playRequested(index, url, mediaType)
            onPopupRequested: mediaBrowser.popupMenuRequested(index, url, mediaType, title)
        }
    }

    Component {
        id: mediaBrowserTabBrowserComponent
        MediaCenterComponents.TabBrowser {
            anchors { fill: parent; topMargin: 10; bottomMargin: 10 + bottomPanel.height }
            backend: mediaBrowser.currentBrowsingBackend

            onMediaSelected: mediaBrowser.playRequested(index, url, mediaType)
            onPopupRequested: mediaBrowser.popupMenuRequested(index, url, mediaType, title)
        }
    }

    onCurrentBrowsingBackendChanged: {
        if (!currentBrowsingBackend)
            return;

        errorLabel.text = "";

        if (mediaBrowserViewItem.mediaBrowserGridView) {
            mediaBrowserViewItem.mediaBrowserGridView.destroy();
        }
        //Check if there is a custom browser, if yes, load that
        var object;
        if (currentBrowsingBackend.mediaBrowserOverride()) {
            var qmlSource = currentBrowsingBackend.mediaBrowserOverride();
            object = Qt.createQmlObject(qmlSource, mediaBrowserViewItem);
            mediaBrowserViewItem.mediaBrowserGridView = object;
            object.backend = (function() { return currentBrowsingBackend; });
        } else {
            if (mediaBrowser.currentBrowsingBackend.models.length > 1 ) {
                object = mediaBrowserTabBrowserComponent.createObject(mediaBrowserViewItem);
            } else {
                object = mediaBrowserGridBrowserComponent.createObject(mediaBrowserViewItem);
            }
        }
        mediaBrowserViewItem.mediaBrowserGridView = object;
        object.focus = true

        if (currentBrowsingBackend.supportsSearch()) {
            searchMedia.visible = true
        } else {
            searchMedia.visible = false
        }

        if (mediaBrowserSidePanel.child) mediaBrowserSidePanel.child.destroy()
        //Load the panel if the backend supports one
        if (currentBrowsingBackend.mediaBrowserSidePanel) {
            var qmlSource = currentBrowsingBackend.mediaBrowserSidePanel;
            object = Qt.createQmlObject(qmlSource, mediaBrowserSidePanel);
            mediaBrowserSidePanel.child = object
            object.backend = (function() { return currentBrowsingBackend; });
        }

        if (currentBrowsingBackend) {
            currentBrowsingBackend.error.connect(errorLabel.setError);
        }

        searchMedia.text = currentBrowsingBackend.searchTerm;
    }

    function destroyGridView()
    {
        mediaBrowserViewItem.mediaBrowserGridView.destroy()
    }
    function loadModel()
    {
        //JS snippet to do mediaBrowserGridView.model: currentBrowsingBackend.backendModel
        if (mediaBrowserViewItem && mediaBrowserViewItem.mediaBrowserGridView)
            mediaBrowserViewItem.mediaBrowserGridView.model = (function() { return currentBrowsingBackend.models[0]; });
    }

    function hideMediaBrowserSidePanel()
    {
        currentBrowsingBackend.mediaBrowserSidePanel = ""
        mediaBrowserViewItem.mediaBrowserGridView.focus = true;
    }

    Item {
        id: bottomPanel
        width: parent.width
        height: 30
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
            margins: 10
        }

        PlasmaComponents.TextField {
            id: searchMedia
            clearButtonShown: true
            anchors {
                left: parent.left
                bottom: parent.bottom
                top: parent.top
                right: mediaCountLabel.left
            }

            opacity: activeFocus ? 1 : 0.8
            placeholderText: "Search..."
            onTextChanged: searchMediaTimer.restart()
            Keys.onUpPressed: mediaBrowserViewItem.mediaBrowserGridView.focus = true

            Timer {
                id: searchMediaTimer
                interval: 500
                onTriggered: if (currentBrowsingBackend) currentBrowsingBackend.searchTerm = searchMedia.text
            }
        }

        PlasmaComponents.Label {
            id: mediaCountLabel
            text: mediaBrowserViewItem.mediaBrowserGridView ? i18np("%1 item", "%1 items", mediaBrowserViewItem.mediaBrowserGridView.count) : ""
            visible: mediaBrowserViewItem.mediaBrowserGridView ? (mediaBrowserViewItem.mediaBrowserGridView.count != undefined) : false

            anchors {
                bottom: parent.bottom
                top: parent.top
                right: parent.right
                margins: 10
            }
        }
    }

     onPopupMenuRequested: {
        popupMenu.visible = true
        popupMenu.mediaUrl = mediaUrl
        popupMenu.display = display
        popupMenu.mediaType = mediaType
        popupMenu.currentMediaDelegateIndex = index
     }

     MediaCenterElements.PopupMenu {
         id: popupMenu

         property string mediaUrl
         property string display
         property string mediaType
         property int currentMediaDelegateIndex

         anchors.fill: parent
         model: PopupModel {}
         onPopupMenuItemClicked: {
             switch(index) {
                 case 0:
                     playlistModel.addToPlaylist(mediaUrl);
                     break;
                 case 1:
                      mediaBrowser.playRequested(currentMediaDelegateIndex, mediaUrl, mediaType)
                     break;
                 case 2:
                     break;
             }
             popupMenu.visible = false
         }
    }

    PlasmaComponents.Label {
        id: errorLabel
        anchors.centerIn: parent
        font.pointSize: 18
        z: 2

        function setError(message)
        {
            errorLabel.text = message;
        }
    }

    function goBack()
    {
        return mediaBrowser.currentBrowsingBackend.goOneLevelUp();
    }

    Keys.onPressed: {
        if (event.text && searchMedia.visible) {
            searchMedia.focus = true
            searchMedia.text = event.text
        }
    }

}
