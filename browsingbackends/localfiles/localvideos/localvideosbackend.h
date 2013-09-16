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

#ifndef LOCALVIDEOSBACKEND_H
#define LOCALVIDEOSBACKEND_H

#include "../localfilesabstractbackend.h"

class LocalVideosModel;

/**
 * @class LocalVideosBackend @author Alessandro Diaferia
 * @brief A useful backend class for the showing the pictures in Local Indexed files
 *
 * @description This class supports and handles LocalVideosModel.
 * 
 * This class direclty inherit from LocalFilesAbstractBackend
 */
class LocalVideosBackend : public LocalFilesAbstractBackend
{
    Q_OBJECT
public:
  
   /**
     * Constructor
     * Used to load backend instance
     * @param parent is a QObject pointer
     * @param args is of QVariantList type
     */
    LocalVideosBackend(QObject *parent, const QVariantList &args);
    
     /**
     * Destructor
     */
    ~LocalVideosBackend();

protected:
  
   /**
     * Initialized the LocalVideosModel
     */ 
    virtual void initModel();

private:
    LocalVideosModel *m_model;
};

#endif // LOCALVIDEOSBACKEND_H
