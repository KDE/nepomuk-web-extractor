/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

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

#ifndef _WEBEXTRCT_PLUGIN_KCM
#define _WEBEXTRCT_PLUGIN_KCM
#include <kdemacros.h>
//#include "dppexecutive.h"
#include <KSharedConfig>
#include <KCModule>
#include <QtCore/QObject>

namespace Nepomuk
{
    namespace WebExtractor {
	class DppExecutive;
    }
    class DppExecutiveConfig;

    /*! \brief This class is an interface for KCM for any webextractor plugin
     * Because each plugin can has more than one instance, the very important 
     * method is setCurrentDppExecutive()
     */
    class KDE_EXPORT WebExtractorPluginKCM : public KCModule
    {
            Q_OBJECT;
        public:
            WebExtractorPluginKCM(const KComponentData & data, QWidget *, const QVariantList &args);
	    void setCurrentDppExecutive( const QSharedPointer<KConfigBase>  & config );
	    QSharedPointer<KConfigBase> currentConfig() const;
            virtual ~WebExtractorPluginKCM() ;
	private:
	    class Private;
	    Private * d;
    };
}
#endif

