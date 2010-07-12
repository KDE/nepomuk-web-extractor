#include "examinedwidget.h"
#include <KMessageBox>

ExaminedWidget::ExaminedWidget(QWidget * parent):
    QTableWidget(parent)
{;}

bool ExaminedWidget::dropMimeData(int row, int column, const QMimeData * data, Qt::DropAction action )
{
    // Allways mark DataPP as examined. But add it to model only if it is not already in.
    // If it is, then update it's version

    // Decode
    if (!data->hasText())
	return;

    // Get name
    QString name = data->text();
    
    // Get version
    DataPPConfig * dppcfg = new DataPPConfig(name);
    if (!dppcfg->isValid()) {
	KMessageBox::error("Incorrect value was passed as DataPP name");
	return;
    }

    QString version = dppcfg->version();
    delete dppcfg;

    // Mark...
    
    // Add to model
    // Search for it
    QList<QTableWidgetItem*> res = this->findItems(name);
    bool hasAny = false;
    // Names are stored at column 0, versions at column 1
    // Check that there is any name in res
    QTableWidgetItem * datapp_item = 0;
    foreach(QTableWidgetItem* item, res)
    {
	// Check that there is only one item
	if ( item->column == 0) {
	    if (hasAny) {
		kError() << "There must be only one item for every DataPP";
	    }
	    hasAny = true;
	    datapp_item = item;
	}
    }
    if (!hasAny) {

	// If no version available then there is no such 
	// Add new item
	QTableWidgetItem * item = new QTableWidgetItem(name);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
	setItem(this->rowCount(),0,item);

	item = new QTableWidgetItem(version);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled );
	setItem(this->rowCount(),1,item);
    }
    else {
	// Update version
	// Get item for version ( we have item for name )
	int row = item->row();
	item(row,1)->setText(version);
    }

}
