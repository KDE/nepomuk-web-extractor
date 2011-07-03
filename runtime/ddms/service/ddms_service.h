/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#ifndef NEPOMUK_DECISION_MANAGEMENT_SERVICE_H
#define NEPOMUK_DECISION_MANAGEMENT_SERVICE_H

#include <Nepomuk/Service>
#include <QString>
#include <QList>

#include "ddms_core.h"
#include "protocol_types.h"

namespace Nepomuk {

    class DecisionManagementService : public Nepomuk::Service 
    {
        //private
        Q_OBJECT;
        Q_CLASSINFO("D-Bus Interface", "org.kde.nepomuk.DecisionManagement");

        public:
            typedef DecisionStorage::ID ID;
            DecisionManagementService(QObject * parent, const QList<QVariant> &);
            ~DecisionManagementService();

        public Q_SLOTS:
            //void reconfigure();
            IdList getDecisions(const QString & uri);
            //int applyDecision(ID id);
            int removeDecision(int id);
	    /*! \brief Add decision and return it's ID
	     * \param uri List of resources uri this Decision connected to
	     * \param decision Decision to add
	     */
	    IdAndError addDecision( const QString & decision, const QList<QString> & uri);

            bool existsDecision(int id);

            MetadataAndError decisionMetadata(int id);
	private:
	    DecisionStorage * m_storage;
	    QSqlDatabase m_db;

    };
}

#endif

// vim:sw=4 ts=8 expandtab
