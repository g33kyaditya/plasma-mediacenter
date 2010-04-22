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

#ifndef MUSICSTATE_H
#define MUSICSTATE_H

#include "mediacenterstate.h"

#include <Plasma/IconWidget>

namespace MediaCenter {
    enum MusicSubComponent {
        MusicStateLabel
    };

class MEDIACENTER_EXPORT MusicState : public MediaCenterState
{
    Q_OBJECT
public:
    MusicState(QState *parent = 0);
    ~MusicState();

    QList<QGraphicsWidget*> subComponents();
    void configureUIComponents(QList<Plasma:: Applet*> list);
    void connectMediaLayout(MediaLayout *layout);
    void configureMediaLayout(MediaLayout *layout);
    void connectSubComponents(QList<Plasma:: Applet*> list);

protected:
    void onExit(QEvent* event);
    void onEntry(QEvent* event);

private:
    Plasma::IconWidget *m_musicStateLabel;
    Plasma::IconWidget *m_musicTogglePlaylist;
};

} //namespace end
#endif // MUSICSTATE_H