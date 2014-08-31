/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
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

#include <QUrl>
#include <QQmlEngine>
#include <QQmlContext>

#include <QDebug>
#include <Plasma/Package>
#include <KLocalizedString>

#include "pmcview.h"

PmcView::PmcView(PmcCorona *pmccorona, QWindow *parent)
    : PlasmaQuick::View(pmccorona, parent)
{
    qDebug() << "Setting title of view";
    setTitle(i18n("Plasma Mediacenter"));
    qDebug() << "Setting view on corona";
    pmccorona->setView(this);
    qDebug() << "setting desktop of root context";
    engine()->rootContext()->setContextProperty("desktop", this);
    qDebug() << "Setting source to Desktop.qml";
    setSource(QUrl::fromLocalFile(pmccorona->package().filePath("views", "Desktop.qml")));
}

PmcView::~PmcView()
{
}

#include "pmcview.moc"
