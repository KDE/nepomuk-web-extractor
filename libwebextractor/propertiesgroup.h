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

#ifndef __properties_group_h_
#define __properties_group_h_

#include <QtCore/QSharedDataPointer>
#include <QSharedPointer>
#include <QtCore/QSet>
#include <QUrl>
#include <QString>
#include <Nepomuk/Resource>
#include "webextractor_export.h"


namespace Soprano
{
    class Model;
    class Statement;
}

namespace Nepomuk
{
    class ResourceManager;

    namespace Sync
    {
        class ChangeLog;
        class ChangeLogFilterModel;
    }

    namespace WebExtractor
    {
        class Decision;
        class DecisionFactory;
        class DecisionData;
        /*! \brief PropertiesGroup is a unit of changes that can be applied to the resource
         * PropertiesGroup is actually a context in a separate model. The uri of the context
         * can be retrived with uri() method.
         * TODO Add error methods and ifrastructure
         */
        class WEBEXTRACTOR_EXPORT PropertiesGroup
        {
            public:
                PropertiesGroup(const PropertiesGroup &);
                ~PropertiesGroup();
                const PropertiesGroup & operator=(const PropertiesGroup &);
                /*! \brief add statements to
                 */
                //void addStatement(Soprano::Statement);

                //void addStatement(const Soprano::Node & predicate, const Soprano::Node & second, bool subject = true );

                /*! \brief Return rank of the PropertiesGroup
                 * See setRank for detalis
                 */
                double rank() const;

                //QSet< Soprano::Statement > data() const;

                /*! \brief This function set the rank of this PropertiesGroup
                 * The rank of the group is taken into account when automatically applying Decision.
                 * Greater rank will increase the chance that this PropertiesGroup will be applied.
                 * Rank must meet common requirements to rank. Incorrect rank will be automatically
                 * trucated
                 */
                void setRank(double newRank);

                /*! \brief Return proxy resource.
                 * The proxy resource is an alias to target resource. All changes that you want
                 * to make with target resource, you must instead do with this proxy resource.
                 * DO NOT MODIFICATE ORIGINAL RESOURCE!
                 */
                //Nepomuk::Resource mainProxyResource();
                //QUrl mainProxyResourceUrl();


                /*! Return ResourceManager over main Decisions model.
                 * All changes that will be done with this ResourceManager will
                 * be added to this PropertiesGroup
                 * \return Manager for the PropertiesGroup, 0 if PropertiesGroup is
                 * invalid or Decisions is already freezed.
                 */
                ResourceManager * manager() ;
                Soprano::Model * model() ;

                /*! \brief Make this PropertiesGroup current.
                 * All changes that will be done after that call and befor another
                 * PropertiesGroup will become current with decision's ResourceManager
                 * will be added to this PropertiesGroup
                 */
                void makeCurrent();
                /*! \brief This function will reset current group.
                 * This method will reset current group even if current group is not
                 * this one.
                 */
                void resetCurrent();

                QString description() const;
                void setDescription(const QString &);

                /*! \brief Return url of the group
                 * This method will return non-empty url only after any statements
                 * was added to the group.
                 * If some error occured and group failed to create, then
                 * empty url will be returned too.
                 */

                /*! \brief This function will return log of all changes in this PropertiesGroup
                 */
                Sync::ChangeLog log() const;
                //QUrl uri() const;
                /*! \brief Return true if this PropertiesGroup is valid
                 * The PropertiesGroup is valid if it was created within Decision.
                 * PropertiesGroup created with default constructor are invalid.
                 * This method will return true even if group failed to initialize
                 * properly
                 */
                bool isValid() const;

                /*! \brief Return proxy resource for given one
                 */
                Nepomuk::Resource proxyResource(const Nepomuk::Resource &);
                QUrl proxyUrl(const Nepomuk::Resource &);
                bool operator==(const PropertiesGroup & rhs)const;
                bool operator!=(const PropertiesGroup & rhs)const;
                friend PropertiesGroup & operator<<(PropertiesGroup &, const Soprano::Statement &);
                friend class Decision;
                friend class DecisionData;
                friend class DecisionFactory;
                friend unsigned int qHash(const PropertiesGroup &);
            protected:
                PropertiesGroup(DecisionData *);
                PropertiesGroup();
                class Private;
                //QSharedDataPointer<Private> d;
                QSharedPointer<Private> d;
            private:
                // Return pointer to internal log
                Nepomuk::Sync::ChangeLog * logPtr() const;
                // This method should be called from any method that edits data.
                // When it is called first time, it will create url of the PropertiesGroup
                // and register it in the decision
                //void inline registerGroup(const PropertiesGroup &);

                void initFilterModel();
                void initFilterManager();

                // This function is used for retrieving filter model
                // It can return 0 if model is not initialized
                Sync::ChangeLogFilterModel * filterModel() const;

                // This is service method. It set url of the PropertiesGroup. It doesn't
                // do any checks.
                //void setUrl( const QUrl & url);
        };
        PropertiesGroup & operator<<(PropertiesGroup &, const Soprano::Statement &);
    }
}

unsigned int qHash(const Nepomuk::WebExtractor::PropertiesGroup &);

#endif
