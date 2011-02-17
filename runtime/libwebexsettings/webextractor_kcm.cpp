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

#include "webextractor_kcm.h"
#include "datappconfig.h"

class Nepomuk::WebExtractorPluginKCM::Private 
{
    public:
	Private();
	QSharedPointer<KConfigBase> m_config;
};

Nepomuk::WebExtractorPluginKCM::Private::Private():
    m_config(0)
{;}

Nepomuk::WebExtractorPluginKCM::WebExtractorPluginKCM(const KComponentData & data, QWidget * parent, const QVariantList &args):
    KCModule(data, parent,args),
    d(new Private())
{
}

void Nepomuk::WebExtractorPluginKCM::setCurrentDppExecutive( const QSharedPointer<KConfigBase> & dataPPConfig )
{
    d->m_config = dataPPConfig;
    this->load();
}

QSharedPointer<KConfigBase> Nepomuk::WebExtractorPluginKCM::currentConfig() const
{
    return d->m_config;
}

Nepomuk::WebExtractorPluginKCM::~WebExtractorPluginKCM()
{
    delete d;
}
