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

#ifndef __properties_group_h_
#define __properties_group_h_

#include <QtCore/QSharedDataPointer>
#include <QtCore/QSet>
#include <Soprano/Statement> 

namespace Nepomuk {
    namespace WebExtractor {
	class Decision;
	class DecisionFactory;
	class PropertiesGroup
	{
	    public:
		PropertiesGroup( const PropertiesGroup &);
		~PropertiesGroup();
		const PropertiesGroup & operator=(const PropertiesGroup &);
		void addStatement(const Soprano::Statement & );
		//void addStatement(const Soprano::Node & predicate, const Soprano::Node & second, bool subject = true );
		double rank() const;
		QSet< Soprano::Statement > data() const;
		void setRank(double newRank );
		bool operator==(const PropertiesGroup & rhs)const;
		bool operator!=(const PropertiesGroup & rhs)const;
		friend PropertiesGroup & operator<<( PropertiesGroup &, const Soprano::Statement & );
		friend class Decision;
		friend class DecisionFactory;
		friend unsigned int qHash(const PropertiesGroup &);
	    protected:
		PropertiesGroup();
		class Private;
		QSharedDataPointer<Private> d;
	};
	PropertiesGroup & operator<<( PropertiesGroup &, const Soprano::Statement & );
    }
}

unsigned int qHash( const Nepomuk::WebExtractor::PropertiesGroup &);

#endif
