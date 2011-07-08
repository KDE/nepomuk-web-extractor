/*
Copyright (c) 2010 Oszkar Ambrus <aoszkar@gmail.com>
Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.1 of the License, or (at your option) version 3, or any
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

#include "decisionwidgetdesignerplugin.h"
#include "decisionwidget.h"

#include <QtCore/QtPlugin>
#include <QIcon>


DecisionWidgetDesignerPlugin::DecisionWidgetDesignerPlugin( QObject* parent )
  : QObject( parent )
{
}

QWidget* DecisionWidgetDesignerPlugin::createWidget( QWidget* parent )
{
    DecisionWidget* widget = new DecisionWidget( parent );
    // init with some example data useful in the preview inside Qt Designer
    // this data will be only used there, not in the resulting view in the program.
    return widget;
}

QString DecisionWidgetDesignerPlugin::group() const
{
    return QString::fromLatin1("Nepomuk (KDE)");
}

QIcon DecisionWidgetDesignerPlugin::icon() const
{
    return QIcon();
}

QString DecisionWidgetDesignerPlugin::includeFile() const
{
    return QString::fromLatin1("nepomuk/decision/decisionwidget.h");
}

QString DecisionWidgetDesignerPlugin::toolTip() const
{
    return QString::fromLatin1("A widget to display Web Extractor Decisions");
}

QString DecisionWidgetDesignerPlugin::whatsThis() const
{
    return QString::fromLatin1("A widget for displaying and manipulating Decision ");
}

bool DecisionWidgetDesignerPlugin::isContainer() const
{
    return false;
}

QString DecisionWidgetDesignerPlugin::name() const
{
    return QString::fromLatin1("DecisionWidget");
}

// export macro, takes the name of the plugin module and the class name
Q_EXPORT_PLUGIN2( decisionwidgetplugin, DecisionWidgetDesignerPlugin )
