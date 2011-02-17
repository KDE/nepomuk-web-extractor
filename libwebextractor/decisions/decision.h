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

#ifndef __webextractor_decision_data_h_
#define __webextractor_decision_data_h_

#include <QSet>
#include <QList>
#include <QString>
#include <QMetaType>
#include <QTime>
#include <QHash>
#include <QUrl>
#include <QSharedDataPointer>

#include "identificationset.h"

#include "propertiesgroup.h"

namespace Soprano {
    class Model;
}

namespace Nepomuk {
    namespace WebExtractor {
    class DecisionApplicationRequest;
    class DecisionCreator;
    class DecisionCreatorInternals;
    class DecisionApplicationRequest;
    /*! \brief This class store the persistent information of Decision
     * 
     */
    class WEBEXTRACTOR_EXPORT Decision
    {
        public:
        //typedef QSharedPointer<Decision> Ptr;
        Decision();
        ~Decision();
        Decision(const Decision &);
        const Decision & operator=( const Decision &);

        /* ==== Introspection section ==== */

        double rank() const;

        QString description() const;

        const QTime & timeStamp() const;

        /*! \brief Return all groups of the Decision
         */
        QList< PropertiesGroup > groups() const;

        /*! \brief Return the number of groups in Decision
         */
        int size() const;

        /*! \brief Return const reference to the group
         * \param index index must be valid, 0 <= index < size()
         * \return const reference to the PropertiesGroup
         */
        const PropertiesGroup & group(int index) const;
        
        /*! \brief Return name of the Executive that creates this DecisionCreator
         * If there is more, then one Executive, then randomly selected one
         * will be returned
         * \return Name of the Executive
         * \sa pluginVersion
         */
        QString  dataPPName() const;

        /*! \brief Return version of the Executive that creates this DecisionCreator
         * If there is more, then one Executive, then randomly selected one
         * will be returned. It is guaranteed that this will be version
         * of the Executive, whose name was returned with pluginName()
         * \return Version of the Executive
         * \sa pluginName
         */
        int dataPPVersion() const;

        /*! \brief Return true if decision is valid
         * Valid decisions are:
         * there is a identification set for each resource in changelog
         */
        bool isValid() const;

        /*! \brief Return true if decision contains any record
         * Non-empty decision is decision that conatins at least one
         * non-empty PropertiesGroup
         * <b>Be aware: empty decisions are valid! </b>
         */
        bool isEmpty() const;

        /*! \brief Return set of the target resources
         */
                QSet<QUrl> targetResources() const;

        const QHash< QUrl, Nepomuk::Sync::IdentificationSet> & identificationSets() const;

        /*! \brief This function will return the global log of the Decision
         * It will join all logs of all PropertiesGroup in the Decision
         * and return it
         */
        Nepomuk::Sync::ChangeLog log() const;

        /* ==== Application section ==== */

                /*! \brief Return application request, that will perform identification for Decision
                 * \param targetModel you cah specify the model for applying changes. If 
         * set to NULL then ResourceManager::instance()->mainModel() will be used.
                 * \return New IdentificationRequest. The user is responsible for deleting
                 * this object. If Decision is invalid, then 0 will be returned
                 */
                DecisionApplicationRequest * applicationRequest(Soprano::Model * targetModel  = 0) const;
                /*! \brief Apply Decision
                 * Write all statements back to model
         * \param targetModel you cah specify the model for applying changes. If
         * set to NULL then ResourceManager::instance()->mainModel() will be used
                 */
                bool apply(Soprano::Model * targetModel = 0) const;

        /* ==== Editing section ==== */
        public:
        void setDescription( const QString & );
        void setRank( double rank );
        private:
        /*! \brief This function create and register new PropertiesGroup
         * \return index of the newly created group
         */
        int  addGroup();
        int  addGroup(const PropertiesGroup &);
        int  addGroup(const Nepomuk::Sync::ChangeLog & log, const QString & description, double rank = 0.5 );
        /*! \brief Set identificationSet for target resources in ChangeLog
         */
        void setIdentificationSets( const QHash<QUrl,Nepomuk::Sync::IdentificationSet> isets );
        //QHash< QUrl, Nepomuk::Sync::IdentificationSet> & identificationSets();
        void addIdentificationSet(const QUrl &, const Nepomuk::Sync::IdentificationSet & iset);

        void setTimeStamp( const QTime & time );


        /*
        void setMainResources(const QSet<QUrl> & resources );
        void addMainResources( const QUrl & resource);
        */

        /*! \brief Set identificationSet for auxilary resources in ChangeLog
         */
        void setAuxiliaryIdentificationSet( const Nepomuk::Sync::IdentificationSet & oset);
        Nepomuk::Sync::IdentificationSet auxiliaryIdentificationSet() const;

        void setResourceProxyMap( const QHash<QUrl,QUrl> & map );
        QHash<QUrl,QUrl> resourceProxyMap() const;

        /*! \brief Remove unnecessary elements
         * This function will remove:
         * Empty PropertiesGroup
         * non-main resources identification sets from identification sets hash
         * <b>Be carefull with this function. Do not call it during Decision
         * editing </b>
         */
        void cleanUnused();


        private:
        /* There are some functions that allow editing of the Decision.
         * Because this class should not be editable by user, this functions
         * are private and are accessible only by several selected classes
         */
        friend class DecisionCreatorInternals;
        friend class DecisionCreator;
        friend class DecisionApplicationRequest;

        void markDirtyLog();
        void markCleanLog() const;
        void markDirtySets();
        void markCleanSets() const;
        void markCleanValidness() const;
        void markCleanTargetResources() const;
        void markCleanEmptyness() const;
        bool isDirtyLog() const;
        bool isDirtySets() const;
        bool isDirtyEmptyness() const;
        bool isDirty() const;
        bool isDirtyTargetResources() const;



        class Private;
        QSharedDataPointer<Private> d;
    };
    }
}
Q_DECLARE_METATYPE(Nepomuk::WebExtractor::Decision);

#endif
