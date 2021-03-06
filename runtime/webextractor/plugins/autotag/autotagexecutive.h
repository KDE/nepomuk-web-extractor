/*
   Copyright (C) 2010 by Artem Serebriyskiy <v.for.vandal@gmail.com>

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


#ifndef __webextractor_datapp_autotag

#define __webextractor_datapp_autotag

#include "executive.h"
#include <QRegExp>


namespace Nepomuk
{
    class AutotagReply;
    class AutotagExecutive : public WebExtractor::Executive
    {
            Q_OBJECT;
        public:
            WebExtractor::ExecutiveReply * requestDecisions(const Decision::DecisionFactory * factory, const Nepomuk::Resource & res) ;
            AutotagExecutive(
                int pluginVersion,
                const QRegExp & regexp,
                const QString & tag,
                const QString & description
            );
            ~AutotagExecutive();
            friend class AutotagReply;
        private:
            QRegExp m_regexp;
            QString m_tag;
            QString m_description;
    };
}

#endif

