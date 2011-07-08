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

#ifndef __properties_group_data_h_
#define __properties_group_data_h_

#include <QSharedDataPointer>
#include <QSharedPointer>
#include <QString>
#include "decision_export.h"

#include <nepomuk/simpleresourcegraph.h>


class QDataStream;

namespace Soprano
{
    class Model;
    class Statement;
}

namespace Nepomuk
{
    namespace Decision {
	class DECISION_EXPORT PropertiesGroup
	{
	    public:
		typedef QSharedPointer<PropertiesGroup> Ptr;

		PropertiesGroup();
		PropertiesGroup(const Nepomuk::SimpleResourceGraph & changes,const QString & description, double rank = 0.5 );
		~PropertiesGroup();
		PropertiesGroup(const PropertiesGroup &);

		/* ==== Introspection section ==== */
		const PropertiesGroup & operator=( const PropertiesGroup &);
		QString description() const;
		double rank() const;

		bool isValid() const;

		/*! \brief Return true if log is empty
		 */
		bool isEmpty() const;

		/* ==== Editing section ==== */
		void setRank(double rank);
		void setDescription( const QString & description );
		void setChanges( const Nepomuk::SimpleResourceGraph & changes);
                friend QDataStream & operator<<(QDataStream &, const Nepomuk::Decision::PropertiesGroup & );
                friend QDataStream & operator>>(QDataStream &, Nepomuk::Decision::PropertiesGroup & );
	    private:
                /*! \brief This function will return the log of all chanegs
                 * \note Private as API is unstable
                 */
                SimpleResourceGraph changes() const;
                friend class Decision;
		/* This functions are used for editing and are accessible only
		 * by several classes
		 */
		//Nepomuk::Sync::ChangeLog* logPtr() const;

		class Private;
		QSharedDataPointer<Private> d;
	};
        DECISION_EXPORT QDataStream & operator<<(QDataStream &, const Nepomuk::Decision::PropertiesGroup & );
        DECISION_EXPORT QDataStream & operator>>(QDataStream &, Nepomuk::Decision::PropertiesGroup & );
    }
}


#endif
