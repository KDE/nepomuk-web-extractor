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


#include "decisiondata.h"
#include <KDebug>
#include "ndco.h"

namespace NW = Nepomuk::WebExtractor;


NW::DecisionData::~DecisionData()
{;
    kDebug() << "DecisionData is destroyed";
}

bool NW::DecisionData::isFreezed() const
{
    return m_freeze;
}

void NW::DecisionData::setFreeze(bool val)
{
    m_freeze = val;
}

QUrl NW::DecisionData::createPropertiesGroupUrl()
{
    QUrl answer = manager->generateUniqueUri("grp");
    // Then attach the group to the Decision
    Soprano::Statement st(this->contextUrl, NW::Vocabulary::NDCO::hasPropertiesGroup(),answer, this->contextUrl);
    Soprano::Error::ErrorCode error = this->model->addStatement(st);
    if ( error != Soprano::Error::ErrorNone ) {
	kError() << " An error when adding PropertiesGroup to the Decision has occured ";
	return QUrl();
    }

    return answer;
}
