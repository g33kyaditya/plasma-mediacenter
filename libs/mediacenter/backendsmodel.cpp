/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                               *
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

#include "backendsmodel.h"
#include "abstractbrowsingbackend.h"

#include <QDebug>

bool pluginLessThan(const KPluginInfo &lh, const KPluginInfo &rh)
{
    return lh.name().compare(rh.name(), Qt::CaseInsensitive) < 0;
}

class BackendsModel::Private
{
public:
    QHash<QString, MediaCenter::AbstractBrowsingBackend*> backends;
    KPluginInfo::List backendInfo;
    KPluginInfo::List loadedBackendsInfo;
    QWeakPointer<PmcRuntime> pmcRuntime;
};

BackendsModel::BackendsModel(QWeakPointer<PmcRuntime> pmcRuntime, QObject* parent)
    : QAbstractListModel(parent)
    , d(new Private)
{
    d->pmcRuntime = pmcRuntime;
    d->backendInfo = MediaCenter::AbstractBrowsingBackend::availableBackends();
    qStableSort(d->backendInfo.begin(), d->backendInfo.end(), pluginLessThan);

    QHash<int, QByteArray> roles = roleNames();
    roles[ModelObjectRole] = "modelObject";
    roles[BackendCategoryRole] = "backendCategory";
    setRoleNames(roles);

    loadBrowsingBackends();
}

void BackendsModel::loadBrowsingBackends()
{
    if (!d->backends.isEmpty())
        return;

    Q_FOREACH (KPluginInfo info, d->backendInfo) {

        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        const QVariantList args = QVariantList() << loader.metaData().toVariantMap();

        if(factory)
        {
            MediaCenter::AbstractBrowsingBackend *backend = factory->create<MediaCenter::AbstractBrowsingBackend>(0, args); 
            if (backend) {

                if (!backend->okToLoad()) {
                    qDebug() << "Backend " << info.name() << " doesn't want to be loaded";
                    continue;
                }
                backend->setName(info.pluginName());
                backend->setParent(const_cast<BackendsModel *>(this));
                backend->setPmcRuntime(d->pmcRuntime);
                d->loadedBackendsInfo.append(info);
                qDebug() << "Loaded backend : " << info.name() << " from : " << info.libraryPath();
            } else {
                qDebug() << "Could not create a instance for the backend " << info.name();
            }
        }

    }
}

QVariant BackendsModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    const KPluginInfo &info = d->loadedBackendsInfo.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return info.name();
        case Qt::DecorationRole:
            return info.icon();
        case Qt::ToolTipRole:
            return info.comment();
        case BackendCategoryRole:
            return info.category();
        case ModelObjectRole:
            QObject *backend = d->backends.value(info.service()->library());
            QVariant ptr;
            ptr.setValue(backend);
            return ptr;
    }

    return QVariant();
}

int BackendsModel::rowCount(const QModelIndex &) const
{
    return d->loadedBackendsInfo.count();
}
