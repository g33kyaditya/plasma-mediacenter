/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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

#include "mainwindow.h"

#include <libs/mediacenter/backendsmodel.h>

#include <Plasma/Package>

#include <KDE/KCmdLineArgs>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QtOpenGL/QGLWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent)
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->isSet("fullscreen")) {
        toggleFullScreen();
    }
    args->clear();

    QDeclarativeView *view = new QDeclarativeView(this);

    QGLWidget *glWidget = new QGLWidget;
    glWidget->setAutoFillBackground(false);
    view->setViewport(glWidget);
      
    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setCentralWidget(view);

    m_kdeclarative.setDeclarativeEngine(view->engine());
    m_kdeclarative.initialize();
    m_kdeclarative.setupBindings();

    BackendsModel *backendsModel = new BackendsModel(view->engine(), this);
    view->rootContext()->setContextProperty("backendsModel", backendsModel);

    m_structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", m_structure);
    view->setSource(QUrl(package->filePath("mainscript")));
}

void MainWindow::toggleFullScreen()
{
    if (windowState() & Qt::WindowFullScreen) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
    }
}

MainWindow::~MainWindow()
{
    
}