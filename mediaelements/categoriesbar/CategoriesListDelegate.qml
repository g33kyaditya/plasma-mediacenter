/***************************************************************************
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
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

import QtQuick 2.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    id: root

    property QtObject backendObject: modelObject

    Row {
        anchors.fill: parent
        anchors.leftMargin: spacing
        anchors.rightMargin: spacing
        spacing: units.smallSpacing * 2

        PlasmaCore.IconItem {
            id: categoryIcon
            source: decoration
            height: parent.height
            width: height
        }

        PlasmaComponents.Label {
            height: parent.height
            width: parent.width - categoryIcon.width - parent.spacing
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
            elide: Text.ElideRight
            text: display
            font.pointSize: fontSizes.large
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.ListView.view.currentIndex = index;
            root.ListView.view.focus = true;
        }
    }

}
