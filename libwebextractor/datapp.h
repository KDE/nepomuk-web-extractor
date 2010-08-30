/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NEPOMUK_WEBEXTRCT_DATA_PP_H_
#define _NEPOMUK_WEBEXTRCT_DATA_PP_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <Nepomuk/Resource>
#include <Soprano/Statement>
#include "datappreply.h"
#include "decisionfactory.h"
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class DataPPWrapper;
        /*! \brief MUST be reentrant and thread safe
         */
        class WEBEXTRACTOR_EXPORT DataPP : public QObject
        {
                Q_OBJECT;
            public:
                virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res) = 0;
                //virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res, QObject * target, const char * finishedSlot, const char * errorSlot) = 0;
                virtual ~DataPP();
                DataPP(
                    int dataPPVersion
                );
                /*! \brief Return version of this DataPP.
                 * The version of DataPP is actualy the version of the plugin, that create this DataPP
                 */
                int  version() const;
                /*! \brief Return name of this DataPP
                 */
                QString name() const;
                // Force uncopyable
                // No implementation
                DataPP(const DataPP &);
                const DataPP& operator=(const DataPP&);



                friend class DataPPWrapper;
            private:
                void setName(const QString &);
                class Private;
                Private * d;
        };
    }
}
#endif

