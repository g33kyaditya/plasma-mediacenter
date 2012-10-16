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

import QtQuick 1.1
import org.kde.plasma.mediacentercomponents 0.1 as MediaCenterComponents

Item {
    id: filterBackend
    property alias backendsModel: filteredModel.sourceBackendsModel
    property alias categoryFilter: filteredModel.backendCategory

    MediaCenterComponents.FilteredBackendsModel {
        id: filteredModel

        onBackendCategoryChanged: console.log(backendCategory)
    }

    ListView {
        id: listView
        anchors { fill: parent; margins: 20 }
        spacing: 20

        model: filteredModel
        delegate: BackendsListDelegate { width: listView.width; finalHeight: 64 }
        highlight: Rectangle {
            radius: 10
            color: "white"
            opacity: 0.5
        }
        highlightFollowsCurrentItem: true
    }
}