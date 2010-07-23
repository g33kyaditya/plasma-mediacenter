/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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
#include "qmlhomeview.h"
#include "viewitem.h"

#include <QAbstractItemModel>
#include <QFile>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/qdeclarative.h>

#include <QGraphicsLinearLayout>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <KDebug>

QML_DECLARE_TYPE(ViewItem)

QmlHomeView::QmlHomeView(QGraphicsItem *parent) :
    QGraphicsWidget(parent),
    m_engine(0),
    m_mainComponent(0),
    m_model(0),
    m_root(0)
{
    qmlRegisterType<ViewItem>("MediaBrowser", 0, 1, "ViewItem");
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(layout);
}

QmlHomeView::~QmlHomeView()
{}

void QmlHomeView::setModel(QAbstractItemModel *model)
{
    if (!m_engine) {
        return;
    }
    m_model = model;
    m_engine->rootContext()->setContextProperty("homeModel", m_model);
}

void QmlHomeView::setQmlPath(const QString &path)
{
    kDebug() << "loading qml" << path;
    if (!QFile::exists(path)) {
        kWarning() << "the given path does not exist";
        return;
    }

    delete m_engine;
    delete m_mainComponent;

    m_engine = new QDeclarativeEngine(this);
    m_engine->rootContext()->setContextProperty("homeModel", m_model);

    m_mainComponent = new QDeclarativeComponent(m_engine, path, this);
    connect(m_mainComponent, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(printError()));

    if (m_mainComponent->isReady() || m_mainComponent->isError()) {
        aquireRootComponent();
    } else {
        connect(m_mainComponent, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(aquireRootComponent()));
    }
}

void QmlHomeView::printError()
{
    foreach (const QDeclarativeError &error, m_mainComponent->errors()) {
        kDebug() << error.toString();
    }
}

void QmlHomeView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsWidget::paint(painter, option, widget);
}

void QmlHomeView::aquireRootComponent()
{
    if (m_mainComponent->status() == QDeclarativeComponent::Error) {
        return;
    }

    m_root = m_mainComponent->create();

    if (!m_root) {
        kWarning() << "cannot create root component: aborting";
        return;
    }

    QGraphicsObject *object = qobject_cast<QGraphicsObject*>(m_root);
    QGraphicsWidget *widget = qobject_cast<QGraphicsWidget*>(m_root);

    if (object) {
        object->setParentItem(this);
        if (scene()) {
            scene()->addItem(object);
        }
    }

    if (widget) {
        static_cast<QGraphicsLinearLayout*>(layout())->addItem(widget);
    }
}

void QmlHomeView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsWidget::resizeEvent(event);

    if (m_root) {
        m_root->setProperty("width", size().width());
        m_root->setProperty("height", size().height());
    }
}
