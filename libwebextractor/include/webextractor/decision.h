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
#include <webextractor/webextractor_export.h>
#include <webextractor/propertiesgroup.h>
#include <webextractor/soprano_statement_qhash.h>

namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzer;
	class DecisionFactory;
	class DecisionList;
	class DataPP;
	class PropertiesGroup;

	/*! \brief This class represent one Decision - set of statements that can be applied to resource
	 * Decision consist of PropertiesGroup. PropertiesGroup is a set of statements
	 *  that can be applied only together
	 */
	class WEBEXTRACTOR_EXPORT Decision /*: public QOb*/
	{
	    public:
		double rank() const; 
		void setRank(double rank);
		void addStatement(const Soprano::Statement &, double rank);
		void addGroup(const PropertiesGroup &);
		bool isEmpty() const;
		bool isValid() const;
		const QString & pluginName() const;
		const QString  & pluginVersion() const;
		const QSet<DataPP*> & pluginsInformation() const;
		~Decision();
		const Decision & operator=( const Decision & rhs);
		bool operator==( const Decision & rhs)const;
		bool operator!=( const Decision & rhs)const;
		Decision( const Decision & );
		static double truncateRank(double );
		/*! \brief This function load decision from model
		 * Load decision from given model by given url. It will return invalid
		 * decision if resource with given url is not a Decision or if url is
		 * invalid
		 */
		Decision loadDecision(Soprano::Model * model,QUrl url);
		friend class ResourceAnalyzer;
		friend class DecisionFactory;
		friend class DecisionList;
		friend class PropertiesGroup;
		friend unsigned int qHash( const Decision & );
	    protected:
		Decision(
			const DataPP * parent
			);
		/*! \brief Apply Decision
		 * Write all statements back to model
		 */
		void apply() const;
		/*! \brief Add statements to the discretion of the user
		 */
		void addToUserDiscretion();
		void addAuthor(const DataPP * author);
		unsigned int timeStamp() const;
		class Private;
		QSharedDataPointer<Private> d;

	};
	unsigned int qHash( const Nepomuk::WebExtractor::Decision & );
    }
}

#endif
