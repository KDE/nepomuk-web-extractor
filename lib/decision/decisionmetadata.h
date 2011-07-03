/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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


#ifndef NEPOMUK_DECISION_METADATA_H
#define NEPOMUK_DECISION_METADATA_H

#include "decision_export.h"
#include <QtCore/QMetaType>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusArgument>

namespace Nepomuk {
    namespace Decision {
        class DECISION_EXPORT DecisionMetadata
        {
            public:
                QString description;
                static void registerMetaType()
                {
                    static bool registred = false;
                    if (!registred) {
                        registred = true;
                        qRegisterMetaType<DecisionMetadata>("DecisionMetadata");

                        qDBusRegisterMetaType<DecisionMetadata>();
                    }
                }
        };
    }

}

DECISION_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const Nepomuk::Decision::DecisionMetadata &mystruct);
DECISION_EXPORT const QDBusArgument &operator>>( const QDBusArgument &argument, Nepomuk::Decision::DecisionMetadata &mystruct);
Q_DECLARE_METATYPE(Nepomuk::Decision::DecisionMetadata)
#endif

