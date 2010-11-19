/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

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

#ifndef __webextractor_plugin_olena_h_
#define __webextractor_plugin_olena_h_

#include "webextractor_plugin.h"
#include <QString>

namespace Nepomuk
{
    class OlenaPlugin : public WebExtractorPlugin
    {
	Q_OBJECT

    public:
        OlenaPlugin(QObject*, const QList<QVariant>&);
        Nepomuk::WebExtractor::DataPP* getDataPP(const QSharedPointer<KConfigBase>& configFile);
        static int version();
    };
}

#endif 
