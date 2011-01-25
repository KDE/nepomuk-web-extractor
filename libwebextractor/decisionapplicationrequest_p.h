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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_PRIVATE_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_PRIVATE_H_

#include "decisiondata.h"
#include "identificationrequest.h"
#include "changelog.h"
#include <Soprano/Model>
#include <Soprano/Node> // for qHash<QUrl>
#include <QSharedPointer>
#include <QHash>
#include <QSet>

namespace Nepomuk
{
    namespace WebExtractor
    {
        class DecisionApplicationRequestPrivate
        {
            public:
                // This is pointer to the actual decision data.
                // Because class Decision is just a wrapper, storing
                // pointer to Decision is useless comparing to storing
                // pointer to DecisionData
                Decision decision;

                // This is the log that contains all changes
                // that we are trying to apply to the target model
                Nepomuk::Sync::ChangeLog log;

                // Main request.
                Nepomuk::Sync::IdentificationRequest * mainRequest;

                // This is the target model. This model is 'target'
                // of the application stage and Decision storge model
                // is 'source' of the changes. Generally speaking, it is
                // not necessary for this model to be the same model that
                // was used when generating Decision. But this case
                // is rare.
                Soprano::Model * targetModel;

                // In this variable identification information about
                // decision main resources is stored
                QHash<QUrl, QUrl> targetResourceIdentificationHash;

                // In this variable information about these main resources
                // of Decision, that are required for application request
                // is stored.
                // Some of Decision's main resources may not be here,
                // because there is no records containing them in
                // the changelog. E.g. no PropertiesGroup with changes
                // to this missing resoureces is going to be applied
                // This variable is not necessary for algorithm work,
                // but only for users of this class. Because of this
                // it will be populated only after first access.
                //mutable QSet<QUrl> targetResources;

                // This variable is used for staving state of the identification
                // stage. If main resources was successfuly identified, then
                // this variable is set to true.
                //
                bool targetsIdentified;

                // This variable is used for saving state of the identification
                // stage. If identification was successfull then this member
                // will be set to true.
                // If this member is set to true, then any other attempts to
                // launch identification process will be ignored.
                // Use special method to reset this member.
                bool identified;

                // As with identified member, here the state of the
                // application stage is stored. If application stage
                // was successfull, then this member will store true
                // and all other attempts to perform application stage
                // will be silently ignored.
                bool applied;
        };
    }
}

#endif


