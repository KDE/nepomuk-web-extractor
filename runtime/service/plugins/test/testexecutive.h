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


#ifndef __webextractor_datapp_test

#define __webextractor_datapp_test

#include "executive.h"

namespace Nepomuk
{
    class TestReply;
    class TestExecutive : public WebExtractor::Executive
    {
            Q_OBJECT;
        public:
            WebExtractor::ExecutiveReply * requestDecisions(const WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res) ;
            TestExecutive(
                int pluginVersion
            );
            ~TestExecutive();
            friend class TestReply;
    };
}

#endif

