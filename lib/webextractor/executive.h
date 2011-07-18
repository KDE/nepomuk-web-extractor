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

#ifndef _NEPOMUK_WEBEXTRCT_EXECUTIVE_H_
#define _NEPOMUK_WEBEXTRCT_EXECUTIVE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <Nepomuk/Resource>
#include <Soprano/Statement>
#include <decision/decisionfactory.h>
#include <decision/decisionauthor.h>

#include "executivereply.h"
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class ExecutiveWrapper;
        /*! \brief Main object that perform actions. MUST be reentrant and thread safe
         * This is the class that responsible for extractin information. 
         * This class supply name and version, and provide method requestDecisions() 
         * that return a pointer to ExecutiveReply. 
         */
        class WEBEXTRACTOR_EXPORT Executive : public QObject, public Decision::DecisionAuthor
        {
                Q_OBJECT;
            public:
                /*! \brief Make some tests and call virtual method requestDecisions
                 * This method will first test given resources with filters set up by
                 * subclass. If filters failed, then 0 will be return imidiatelly.
                 * Otherwise requestDecisions will be called and it's answer returned
                 */
                ExecutiveReply * decisions(const Decision::DecisionFactory * factory, const Nepomuk::Resource & res);
                //virtual ExecutiveReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res, QObject * target, const char * finishedSlot, const char * errorSlot) = 0;
                virtual ~Executive();
                Executive(
                    int dataPPVersion
                );
                /*! \brief Return version of this Executive.
                 * The version of Executive is actualy the version of the plugin, that create this Executive
                 */
                int  version() const;
                /*! \brief Return name of this Executive
                 * Name of the executive is set by application using it
                 */
                QString name() const;
                // Force uncopyable
                // No implementation
                Executive(const Executive &);
                const Executive& operator=(const Executive&);


                enum FilterClassType { ExactMatch = 0, SubclassMatch = 1, SubclassOrExactMatch = 2, Match_MAX = 2 };
            protected:
                virtual ExecutiveReply * requestDecisions(const Decision::DecisionFactory * factory, const Nepomuk::Resource & res) = 0;

                /* ====== Filtering section ======= */
                /*! \brief Return true if this executive can extract information for given resource
                 * First, assigned filters are used. Then user-supplied filter is executed
                 */
                bool canProcess(const Nepomuk::Resource & res) const;
                void addTypeFilter( const QUrl & typeUrl, FilterClassType matchType, bool allowed );
                /*! \brief Default rule
                 * If no previous filter works, then resource will be allowed/rejected 
                 * depenging of the default filter
                 */
                void setDefaultFilter(bool allowed);


                friend class ExecutiveWrapper;
            private:
                void setName(const QString &);
                class Private;
                Private * d;
        };
    }
}
#endif

