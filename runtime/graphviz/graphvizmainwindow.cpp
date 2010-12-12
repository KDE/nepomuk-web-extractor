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

#include "graphvizmainwindow.h"
#include "vtkGraphDisplay.h"
#include <webextractor/algorithm.h>
#include <QHash>
#include <QFile>
#include <KMessageBox>
#include <KSaveFile>
#include <KDebug>
#include <Soprano/Node>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Term>
#include <dotvisitor.h>
#include <childqueryinterface.h>
#include <selectedpropertiesfunc.h>
#include <resourcenodefilter.h>
#include <modelgraphvisitor.h>
#include <modelgraph.h>
#include <graphalgorithm.h>
#include <plaintextvisitor.h>
#include <vtkgraphvisitor.h>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Types/Class>
#include <Nepomuk/Vocabulary/NFO>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/RDF>

#include <vtkMutableDirectedGraph.h>



using namespace Nepomuk::Graph;
using namespace Nepomuk::Query;
using namespace Nepomuk::Utils;


GraphVizMainWindow::GraphVizMainWindow(QWidget * parent):
    KXmlGuiWindow(parent)
{
    this->setupUi(this);

    // Set query core term for properties selection
    //

    //  Set widgets
    this->resultFileRequester->setMode(KFile::File);

    this->resourcesSelectWidget->setQuery(Nepomuk::Query::Query());

    this->propertiesSelectWidget->setConfigFlags(SearchWidget::SearchWhileYouType);
    Query q(ResourceTypeTerm(Nepomuk::Types::Class(Soprano::Vocabulary::RDF::Property())));
    q.setQueryFlags(q.NoResultRestrictions);
    q.setLimit(100);
    this->propertiesSelectWidget->setBaseQuery(q);

    //  connect signals
    connect(this->applybutton, SIGNAL(clicked()), this, SLOT(draw()));
}
GraphVizMainWindow::~GraphVizMainWindow()
{
}


void GraphVizMainWindow::draw()
{
    // Store file to save result in
    KSaveFile * file;
    QTextStream * stream;
    // Visitors variables
    VtkGraphVisitor * vtkGV; 
    // Store the visitor to use. It is equal to the one of the above
    // variables
    ModelGraphVisitor * visitor = 0;
    // Get current resources
    QSet<QUrl> targets;
    foreach(const Nepomuk::Resource & res, this->resourcesSelectWidget->selectedResources()) {
        targets << res.resourceUri();
    }
    if(targets.isEmpty())
        return;

    kDebug() << "Drawing resources: " << targets;


    if ( this->dotRadioButton->isChecked() or this->textRadioButton->isChecked() ) {
        // Get filename
        KUrl fileUrl = resultFileRequester->url();
        QString filename;

        if(fileUrl.isEmpty()) {
            // Use first resource url as filename
            filename = targets.begin()->toString();
        } else {
            filename = fileUrl.toLocalFile();
        }


        // Create text stream
        file = new KSaveFile(filename);
        stream = new QTextStream(file);
        //QFile * file = new QFile(filename);
        //file->open( QIODevice::WriteOnly );
        //QTextStream stream(file);
        if(!file->open(QIODevice::WriteOnly)) {
            KMessageBox::error(this, "Can't open file for saving");
            return;
        }
        if(this->dotRadioButton->isChecked()) {

            visitor = new DotVisitor(stream); /* Conversation func. This function will perform actual copying */
        } else if(this->textRadioButton->isChecked()) {
            visitor = new PlainTextVisitor(stream);
        }
    }
    else if ( this->vtkRadioButton->isChecked() ) {
        vtkGV = new VtkGraphVisitor();
        visitor = vtkGV;

        if (singleNodeRadioButton->isChecked()) {
            vtkGV->setLiteralBehaviour(VtkGraphVisitor::SingleNode);
        }
        else if ( multipleNodeRadioButton->isChecked() ) {
            vtkGV->setLiteralBehaviour(VtkGraphVisitor::MultipleNodes);
        }
        else {
            vtkGV->setLiteralBehaviour(VtkGraphVisitor::VertexProperty);
        }
    }



    // launch algorithm
    QList<Nepomuk::Resource> lp = this->propertiesSelectWidget->selectedResources();
    QSet<QUrl> sp;
    foreach( const Nepomuk::Resource & r, lp )
    {
        sp << r.resourceUri();
    }

    kDebug() << "Selected properties: " << sp;
    SelectedPropertiesFunc::ConfigFlags f = SelectedPropertiesFunc::NoConfigFlags;
    if ( sp.size() ) {
        if (literalPropertiesCheckBox->isChecked() ) {
            f = f | SelectedPropertiesFunc::AddLiteralProperties;
        }
        if ( defaultPropertiesCheckBox->isChecked() ) {
            f = f | SelectedPropertiesFunc::AddDefaultProperties;
            // Hack. Add nfo:fileName to the list
            sp << Nepomuk::Vocabulary::NFO::fileName();
        }
    }

    ChildQueryInterface * childrenFunc = new SelectedPropertiesFunc(sp,f);

    // Parse some parames
    if(this->nonresourceFilterCheckBox->isChecked())
        childrenFunc = new ResourceNodeFilter(childrenFunc);

    ModelGraph * mg = new ModelGraph(Nepomuk::ResourceManager::instance()->mainModel(), childrenFunc);


    if(visitor) {
        kDebug() << "Parsing graph";
        Nepomuk::Graph::visit_model_graph(
            mg,
            targets,
            visitor,
            -1,
            false
        );
        kDebug() << "Parsing finished";
    }
    else {
        return;
    }

    // Now display
    if ( vtkRadioButton->isChecked() ) {
        kDebug() << "Display VTK";
        // Create new window
        //vtkGV->graph()->Print(std::cout);
        VtkGraphDisplay * w = new VtkGraphDisplay(vtkGV->graph(),vtkGV->edgeLabelArrayName(), vtkGV->vertexLabelArrayName());
        // Make a copy of the graph
        w->show();
    }

    delete visitor;
    delete mg;
    delete childrenFunc;


    if ( dotRadioButton->isChecked() or textRadioButton->isChecked() ) { 
        file->finalize();
        delete file;
        file = 0;
        delete stream;
        stream = 0;
    }
    //delete file;

}
