/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "misc.h"
#include "identificationrequest.h"
#include "mergerequest.h"
#include "changelog.h"
#include "identificationset.h"

void Nepomuk::Sync::performMerge(Soprano::Model* fromModel, const Nepomuk::Sync::ChangeLog& log, Soprano::Model* toModel)
{
    IdentificationSet identSet = IdentificationSet::fromChangeLog( log, fromModel );
    
    IdentificationRequest identReq( log, identSet, toModel );
    identReq.load();
    identReq.identifyAll();

    ChangeLog newChangeLog = identReq.convert();
    MergeRequest mergeReq( newChangeLog, toModel );
    mergeReq.merge();
}


void Nepomuk::Sync::performMerge(Soprano::Model* fromModel, const QList< Soprano::Statement >& sts, Soprano::Model* toModel)
{
    ChangeLog log( sts );
    performMerge( fromModel, log, toModel );
}
