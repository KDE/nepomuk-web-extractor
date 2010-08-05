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

#include "webextractor_export.h"

#ifndef __nepomuk_chaneglogfiltermodel_h_
#define __nepomuk_chaneglogfiltermodel_h_

#include <Soprano/FilterModel>
#include <Soprano/Statement>
#include "nepomuk/changelogrecord.h"
#include "nepomuk/changelog.h"

namespace Nepomuk {
    namespace Sync {
	//template < typename ChangeLogType>
	//typedef ChangeLog ChangeLogType;
	class ChangeLogFilterModel : public Soprano::FilterModel
	{
	    public:
		ChangeLogFilterModel(ChangeLog * t, Soprano::Model * parent):
		    Soprano::FilterModel(parent)
		{
		    m_log = t;
		}
		
		void setLog( ChangeLog * t)
		{
		    m_log = t;
		}
		
		ChangeLog * log() const { return m_log; }

		virtual Soprano::Error::ErrorCode addStatement (const Soprano::Statement &statement)
		{
		    Soprano::Error::ErrorCode c = Soprano::FilterModel::addStatement(statement);

		    // If log not set  
		    if (!m_log )
			return c;

		    if ( c != Soprano::Error::ErrorNone)
			return c;

		    // Add to log
		    //*m_log << ChangeLogRecord(QDateTime::currentDateTime(), true, statement);
		    *m_log << ChangeLogRecord(statement);
		    //m_log->add(ChangeLogRecord(statement));
		    return c;
		}
		
		virtual Soprano::Error::ErrorCode removeStatement (const Soprano::Statement &statement)
		{
		    Soprano::Error::ErrorCode c = Soprano::FilterModel::removeStatement(statement);

		    // If log not set  
		    if (!m_log )
			return c;

		    if ( c != Soprano::Error::ErrorNone)
			return c;

		    // Add to log
		    *m_log << ChangeLogRecord(QDateTime::currentDateTime(), false, statement);
		   /* m_log->add(
			    ChangeLogRecord(QDateTime::currentDateTime(), false, statement)
			    );*/
		    return c;
		}

	    private:
		ChangeLog * m_log;

	};
    }
}

#endif

