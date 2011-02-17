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

#ifndef _NEPOMUK_WEBEXTRCT_EXECUTIVE_PROXY_H_
#define _NEPOMUK_WEBEXTRCT_EXECUTIVE_PROXY_H_

#include <QtCore/QSharedDataPointer>
#include <QtCore/QHash>

#include "webextractor_export.h"
namespace Nepomuk
{
    class Resource;

    namespace WebExtractor
    {
        class Executive;
        class ExecutiveReply;
        class DecisionFactory;
        /*! \brief This class bounds settings ( rank, scale coff ) of the Executive with Executive
         */
        class WEBEXTRACTOR_EXPORT ExecutiveWrapper
        {
            public:
                ExecutiveWrapper(Executive*, const QString & , double, double);
                ExecutiveWrapper(const ExecutiveWrapper &);
                ExecutiveWrapper & operator=(const ExecutiveWrapper &);
                ~ExecutiveWrapper();
                ExecutiveReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res)const;
                Executive * data() const;
                double rank() const;
                double coff() const;
                void setRank(double val);
                QString  name() const;
                int  version() const;
            private:
                class Private;
                QSharedDataPointer<Private> d;
        };
        typedef QHash< const Executive*, ExecutiveWrapper* > ExecutiveKeeper;
    }
}

#endif
