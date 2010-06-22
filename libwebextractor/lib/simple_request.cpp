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

#include <webextractor/simple_request.h>
#include <QtGlobal>

namespace NW=Nepomuk::WebExtractor;

NW::SimpleDataPPRequest::SimpleDataPPRequest(const QString & requestUrlPart, QObject* parent):
    QObject(parent),
    m_requestUrlPart(requestUrlPart),
    m_reply(0)
{
}

NW::SimpleDataPPRequest::~SimpleDataPPRequest()
{
    delete m_reply;
}

QNetworkReply * NW::SimpleDataPPRequest::reply() const
{
    return m_reply;
}

void NW::SimpleDataPPRequest::clear()
{
    if (m_reply)
	m_reply->deleteLater();
    m_reply = 0;
}

QString NW::SimpleDataPPRequest::requestUrlPart() const
{
    return m_requestUrlPart;
}

void NW::SimpleDataPPRequest::setReply( QNetworkReply * reply )
{
    Q_ASSERT(m_reply == 0);
    m_reply = reply;
    connect(m_reply,SIGNAL(finished()),this,SIGNAL(finished()));
   // connect(m_reply,SIGNAL(error(QNetworkReply::NetworkError code)),this,SIGNAL(error()));
}
