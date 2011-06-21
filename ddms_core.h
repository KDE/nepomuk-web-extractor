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

namespace Nepomuk {
    class Decision;
    class DecisionStorage
    {
	public:
	    typedef int ID;
	    enum Error {SystemError = -1, NoError = 0, NoSuchDecision = 1, DatabaseError };
	    /*! \brief Create new DecisionStorage
	     */
	    DecisionStorage(const QSqlDatabase & database, const QString & pathFolder);
	    ~DecisionStorage();
	    /*! \brief Return true if everything is ok
	     * Check that database do exists and that pathFolder do exists too
	     */
	    bool isValid() const;
	    
	    /*! \brief Add decision to the index
	     * \return ID of the added decision, or -1 if failed
	     */
	    Error  addDecision( const Decision & );

	    /*! \brief Return list of ID of the decisions, related to the resource
	     */
	    QList<ID> queryDecisions(const QUrl & resource);

	    /*! \brief Remove decision with specified ID
	     * Nothing happens if there is no such Decision
	     */
	    Error removeDecision(ID id);
	private:
	   class Private;
	   QSharedPointer<Private> d;
    };
}

#endif 
