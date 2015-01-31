/*
 * Copyright 2015 Bhushan Shah <bshah@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "dvbbackend.h"

#include "mediacenter/pmcmodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(DvbBackend, "dvbbackend.json")

DvbBackend::DvbBackend(QObject *parent, const QVariantList& args)
    : AbstractBrowsingBackend(parent, args)
{
}

DvbBackend::~DvbBackend()
{
}

bool DvbBackend::initImpl()
{
    // here there is two to three things to do
    // 1) init the model of dvb
    //      QAbstractItemModel *foobar = ...
    // 2) create PmcModel with that model as argument
    //      PmcModel *metadata = new PmcModel(foobar, this);
    // 3) set name of PmcModel
    //      metadata->setName(i18n("All Channels");
    // 4) set PmcModel using setModel()
    //      setModel(metadata);
    return true;
}

#include "dvbbackend.moc"
