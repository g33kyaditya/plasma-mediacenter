/*
 *  Copyright 2012 Marco Martin <mart@kde.org>
 *  Copyright 2014 David Edmundson <davidedmundson@kde.org>
 *  Copyright 2014-2015 Bhushan Shah <bshah@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

import QtQuick 2.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.shell 2.0 as Shell
import "../explorer"


Item {
    id: root
    height: 768
    width: 1366

    property Item containment
    property Item wallpaper

    function toggleWidgetExplorer(containment) {
         console.log("Widget Explorer toggled");
    }

    onContainmentChanged: {
        //containment.parent = root;

        internal.newContainment = containment;

        if (containment != null) {
            containment.visible = true;
        }
        if (containment != null) {
            if (internal.oldContainment != null && internal.oldContainment != containment) {
                if (internal.newContainment != null) {
                    switchAnim.running = true;
                }
            } else {
                containment.anchors.fill = undefined;
                containment.anchors.left = root.left;
                containment.anchors.right = root.right;
                containment.anchors.bottom = root.bottom;
                containment.height = root.height / 3;
                internal.oldContainment = containment;
            }
        }
    }

    onWallpaperChanged: {
        wallpaper.parent = root;
        wallpaper.anchors.fill = root;
        if (!internal.oldWallpaper) {
            internal.oldWallpaper = wallpaper;
        }
    }

    //some properties that shouldn't be accessible from elsewhere
    QtObject {
        id: internal;

        property Item oldContainment: null;
        property Item newContainment: null;
        property Item oldWallpaper: null;
    }

    SequentialAnimation {
        id: switchAnim
        ScriptAction {
            script: {
                if (containment) {
                    containment.anchors.left = undefined;
                    containment.anchors.top = undefined;
                    containment.anchors.right = undefined;
                    containment.anchors.bottom = undefined;
                }
                internal.oldContainment.anchors.left = undefined;
                internal.oldContainment.anchors.top = undefined;
                internal.oldContainment.anchors.right = undefined;
                internal.oldContainment.anchors.bottom = undefined;

                if (containment) {
                    internal.oldContainment.z = 0;
                    internal.oldContainment.x = 0;
                    containment.z = 1;
                    containment.x = root.width;
                }
            }
        }
        ParallelAnimation {
            NumberAnimation {
                target: internal.oldContainment
                properties: "x"
                to: internal.newContainment != null ? -root.width : 0
                duration: 400
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: internal.newContainment
                properties: "x"
                to: 0
                duration: units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        ScriptAction {
            script: {
                if (containment) {
                    containment.anchors.left = root.left;
                    containment.anchors.top = root.top;
                    containment.anchors.right = root.right;
                    containment.anchors.bottom = root.bottom;
                    internal.oldContainment.visible = false;
                    internal.oldWallpaper.opacity = 1;
                    internal.oldContainment = containment;
                    internal.oldWallpaper = wallpaper;
                }
            }
        }
    }


    Component.onCompleted: {
        //configure the view behavior
        desktop.windowType = Shell.Desktop.Desktop;
    }
}