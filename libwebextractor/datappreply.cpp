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

#include "datappreply.h"
#include "datapp.h"
#include "datappreply_p.h"
#include "global.h"

namespace NW = Nepomuk::WebExtractor;

NW::DataPPReply::DataPPReply(
    DataPP * parent
):
    d_ptr(new DataPPReplyPrivate())
{
    init(parent);
}

NW::DataPPReply::DataPPReply(DataPPReplyPrivate & p, DataPP* parent):
    d_ptr(&p)
{
    init(parent);
}

void NW::DataPPReply::init(DataPP * parent)
{
    Q_D(DataPPReply);
    Q_CHECK_PTR(parent);
    d->m_parent = parent;
    d->m_error = NoError;
    d->m_timer.setInterval(defaultDataPPTimeout());
    d->m_timer.setSingleShot(true);
    d->m_timer.start();
    connect(&this->d_ptr->m_timer, SIGNAL(timeout()), this, SLOT(finishByTimeout()));
}
/*
void NW::DataPPReply::setPluginName(const QString & pluginName)
{
    d->m_pluginName = pluginName;
}
*/

NW::DataPPReply::~DataPPReply()
{
    delete d_ptr;
}

void NW::DataPPReply::setError(DataPPReply::Error errorCode)
{
    Q_D(DataPPReply);
    d->m_error = errorCode;
}

NW::DataPPReply::Error NW::DataPPReply::error() const
{
    Q_D(const DataPPReply);
    return d->m_error;
}

NW::DataPP * NW::DataPPReply::parentDataPP() const
{
    Q_D(const DataPPReply);
    return d->m_parent;
}


QString NW::DataPPReply::pluginName() const
{
    Q_D(const DataPPReply);
    return d->m_parent->name();
}

int NW::DataPPReply::timeout() const
{
    Q_D(const DataPPReply);
    // Call abort function
    return d->m_timer.interval();
}

void NW::DataPPReply::setTimeout(int interval)
{
    Q_D(DataPPReply);
    d->m_timer.stop();
    d->m_timer.setInterval(interval);
    if(interval > 0) {
        d->m_timer.start();
    }
}

void NW::DataPPReply::finishByTimeout()
{
    Q_D(DataPPReply);
    //kDebug() << "Finish by timeout";
    // Call user function.
    this->aboutToTimeOut();

    if(d->m_timer.isActive()) {
        //kDebug()  << "Timer was reactivated";
        return;
    } else {
        //kDebug() << "Aborting";
        d->m_error = TimeExceeded;
        this->abort();
        emit error(d->m_error);
    }
}

void NW::DataPPReply::aboutToTimeOut()
{
    ;
}
/*
QString NW::DataPPReply::pluginVersion() const
{ return d->m_pluginVersion; }

*/
