/***************************************************************************
 *   Copyright 2011 Shantanu Tushar <shantanu@kde.org>                     *
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

#ifndef THUMBNAILPROVIDER_H
#define THUMBNAILPROVIDER_H

#include "mediacenter_export.h"
#include <QtQuick/QQuickImageProvider>

#include <QUrl>

namespace KIO
{
class PreviewJob;
};

/**
 *Forward Declarations of the classes
 */
class KFileItemList;
class KFileItem;

/**
 * @class ThumbnailProvider @author Shantanu Tushar
 * @brief A useful class to provide for thumbnails of the content images/videos in a folder view
 * 
 * This class inherit from QQuickImageProvider
 */
class MEDIACENTER_EXPORT ThumbnailProvider : public QObject, public QQuickImageProvider
{

    Q_OBJECT
public:
  
   /**
     * Constructor
     * @brief Load a ThumbnailProvider instance
     * @param parent is a QObject pointer
     */
    explicit ThumbnailProvider(QObject* parent = 0);
    
   /**
    * Destructor
    * @brief Deletes the ThumbnailProvider instance
    */ 
    virtual ~ThumbnailProvider();

    virtual QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);

    /**
     * Catches a preview (thumbnail) for files using KIO:: PreviewJob
     * @param fileList which is a KUrl List type
     * @param size is fileList size
     */
    void loadThumbnails(const QList<QUrl>& fileList, const QSize& size);
    
    /**
     * Calls loadThumbnails function with appropriate file url and size
     * @param file a QUrl of the file for which thumbnail has there.
     * @param size is the list size
     */
    void loadThumbnail(const QUrl& file, const QSize& size);
    
    /**
     * It uses QPixmapCatche::Key which can be used for efficient access to the QPixmapCache
     */ 
    bool hasThumbnail(const QString& url);

signals:
    void gotThumbnail(const QString& url);

private slots:
    
    /**
     * It uses QPixmapCatche::insert to receive an instance of Key generated by the pixmap cache
     */ 
    void processPreview(const KFileItem& file, const QPixmap& thumbnail);

private:
    class Private;
    Private* const d;
};

#endif // THUMBNAILPROVIDER_H
