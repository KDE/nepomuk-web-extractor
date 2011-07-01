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

#ifndef NEPOMUK_DDMS_PROTOCOL_TYPES
#define NEPOMUK_DDMS_PROTOCOL_TYPES

#include "ddms_export.h"

#include <QPair>
#include <QList>
#include <QMetaType>
#include <QDBusMetaType>

class DDMS_COMMON_EXPORT IdAndError : public QPair<int,int>
{
    public:
        static void registerMetaType()
        {
            static bool registred = false;
            if (!registred) {
                registred = true;
                qRegisterMetaType<IdAndError>("IdAndError");

                qDBusRegisterMetaType<IdAndError>();
            }
        }
};


Q_DECLARE_METATYPE(IdAndError)

DDMS_COMMON_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const IdAndError &mystruct);
DDMS_COMMON_EXPORT const QDBusArgument &operator>>( const QDBusArgument &argument, IdAndError &mystruct);


class Error {
    public:
       enum {SystemError = -1, NoError = 0, NoSuchDecision = 1, DatabaseError };
};

class DDMS_COMMON_EXPORT IdList : public QList<int>
{
    public:
        IdList() {;}
        IdList(const QList<int> & rhs):
            QList<int>(rhs)
        {;}
        static void registerMetaType()
        {
            static bool registred = false;
            if (!registred) {
                registred = true;
                qRegisterMetaType<IdList>("IdList");

                qDBusRegisterMetaType<IdList>();
            }
        }
};
Q_DECLARE_METATYPE(IdList)

class DDMS_COMMON_EXPORT DecisionMetadata
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
Q_DECLARE_METATYPE(DecisionMetadata)
DDMS_COMMON_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const DecisionMetadata &mystruct);
DDMS_COMMON_EXPORT const QDBusArgument &operator>>( const QDBusArgument &argument, DecisionMetadata &mystruct);

class DDMS_COMMON_EXPORT MetadataAndError
{
    public:
        int error;
        DecisionMetadata metadata;
        static void registerMetaType()
        {
            static bool registred = false;
            if (!registred) {
                registred = true;
                qRegisterMetaType<MetadataAndError>("MetadataAndError");

                qDBusRegisterMetaType<MetadataAndError>();
            }
        }
};
Q_DECLARE_METATYPE(MetadataAndError)
DDMS_COMMON_EXPORT QDBusArgument &operator<<(QDBusArgument &argument, const MetadataAndError &mystruct);
DDMS_COMMON_EXPORT const QDBusArgument &operator>>( const QDBusArgument &argument, MetadataAndError &mystruct);

#endif
// vim:sw=4 ts=8 expandtab
