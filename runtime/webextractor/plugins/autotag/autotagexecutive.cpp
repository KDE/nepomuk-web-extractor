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

#include "autotagplugin_config.h"

#include "autotagexecutive.h"

#include "executivereply.h"

#include "autotagexecutivereply.h"


Nepomuk::AutotagExecutive::AutotagExecutive(
    int pluginVersion,
    const QRegExp & regexp,
    const QString & tag,
    const QString & description
):
    Executive(pluginVersion),
    m_regexp(regexp),
    m_tag(tag),
    m_description(description)
{
}


Nepomuk::WebExtractor::ExecutiveReply * Nepomuk::AutotagExecutive::requestDecisions(const Decision::DecisionFactory * factory, const Nepomuk::Resource & res)
{

    return new AutotagReply(this, factory, res);

}

Nepomuk::AutotagExecutive::~AutotagExecutive()
{
    ;
}

