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


#ifndef MISC_H
#define MISC_H

#include <QtCore/QList>
#include "nepomuksync_export.h"

namespace Soprano {
    class Model;
    class Statement;
}

namespace Nepomuk {
    namespace Sync {

        class ChangeLog;

        /**
         * A convenience function to merge the \p log into \p toModel.
         *
         * Internally the \p fromModel is used to generate the IdentificationSet from the
         * \p log. Both the \p log and the identificationSet are then passed to an IdentificationRequest.
         * After identification, the new ChangeLog is passed to a MergeRequest.
         *
         * \param toModel It is used to perform the identification. This is the model to which
         *                the \p log will be merged to.
         */
        NEPOMUKSYNC_EXPORT void performMerge( Soprano::Model * fromModel,
                                              const ChangeLog & log,
                                              Soprano::Model * toModel );

        /**
         * A convenience function to merge the statments \p sts into \p toModel.
         * 
         * The Statements are internally converted into a ChangeLog with the timestamp
         * equal to the currentDateTime and all statements marked as add.
         */
        NEPOMUKSYNC_EXPORT void performMerge( Soprano::Model * fromModel,
                                              const QList<Soprano::Statement> & sts,
                                              Soprano::Model * toModel );
                           
    }
}
#endif // MISC_H
