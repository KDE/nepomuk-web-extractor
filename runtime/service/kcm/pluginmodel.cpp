/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "pluginmodel.h"

PluginModel::PluginModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QVariant PluginModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < m_plugins.count()) {
        const DataPPDescr& plugin = m_plugins[index.row()];
        switch(role) {
        case Qt::DisplayRole:
            return plugin.name;
        }
    }

    return QVariant();
}

int PluginModel::rowCount(const QModelIndex &parent) const
{
    if( !parent.isValid() )
        return m_plugins.count();
    else
        return 0;
}

void PluginModel::setPlugins(const QList<DataPPDescr> &plugins)
{
    m_plugins = plugins;
    reset();
}

void PluginModel::addPlugin(const DataPPDescr &plugin)
{
    beginInsertRows(QModelIndex(),m_plugins.count(), m_plugins.count());
    m_plugins << plugin;
    endInsertRows();
}

QList<DataPPDescr> PluginModel::plugins() const
{
    return m_plugins;
}
