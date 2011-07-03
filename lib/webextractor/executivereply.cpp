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

#include "executivereply.h"
#include "executive.h"
#include "executivereply_p.h"
#include "global.h"

namespace NW = Nepomuk::WebExtractor;

NW::ExecutiveReply::ExecutiveReply(
    Executive * parent
):
    d_ptr(new ExecutiveReplyPrivate())
{
    init(parent);
}

NW::ExecutiveReply::ExecutiveReply(ExecutiveReplyPrivate & p, Executive* parent):
    d_ptr(&p)
{
    init(parent);
}

void NW::ExecutiveReply::init(Executive * parent)
{
    Q_D(ExecutiveReply);
    Q_CHECK_PTR(parent);
    d->m_parent = parent;
    d->m_error = NoError;
    d->m_timer.setInterval(defaultExecutiveTimeout());
    d->m_timer.setSingleShot(true);
    d->m_timer.start();
    connect(&this->d_ptr->m_timer, SIGNAL(timeout()), this, SLOT(finishByTimeout()));
}
/*
void NW::ExecutiveReply::setPluginName(const QString & pluginName)
{
    d->m_pluginName = pluginName;
}
*/

NW::ExecutiveReply::~ExecutiveReply()
{
    delete d_ptr;
}

void NW::ExecutiveReply::setError(ExecutiveReply::Error errorCode)
{
    Q_D(ExecutiveReply);
    d->m_error = errorCode;
}

NW::ExecutiveReply::Error NW::ExecutiveReply::error() const
{
    Q_D(const ExecutiveReply);
    return d->m_error;
}

NW::Executive * NW::ExecutiveReply::parentExecutive() const
{
    Q_D(const ExecutiveReply);
    return d->m_parent;
}


QString NW::ExecutiveReply::pluginName() const
{
    Q_D(const ExecutiveReply);
    return d->m_parent->name();
}

int NW::ExecutiveReply::timeout() const
{
    Q_D(const ExecutiveReply);
    // Call abort function
    return d->m_timer.interval();
}

void NW::ExecutiveReply::setTimeout(int interval)
{
    Q_D(ExecutiveReply);
    d->m_timer.stop();
    d->m_timer.setInterval(interval);
    d->m_timer.start();
}

void NW::ExecutiveReply::finishByTimeout()
{
    Q_D(ExecutiveReply);
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

void NW::ExecutiveReply::aboutToTimeOut()
{
    ;
}
/*
QString NW::ExecutiveReply::pluginVersion() const
{ return d->m_pluginVersion; }

*/
