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

#include "protocol_types.h"

QDBusArgument &operator<<(QDBusArgument &argument, const IdAndError &mystruct)
{
    argument.beginStructure();
    argument << mystruct.first << mystruct.second;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>( const QDBusArgument &argument, IdAndError &mystruct)
{
    argument.beginStructure();
    argument >> mystruct.first >> mystruct.second;
    argument.endStructure();
    return argument;
}



QDBusArgument &operator<<(QDBusArgument &argument, const MetadataAndError &mystruct)
{
    argument.beginStructure();
    argument << mystruct.error << mystruct.metadata;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>( const QDBusArgument &argument, MetadataAndError &mystruct)
{
    argument.beginStructure();
    argument >> mystruct.error >> mystruct.metadata;
    argument.endStructure();
    return argument;
}

