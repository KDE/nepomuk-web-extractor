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

#include "simplenetworkrequest.h"
#include <QtGlobal>

namespace NW=Nepomuk::WebExtractor;

NW::SimpleNetworkExecutiveRequest::SimpleNetworkExecutiveRequest(const QString & requestUrlPart, QObject* parent):
    QObject(parent),
    m_requestUrlPart(requestUrlPart),
    m_reply(0)
{
}

NW::SimpleNetworkExecutiveRequest::~SimpleNetworkExecutiveRequest()
{
    delete m_reply;
}

QNetworkReply * NW::SimpleNetworkExecutiveRequest::reply() const
{
    return m_reply;
}

void NW::SimpleNetworkExecutiveRequest::clear()
{
    if (m_reply)
	m_reply->deleteLater();
    m_reply = 0;
}

QString NW::SimpleNetworkExecutiveRequest::requestUrlPart() const
{
    return m_requestUrlPart;
}

void NW::SimpleNetworkExecutiveRequest::setReply( QNetworkReply * reply )
{
    Q_ASSERT(m_reply == 0);
    m_reply = reply;
    connect(m_reply,SIGNAL(finished()),this,SIGNAL(finished()));
   // connect(m_reply,SIGNAL(error(QNetworkReply::NetworkError code)),this,SIGNAL(error()));
}
