/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NEPOMUK_WEBEXTRCT_DATA_PP_PROXY_H_
#define _NEPOMUK_WEBEXTRCT_DATA_PP_PROXY_H_

#include <QtCore/QSharedDataPointer>
#include <QtCore/QHash>

#include "webextractor_export.h"
namespace Nepomuk
{
    class Resource;

    namespace WebExtractor
    {
        class DataPP;
        class DataPPReply;
        class DecisionFactory;
        /*! \brief This class bounds settings ( rank, scale coff ) of the DataPP with DataPP
         */
        class WEBEXTRACTOR_EXPORT DataPPWrapper
        {
            public:
                DataPPWrapper(DataPP*, const QString & , double, double);
                DataPPWrapper(const DataPPWrapper &);
                DataPPWrapper & operator=(const DataPPWrapper &);
                ~DataPPWrapper();
                DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res)const;
                DataPP * data() const;
                double rank() const;
                double coff() const;
                void setRank(double val);
                QString  name() const;
                int  version() const;
            private:
                class Private;
                QSharedDataPointer<Private> d;
        };
        typedef QHash< const DataPP*, DataPPWrapper* > DataPPKeeper;
    }
}

#endif
