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

#include "examinedwidget.h"
#include "datapp.h"
#include "executive.h"
#include "datapppool.h"
#include <KMessageBox>
#include <QMimeData>

ExaminedWidget::ExaminedWidget(QWidget * parent):
    QTableWidget(parent)
{
    ;
}

#if 0
bool ExaminedWidget::dropMimeData(int row, int column, const QMimeData * data, Qt::DropAction action)
{
    // Allways mark DataPP as examined. But add it to model only if it is not already in.
    // If it is, then update it's version

    kDebug() << "Enter";

    // Decode
    if(!data->hasText()) {
        kDebug() << "No text in drag item";
        return false;
    }

    // Get name
    QString name = data->text();

    // Get version
    Nepomuk::DataPP * dppcfg = new Nepomuk::DataPP(name);
    if(!dppcfg->isValid()) {
        KMessageBox::error(this, "Incorrect value was passed as DataPP name");
        return false;
    }

    QString version = dppcfg->dataPP()->pluginVersion();
    delete dppcfg;

    // Mark...

    // Add to model
    // Search for it
    QList<QTableWidgetItem*> res = this->findItems(name, Qt::MatchExactly);
    bool hasAny = false;
    // Names are stored at column 0, versions at column 1
    // Check that there is any name in res
    QTableWidgetItem * datapp_item = 0;
    foreach(QTableWidgetItem * _item, res) {
        // Check that there is only one item
        if(_item->column() == 0) {
            if(hasAny) {
                kError() << "There must be only one item for every DataPP";
            }
            hasAny = true;
            datapp_item = _item;
        }
    }
    if(!hasAny) {

        // If no version available then there is no such
        // Add new item
        QTableWidgetItem * _item = new QTableWidgetItem(name);
        _item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        setItem(this->rowCount(), 0, _item);

        _item = new QTableWidgetItem(version);
        _item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        setItem(this->rowCount(), 1, _item);
    } else {
        // Update version
        // Get item for version ( we have item for name )
        int row = datapp_item->row();
        item(row, 1)->setText(version);
    }

    return true;

}

Qt::DropActions ExaminedWidget::supportedDropActions() const
{
    kDebug() << "Supported";
    return Qt::CopyAction | Qt::IgnoreAction;
}

#endif

void ExaminedWidget::dropEvent(QDropEvent *event)
{
    // Allways mark DataPP as examined. But add it to model only if it is not already in.
    // If it is, then update it's version



    kDebug() << "Enter. Action: " << event->proposedAction();
    if(event->proposedAction() == Qt::IgnoreAction) {
        event->accept();
        return ;
    }


    const QMimeData * data = event->mimeData();



    // Decode
    if(!data->hasText()) {
        kDebug() << "No text in drag item";
        event->accept();
        return;
    }

    // Get name
    //QByteArray encodedData = data->data("text/plain");
    //QDataStream stream(&encodedData, QIODevice::ReadOnly);
    //QStringList newItems;
    int rows = 0;


    /*
    while (!stream.atEnd()) {
         QString text;
         stream >> text;
         newItems << text;
         ++rows;
    }
    */

    QString name = data->text();
    //QString name;
    //stream >> name;

    // Get version
    Nepomuk::DataPP * dppcfg = Nepomuk::DataPPPool::dataPPById(name);
    if(!dppcfg or !dppcfg->isValid()) {
        KMessageBox::sorry(this, "No such DataPP or it is invalid:\n" + name);
        event->accept();
        return;
    }
    int version = dppcfg->executive()->version();

    kDebug() << "Name: " << name << " Version: " << version;

    // Mark...
    emit setExaminedDataPP(name, version);


    event->accept();
    return;
}

void ExaminedWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if((event->proposedAction() == Qt::IgnoreAction) or(event->proposedAction() == Qt::CopyAction))
        event->accept();
}

void ExaminedWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}
