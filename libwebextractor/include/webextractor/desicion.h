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
#include <Nepomuk/Resource>
#include <Soprano/Statement>
#include <webextractor/webextractor_export.h>

namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzer;
	class WEBEXTRACTOR_EXPORT Decision /*: public QOb*/
	{
	    public:
		double rank();
		void setRank(double rank);
		void addStatement(const Soprano::Statement &, double rank);
		void addStatementGroup(const QList<Soprano::Statement> &, double rank);
		friend class ResourceAnalyzer;
	    private:
		/*! \brief Apply Decision
		 * Write all statements back to model
		 */
		void apply();
		/*! \brief Add statements to the discretion of the user
		 */
		void addToUserDiscretion();
		class Private;
		QSharedPointer<Private> d;

	};
    }
}
#endif
