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


#include "vtkGraphDisplay.h"
#include <vtkMutableDirectedGraph.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkGraphToPolyData.h>
#include <vtkGraphLayout.h>
#include <vtkRandomLayoutStrategy.h>
#include <vtkMutableDirectedGraph.h>
#include <vtkSmartPointer.h>
#include <vtkGraphLayoutView.h>
#include <vtkViewTheme.h>
#include <vtkDataSetAttributes.h>
#include <vtkStringArray.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAnnotatedCubeActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <QVTKWidget.h>

#include <KDebug>
#include <QKeyEvent>

VtkGraphDisplay::VtkGraphDisplay(vtkMutableDirectedGraph * inputGraph, const char * edgeLabelArrayName, const char * vertexLabelArrayName,QWidget * parent):
    QWidget(parent)
{

    this->dAsimuth = 5.0;
    this->dElevation = 5.0;
    this->dRoll = 5.0;
    this->setupUi(this);
    connect(this->upButton, SIGNAL(clicked()),this,SLOT(upCamera()));
    connect(this->downButton, SIGNAL(clicked()),this,SLOT(downCamera()));
    connect(this->leftButton, SIGNAL(clicked()),this,SLOT(leftCamera()));
    connect(this->rightButton, SIGNAL(clicked()),this,SLOT(rightCamera()));
    //QVTKWidget * vtkWidget = new QVTKWidget(this);
    // Add to layout
    //this->gridLayout->addWidget(new QCheckBox(),0,0);
    //this->gridLayout->addWidget(vtkWidget,0,0);

    if ( edgeLabelArrayName )
        this->edgeLabelArrayName = edgeLabelArrayName;
    else 
        this->edgeLabelArrayName = "label";

    if ( vertexLabelArrayName )
        this->vertexLabelArrayName = vertexLabelArrayName;
    else 
        this->vertexLabelArrayName = "label";

    this->graph = vtkMutableDirectedGraph::New();
    /*
    vtkIdType a = graph->AddVertex();
    vtkIdType b = graph->AddChild(a);
    vtkIdType c = graph->AddChild(a);
    vtkIdType d = graph->AddChild(b);
    vtkIdType e = graph->AddChild(c);
    vtkIdType f = graph->AddChild(c);

    vtkStringArray* labels = vtkStringArray::New();
    labels->SetName(this->edgeLabelArrayName);
    labels->InsertValue(a, "a");
    labels->InsertValue(b, "b");
    labels->InsertValue(c, "c");
    labels->InsertValue(d, "d");
    labels->InsertValue(e, "e");
    labels->InsertValue(f, "f");
    this->graph->GetVertexData()->AddArray(labels);
    */
    
    this->graph->ShallowCopy(inputGraph);
    this->graph->Print(std::cout);

    // Check graph
    if (!inputGraph->GetVertexData()->GetAbstractArray(0) ) {
        kError() << "Vertex 0 array not found";
    }

    if (!inputGraph->GetVertexData()->GetAbstractArray(this->vertexLabelArrayName) ) {
        kError() << "Vertex label array not found. label: " << this->vertexLabelArrayName;
    }

    if (!this->graph->GetEdgeData()->GetAbstractArray(this->edgeLabelArrayName) ) {
        kError() << "Edge label array not found. label: " << this->edgeLabelArrayName;
    }

    theme.TakeReference(vtkViewTheme::CreateMellowTheme());
    theme->SetLineWidth(5);
    theme->SetCellOpacity(0.9);
    theme->SetCellAlphaRange(0.5,0.5);
    theme->SetPointSize(10);
    theme->SetSelectedCellColor(1,0,1);
    theme->SetSelectedPointColor(1,0,1);
    


    this->view = vtkSmartPointer<vtkGraphLayoutView>::New();
    view->SetRepresentationFromInput(this->graph/*->GetOutputPort()*/);

    vtkRandomLayoutStrategy * layout = vtkRandomLayoutStrategy::New();
    layout->ThreeDimensionalLayoutOn();
    view->SetLayoutStrategy(layout);
    //view->SetEdgeLayoutStrategy("Arc Parallel");
    //view->ColorEdgesOn();
    //view->ColorVerticesOn();
    view->ApplyViewTheme(theme);

    view->EdgeLabelVisibilityOn();
    view->SetEdgeLabelArrayName(this->edgeLabelArrayName);
    view->SetEdgeLabelFontSize(18);

    view->SetVertexLabelArrayName(this->vertexLabelArrayName);
    view->SetVertexLabelFontSize(20);
    view->VertexLabelVisibilityOn();
  /*
    vtkRandomLayoutStrategy * layout = vtkRandomLayoutStrategy::New();
    layout->ThreeDimensionalLayoutOn();

    vtkGraphLayout * graphlayout = vtkGraphLayout::New();
    graphlayout->SetInput(this->graph);
    graphlayout->SetLayoutStrategy(layout);

    vtkGraphToPolyData * graphtopoly = vtkGraphToPolyData::New();
    graphtopoly->SetInputConnection (graphlayout->GetOutputPort());

    vtkPolyDataMapper * mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(graphtopoly->GetOutputPort());

    vtkActor * graphactor = vtkActor::New();
    graphactor->SetMapper(mapper);

    vtkRenderer * ren = vtkRenderer::New();
    ren->AddActor(graphactor);

    w->GetRenderWindow()->AddRenderer(ren);
    */
    //view->GetRenderWindow();
    //vtkWidget->GetRenderWindow();
    //vtkWidget->SetRenderWindow(view->GetRenderWindow());
    view->SetInteractor(vtkWidget->GetInteractor());
    vtkWidget->SetRenderWindow(view->GetRenderWindow());

    //vtkWidget->GetRenderWindow()->Render();
    //view->SetInteractor(vtkWidget->GetInteractor());
    //vtkWidget->GetRenderWindow()->AddRenderer(view->GetRenderer());

    // Add orientation marker
    vtkAnnotatedCubeActor * axesActor = vtkAnnotatedCubeActor::New();
    axesActor->SetXPlusFaceText("R");
    axesActor->SetXMinusFaceText("L");
    axesActor->SetYMinusFaceText("H");
    axesActor->SetYPlusFaceText("F");
    axesActor->SetZMinusFaceText("P");
    axesActor->SetZPlusFaceText("A");
    axesActor->GetTextEdgesProperty()->SetColor(1,1,0);
    axesActor->GetTextEdgesProperty()->SetLineWidth(2);
    axesActor->GetCubeProperty()->SetColor(0,0,1);
    vtkOrientationMarkerWidget * marker = vtkOrientationMarkerWidget::New();
    marker->SetOrientationMarker(axesActor);
    marker->SetInteractor(vtkWidget->GetInteractor());
    marker->SetEnabled(1);
    marker->SetInteractive(1);

    view->GetRenderer()->ResetCamera();
    //vtkWidget->GetRenderWindow()->Render();
}

/*
void VtkGraphDisplay::keyPressEvent ( QKeyEvent * event )
{
    double droll = 5.0;
    // Get camera
    vtkCamera * c = view->GetRenderer()->GetActiveCamera();
    kDebug() << "Key press event";
    switch( event->key() )
    {
        case ( Qt::Key_Left ) : { 
                                    kDebug() << "Rotate left";
                                    c->SetRoll( c->GetRoll() + droll ) ;break; }
        case ( Qt::Key_Right ) : { 
                                     kDebug() << "Rotate right";
                                     c->SetRoll( c->GetRoll() - droll ) ;break; }
        default: { QWidget::keyPressEvent(event); }
    }
}
*/
void VtkGraphDisplay::upCamera()
{
    vtkCamera * c = view->GetRenderer()->GetActiveCamera();
    c->Elevation(dElevation);
    vtkWidget->GetRenderWindow()->Render();
}

void VtkGraphDisplay::downCamera()
{
    vtkCamera * c = view->GetRenderer()->GetActiveCamera();
    c->Elevation(-dElevation);
    vtkWidget->GetRenderWindow()->Render();
}

void VtkGraphDisplay::leftCamera()
{
    vtkCamera * c = view->GetRenderer()->GetActiveCamera();
    c->Azimuth(dAsimuth);
    vtkWidget->GetRenderWindow()->Render();
}

void VtkGraphDisplay::rightCamera()
{
    vtkCamera * c = view->GetRenderer()->GetActiveCamera();
    c->Azimuth(-dAsimuth);
    vtkWidget->GetRenderWindow()->Render();
}
