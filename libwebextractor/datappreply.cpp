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

#include "datappreply.h"
#include "datapp.h"
#include "datappreply_p.h"


Nepomuk::WebExtractor::DataPPReply::DataPPReply(
    DataPP * parent
):
    d_ptr(new DataPPReplyPrivate())
{
    Q_D(DataPPReply);
    Q_CHECK_PTR(parent);
    d->m_parent = parent;
}

Nepomuk::WebExtractor::DataPPReply::DataPPReply(DataPPReplyPrivate & p, DataPP* parent):
    d_ptr(&p)
{
    Q_D(DataPPReply);
    Q_CHECK_PTR(parent);
    d->m_parent = parent;
}
/*
void Nepomuk::WebExtractor::DataPPReply::setPluginName(const QString & pluginName)
{
    d->m_pluginName = pluginName;
}
*/

Nepomuk::WebExtractor::DataPPReply::~DataPPReply()
{
    delete d_ptr;
}

Nepomuk::WebExtractor::DataPP * Nepomuk::WebExtractor::DataPPReply::parentDataPP() const
{
    Q_D(const DataPPReply);
    return d->m_parent;
}


QString Nepomuk::WebExtractor::DataPPReply::pluginName() const
{
    Q_D(const DataPPReply);
    return d->m_parent->pluginName();
}


/*
QString Nepomuk::WebExtractor::DataPPReply::pluginVersion() const
{ return d->m_pluginVersion; }

*/
