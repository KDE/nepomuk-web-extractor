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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <Nepomuk/Resource>
#include <Soprano/Statement>
#include <Soprano/Model>
#include <QUrl>
#include <QSharedPointer>
#include "webextractor_export.h"
#include "propertiesgroup.h"
#include <nepomuk/changelog.h>
#include <nepomuk/identificationrequest.h>
//#include "soprano_statement_qhash.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class ResourceAnalyzer;
        class DecisionFactory;
        class DecisionList;
        class DataPP;
        class PropertiesGroup;
        class IdentificationSetManager;
        class DecisionApplicationRequest;

        class DecisionData;

        /*! \brief This class represent one Decision - set of statements that can be applied to resource
         * Decision consist of PropertiesGroup. PropertiesGroup is a set of statements
         * that can be applied only together. Decisions are implicitly shared objects
             * and changing copy of the Decision will change original too. Because of this,
             * this class is not thread-safe.
         */
        class WEBEXTRACTOR_EXPORT Decision /*: public QOb*/
        {
            public:
                /*! \brief Public constructor
                 * This constructor will create invalid Decision
                 */
                Decision();
                double rank() const;
                void setRank(double rank);
                //void addStatement(const Soprano::Statement &, double rank);
                //void addGroup(const PropertiesGroup &);
                /*! \brief Create new group bounded to the Decision
                 * Return pointer to the new PropertiesGroup or 0 if any error has
                 * occured
                 */
                PropertiesGroup newGroup();

                /*! \brief Return all groups of the Decision
                         */
                QSet< PropertiesGroup > groups() const;

                /*! \brief Return urls of all PropertiesGroup
                 */
                //QList<QUrl> groupsUrls() const;

                /*! \brief Set current group to \p group
                 */
                //void setCurrentGroup( const PropertiesGroup & group);

                /*! \brief This method will reset current group( if any)
                 */
                void resetCurrentGroup();


                /*! \brief Return proxy resource for given one
                         * TODO Add checks that given resource is from target model.
                         * This is necessary to prevent situations when target resource
                         * will be from decisions model
                 */
                Nepomuk::Resource proxyResource(const Nepomuk::Resource &);

                /*! \brief Return url of the proxy resource for given one
                                 * TODO Add checks that given resource is from target model.
                                 * This is necessary to prevent situations when target resource
                                 * will be from decisions model
                         */
                QUrl proxyUrl(const Nepomuk::Resource &);

                /*! \brief Return all proxy resources of the Decision
                 * The other names for the proxy resource are 'main Decision resources'
                 * or 'target Decision resources'
                 * \return Hash of all proxy resources of the Decision. Key is url of
                 * the resource in the target ( real-world ) model, value is the url
                 * in the Decision storage model
                 */
                QHash<QUrl, QUrl> proxies() const;

                /*! \brief Return the list of the url of the main Decision resources
                 * The result is the same as proxies().values() call. This function
                 * is provided only for convinience
                 * \return List of the url of the main Decision resources in the Decision
                 * storage model.
                 */
                QList<QUrl> mainResources() const;

                bool isEmpty() const;

                /*! \brief Check the validness of the Decision.
                 * It is easier to detirmine invalid Decision. Decision created with
                 * Decision() constructor is invalid. Most others will be valid.
                 */
                bool isValid() const;

                /*! \brief Return name of the DataPP that creates this Decision
                 * If there is more, then one DataPP, then randomly selected one
                 * will be returned
                 * \return Name of the DataPP
                 * \sa pluginVersion
                 */
                QString  pluginName() const;
                /*! \brief Return version of the DataPP that creates this Decision
                 * If there is more, then one DataPP, then randomly selected one
                 * will be returned. It is guaranteed that this will be version
                 * of the DataPP, whose name was returned with pluginName()
                 * \return Version of the DataPP
                 * \sa pluginName
                 */
                float   pluginVersion() const;
                /*! \brief Return information about all DataPP that create this Decision
                 *\return Set of the DataPP
                 */
                QSet<DataPP*>  pluginsInformation() const;
                /*! \brief Return the uri of the Decision.
                 * Each Decision generated during by one ResourceAnalyzer has
                 * unique uri
                 * \return Url of the Decision
                 */
                QUrl uri() const;
                /*! \brief Return the model of the Decision
                 * This is the model that DataPP should use to create Decision.
                 * \note It is not Decision storage model. Do not try to reach and
                 * edit Decision storage model directly.
                 */
                Soprano::Model * model() const;
                /*! \brief Return the manager of the Decision
                 * This is the ResourceManager that manages the Decision model.
                 * The manager()->mainModel() will return exactly the same model,
                 * as model() call.
                 * \sa model
                 * \return ResourceManager of the Decision model
                 */
                Nepomuk::ResourceManager * manager() const;
                /*! \brief Return description of the Decision.
                 * It is rich text. The description is for displaying to user.
                 * \return Description of the Decision.
                 */
                QString description() const;
                void setDescription(const QString &);


                /*! \brief This function will return the log of all chanegs
                 */
                Sync::ChangeLog log() const;

                ~Decision();
                const Decision & operator=(const Decision & rhs);
                bool operator==(const Decision & rhs)const;
                bool operator!=(const Decision & rhs)const;
                Decision(const Decision &);
                static double truncateRank(double);
                /*! \brief This function load decision from model
                 * Load decision from given model by given url. It will return invalid
                 * decision if resource with given url is not a Decision or if url is
                 * invalid
                 */
                //Decision loadDecision(Soprano::Model * model,QUrl url);
                //
                /*! \brief This method will freeze Decision
                 * Freezed Decision can not be changed
                 */
                void freeze();
                bool isFreezed() const;

                /*! \brief Return application request, that will perform identification for Decision
                 * \param targetModel you cah specify the model for applying changes
                 * \return New IdentificationRequest. The user is responsible for deleting
                 * this object. If Decision is invalid, then 0 will be returned
                 */
                DecisionApplicationRequest * applicationRequest(Soprano::Model * targetModel  = ResourceManager::instance()->mainModel()) const;
                /*! \brief Apply Decision
                 * Write all statements back to model
                         * \param targetModel you cah specify the model for applying changes
                 */
                bool apply(Soprano::Model * targetModel = ResourceManager::instance()->mainModel()) const;
                /*! \brief Add statements to the discretion of the user
                 */
                void addToUserDiscretion();
                friend class ResourceAnalyzer;
                friend class DecisionFactory;
                friend class DecisionList;
                friend class PropertiesGroup;
                friend unsigned int qHash(const Decision &);
            protected:
                /*! \brief Constructor
                 * \param parent The parent of the Decision - DataPP that has created it
                 * \param manager The manager of the model where all information should
                 * be stored
                 */
                Decision(
                    const DataPP * parent,
                    Soprano::Model * decisionsModel,
                    IdentificationSetManager * identsetManager
                );
                void addAuthor(const DataPP * author);
                unsigned int timeStamp() const;
                //QSharedDataPointer<Private> d;
                // ATTENTION The d-pointer must ALWAYS be non-null.
                QSharedPointer<DecisionData> d;

        };
        unsigned int qHash(const Nepomuk::WebExtractor::Decision &);
    }
}

#endif
