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

#ifndef _WEBEXTRCT_PLUGIN
#define _WEBEXTRCT_PLUGIN
#include <kdemacros.h>
//#include "dppexecutive.h"
#include <KConfigBase>
#include <QSharedPointer>
#include <QtCore/QObject>

namespace Nepomuk
{
    namespace WebExtractor {
	class Executive;
    }

    class KDE_EXPORT WebExtractorPlugin : public QObject
    {
            Q_OBJECT;
        public:
            WebExtractorPlugin(QObject *);
            virtual Nepomuk::WebExtractor::Executive * getExecutive(const KConfigGroup & configFile) = 0;
            virtual ~WebExtractorPlugin() = 0;
            // QWidget *
            //
    };
}
#endif
