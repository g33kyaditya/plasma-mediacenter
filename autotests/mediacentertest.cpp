/***********************************************************************************
 *   Copyright 2014 by Sinny Kumari <ksinny@gmail.com>                             *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "mediacentertest.h"
#include <mediacenter/mediacenter.h>

#include <qtest.h>
#include <QStandardPaths>

QTEST_GUILESS_MAIN(MediaCenterTest);

void MediaCenterTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void MediaCenterTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void MediaCenterTest::init()
{
    // Called before each testfunction is executed
}

void MediaCenterTest::cleanup()
{
    // Called after every testfunction
}

//TODO: remove or enable again
void MediaCenterTest::shouldReturnPathForComponent()
{
//    const QString path = MediaCenter::dataDirForComponent("test");

//    const QString expectedPath = QString("%1%2/%3")
//        .arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation))
//        .arg(KCmdLineArgs::appName())
//        .arg("test");

//    QCOMPARE(path, expectedPath);

}

//TODO: remove or enable again
void MediaCenterTest::shouldReturnPathWithoutComponent()
{
//    const QString path = MediaCenter::dataDirForComponent();

//    const QString expectedPath = QString("%1%2")
//    .arg(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation))
//    .arg(KCmdLineArgs::appName());

//    QCOMPARE(path, expectedPath);
}
