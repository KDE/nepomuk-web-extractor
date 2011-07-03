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
#ifndef NEPMOUK_DDMS_CORE
#define NEPOMUK_DDMS_CORE

#include <QSqlDatabase>
#include <QString>
#include <QUrl>
#include <QSharedPointer>

#include "protocol_types.h" 


namespace Nepomuk {
    namespace Decision {
        class Decision;
    }
    class DecisionStorage
    {
	public:
	    typedef int ID;
	    /*! \brief Create new DecisionStorage
	     */
	    DecisionStorage(const QSqlDatabase & database, const QString & pathFolder);
	    ~DecisionStorage();
	    /*! \brief Return true if everything is ok
	     * Check that database do exists and that pathFolder do exists too
	     */
	    bool isValid() const;
	    
	    /*! \brief Add decision to the index
	     * \param decision Decision to add
	     * \param uri List of uri Decision  connected to
	     * \param id If id != NULL, then id of the Decision is written here or -1 
	     * if failed
	     * \return error code ( NoError is succeed )
	     */
	    int  addDecision( const Decision::Decision & decision, const QList<QString> & uri, int * id );

	    /*! \brief Return list of ID of the decisions, related to the resource
	     */
	    QList<ID> queryDecisions(const QUrl & resource);

	    /*! \brief Remove decision with specified ID
	     * Nothing happens if there is no such Decision
	     */
	    int removeDecision(ID id);

            bool existsDecision( int id);

            Decision::DecisionMetadata decisionMetadata(int id, int  & error);
	private:
	   class Private;
	   QSharedPointer<Private> d;
    };
}

#endif 
// vim:sw=4 ts=8 expandtab
