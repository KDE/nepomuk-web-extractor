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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_H_

#include "webextractor_export.h"
#include "decision.h"
#include <QObject>
#include <QHash>
#include <QSet>
#include <Soprano/Node> // for qHash(QUrl)
#include <QSharedPointer>

namespace Nepomuk
{
    namespace WebExtractor
    {
        class DecisionApplicationRequestPrivate;
        /*! \brief This class is used for applying Decision
         * The purpose of this class is to apply changes that were introduced
         * in Decision to the target resource.
         * All process can be divided into following stages:
         * Identification - For all resources mentioned in the changes of Decision
         * an attempt to find corresponding resource in target model is performed.
         * Identification is wise process. For example if Decision was generated for
         * some file and this file move to another directory, then it will find this
         * file even in another directory and apply changes to it. If some auxilary
         * resource was not found, then it will be created. But if some main resource
         * of Decision was not found, then application request will exit with error.
         * Application - The changes introduced in Decision are applied to the
         * found resources.
         */
        class WEBEXTRACTOR_EXPORT DecisionApplicationRequest : public QObject
        {
                Q_OBJECT;
            public Q_SLOTS:
                /*! \brief Identify target Decision resources
                 * This function will identify only the target Decision resources.
                 * This is partof the identification stage. It is extracted into
                 * separate function only for more comprehensive user interface
                 * and debugging functionality.
                 */
                void identifyTargets(bool reset = false);
                /*! \brief Perform identification
                 * \param reset It is possible to call this method more than once. But
                 * all calls after first one will be ignored. Set this parameter to true
                 * if you want to force relaunching identification process. It is
                 * usefull if target model is changing quickly
                 */
                void identify(bool reset = false);
                /*! \brief Reset identification information
                      * This method will delete all information that was retrived during
                      * idenification and, possibly, application stages.
                      * \note It will not revert changes if Decision was already applied.
                      * It is not possible!
                      */
                void reset();
                /*! \brief Perform application stage
                 * Apply changes. If identification was not performed, then
                 * identification stage will be execude first
                 * \return true, if application stage succeed.
                 */
                bool apply();
            public:
                ~DecisionApplicationRequest();
                /*! \brief This function allow to explore mappings of the main Decision resources
                 * This function is useless before identification stage finished
                 */
                QHash<QUrl, QUrl> targetsMappings() const;

                /*! \brief This function return true if main resources was successfuly identified
                 */
                bool isTargetsIdentified() const;

                /*! \brief Return the list of the url of the main Decision resources
                 * Only resources that are necessary for this request will be returned
                 * \return List of the url of the main Decision resources in the Decision
                 * storage model that are used in this request.
                 */
                QSet<QUrl> targetResources() const;
                /*! \brief This allow you to explore mappings from resource in Decision to resource in target model
                 * This function is useless before identification stage finished
                 */
                QHash<QUrl, QUrl> mappings() const;
                /*! \brief Return resource that were failed to identify
                 * This call is useless before identification stage finished(
                 * either successfully or not )
                 *
                 */
                QSet<QUrl> unidentified() const;
                friend class Decision;
            Q_SIGNALS:
                /*! \brief This signal is called when some resource was identified
                 */
                void identified(const QUrl & oldUri, const QUrl & newUri);
            private:
                DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, QObject * parent = 0);
                // Disable assignement and copy constructor
                DecisionApplicationRequest(const DecisionApplicationRequest & rhs);
                DecisionApplicationRequest & operator=(const DecisionApplicationRequest & rhs);
            protected:
                DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, DecisionApplicationRequestPrivate & p, QObject * parent = 0);
            private:
                DecisionApplicationRequestPrivate * d_ptr;
                Q_DECLARE_PRIVATE(DecisionApplicationRequest);

        };
    }
}

#endif

