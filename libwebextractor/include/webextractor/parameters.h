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

#ifndef _WEBEXTRCT_PARAMETERS
#define _WEBEXTRCT_PARAMETERS

#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QSharedPointer>
#include <QtCore/QSharedDataPointer>
#include <webextractor/global.h>
#include <webextractor/webextractor_export.h>
#include <webextractor/datappwrapper.h>


namespace Nepomuk {
    namespace WebExtractor {
	class WEBEXTRACTOR_EXPORT ExtractParameters
	{
	    public:
		ExtractParameters();
		~ExtractParameters();
		ExtractParameters(const ExtractParameters &);
		const ExtractParameters & operator=( const ExtractParameters & rhs);
		

		WE::MergePolitics mergePolitics() const;
		void setMergePolitics(WE::MergePolitics val) ;

		unsigned int pluginSelectStep() const;
		void setPluginSelectStep(unsigned int step);

		WE::LaunchPolitics launchPolitics() const;
		void setLaunchPolitics( WE::LaunchPolitics politics);

		double aCrit() const;
		void setACrit(double);

		double uCrit() const;
		void setUCrit(double);

		/*
		double scaleCoff(DataPP*)  const;
		void setScaleCoff(DataPP*, double) ;
		*/

		const DataPPKeeper & plugins() const;
		void addDataPP( DataPPWrapper * ) ;

		bool hasAnyDataPP() const;

		friend QDebug operator<<( QDebug dbg,  const ExtractParameters & cat);

	    private:
		class Private;
		QSharedDataPointer<Private> d;
	};
	typedef QSharedPointer< const ExtractParameters> ExtractParametersPtr;
	WEBEXTRACTOR_EXPORT QDebug operator<<( QDebug dbg,  const ExtractParameters & cat);
    }
}
#endif
