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


#ifndef __NEPOMUK_GRAPH_VIZ_VTK_DISPLAY_H_
#define __NEPOMUK_GRAPH_VIZ_VTK_DISPLAY_H_

#include <QWidget>

#include <vtkSmartPointer.h>

#include "ui_vtkGraphDisplay.h"
#include <vtkCosmicTreeLayoutStrategy.h>
#include <vtkCircularLayoutStrategy.h>
#include <vtkRandomLayoutStrategy.h>
#include <vtkForceDirectedLayoutStrategy.h>

class vtkMutableDirectedGraph;
class vtkViewTheme;
class vtkGraphLayoutView;
class VtkGraphDisplay : public QWidget, public Ui_VtkGraphDisplay
{
    Q_OBJECT;
    public:
        VtkGraphDisplay(vtkMutableDirectedGraph * graph, const char * edgeLabelArrayName = 0, const char * vertexLabelArrayName = 0,QWidget * parent = 0);
    protected:
        //void keyPressEvent ( QKeyEvent * event ); 
    private Q_SLOTS:
        void upCamera();
        void downCamera();
        void leftCamera();
        void rightCamera();
        void vertexLabels(bool);
        void edgeLabels(bool);
        void layoutChanged();

    private:
        double dAsimuth;
        double dElevation;
        double dRoll;
        //void updateTheme();
        vtkSmartPointer<vtkViewTheme> theme;
        vtkMutableDirectedGraph * graph;
        vtkSmartPointer<vtkGraphLayoutView> view; 
        const char * edgeLabelArrayName;
        const char * vertexLabelArrayName;

        // Layout stategies
        vtkSmartPointer<vtkGraphLayoutStrategy> currentLS;
        vtkSmartPointer<vtkRandomLayoutStrategy> randomLS;
        vtkSmartPointer<vtkForceDirectedLayoutStrategy> forceDirectedLS;
        vtkSmartPointer<vtkCircularLayoutStrategy> circularLS;
        vtkSmartPointer<vtkCosmicTreeLayoutStrategy> cosmicLS;
};
#endif
