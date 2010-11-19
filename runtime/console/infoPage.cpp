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

#include "infoPage.h"
#include "datapppool.h"
#include <QBrush>
#include <QColor>
#include <kdebug.h>
#include <QUrl>
#include <Nepomuk/Resource>
#include <KMessageBox>
#include "resourceservicedata.h"

namespace NW = Nepomuk::WebExtractor;

InfoPage::InfoPage(QWidget * parent):
    QWidget(parent)
{
    this->setupUi(this);

    // Set misc
    this->examinedHelpLabel->hide();

    // Set table widget of examined DataPP
    this->examinedDataPPWidget->setVerticalHeaderItem(0, new QTableWidgetItem("Name"));
    this->examinedDataPPWidget->setVerticalHeaderItem(1, new QTableWidgetItem("Version"));
    this->examinedDataPPWidget->setVerticalHeaderItem(2, new QTableWidgetItem("Last extraction date"));
    this->examinedDataPPWidget->viewport()->setAcceptDrops(true);
    connect(this->examinedDataPPWidget, SIGNAL(setExaminedDataPP(const QString &, int)), this, SLOT(onMarkedExamined(const QString &, int)));

    // Set service info widget
    this->serviceInfoTableWidget->setColumnCount(2);

    // Set uri
    connect(
            this->uriResourceInfo, SIGNAL(textChanged(const QString &)),
            this, SLOT(onUriResourceInfoChanged(const QString &)) 
            );


    // Set properties of resource info widget
    connect(this->resourcesInfoSelectWidget, SIGNAL(currentResourceChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)), this, SLOT(updateExaminedInfo(const Nepomuk::Resource &)));
    connect(this->resourcesInfoSelectWidget, SIGNAL(currentResourceChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)), this, SLOT(updateServiceInfo(const Nepomuk::Resource &)));
    connect(this->resourcesInfoSelectWidget, SIGNAL(currentResourceChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)), this, SLOT(updateUriResourceInfo(const Nepomuk::Resource &)));

    // Set buttons
    connect(this->clearExaminedButton, SIGNAL(clicked()), this, SLOT(onClearExamined()));
    connect(this->clearAllExaminedButton, SIGNAL(clicked()), this, SLOT(onClearAllExamined()));

}


void InfoPage::onMarkedExamined(const QString & name, int version)
{
    QUrl url = uriResourceInfo->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        rsd.setExaminedDataPPInfo(name, version);
        updateServiceInfo(res);
        updateExaminedInfo(res);

    } else {
        KMessageBox::sorry(this, "Please select any existing resource");
    }

}

void InfoPage::onClearExamined()
{
    // Get selected items
    QModelIndexList lst = examinedDataPPWidget->selectionModel()->selectedRows();
    if(lst.isEmpty())
        return;


    QUrl url = uriResourceInfo->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        // Convert it to the list
        foreach(const QModelIndex & idx, lst) {
            QString name = idx.data().toString();
            rsd.clearExaminedInfo(name);
        }
        this->onSelectedResourceChanged();
    }

}

void InfoPage::onClearAllExamined()
{
    QUrl url = uriResourceInfo->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        rsd.clearExaminedInfo();
        this->onSelectedResourceChanged();
    }
}

void InfoPage::onSelectedResourceChanged()
{
    QUrl url = uriResourceInfo->text();
    Nepomuk::Resource res(url);
    updateServiceInfo(res);
    updateExaminedInfo(res);
}

void InfoPage::updateExaminedInfo(const Nepomuk::Resource & current)
{
    // Clear examined info
    this->examinedDataPPWidget->clear();
    examinedDataPPWidget->setRowCount(0);

    NW::ResourceServiceData rsd(current);
    if(!rsd.isValid()) {
        kError() << "ResourceServiceData is invalid";
        return;
    }

    // Change examined widget

    QMap< QString, int > ed = rsd.examinedDataPPInfo();
    QMap< QString, QDateTime > edd = rsd.examinedDataPPDates();
    kDebug() << "Examined info: " << ed;
    int currentRow = 0;
    this->examinedDataPPWidget->setRowCount(ed.size());
    for(
        QMap< QString, int >::iterator it = ed.begin();
        it != ed.end();
        it++
    ) {
        QTableWidgetItem * item = new QTableWidgetItem(it.key());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString::number(it.value()));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 1, item);

        QDateTime d = edd[it.key()];
        item = new QTableWidgetItem(d.toString());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 2, item);

        currentRow++;
    }
    examinedDataPPWidget->resizeColumnsToContents();
}

void InfoPage::updateServiceInfo(const Nepomuk::Resource & current)
{
    // Clear service info
    this->serviceInfoTableWidget->clear();
    this->serviceInfoTableWidget->setRowCount(0);

    // Check
    NW::ResourceServiceData rsd(current);
    if(!rsd.isValid()) {
        kError() << "ResourceServiceData is invalid";
        return;
    }

    // Change service info
    QStringList lst = rsd.serviceInfoPropertiesNames();
    kDebug() << "Properties: " << lst;
    int siCurrentRow = 0;
    serviceInfoTableWidget->setRowCount(lst.size());
    foreach(const QString & name, lst) {
        QVariant val = rsd.property(name.toAscii().data());
        if(!val.isValid()) {
            kError() << "Property is invalid: " << name;
        }
        QTableWidgetItem * item = new QTableWidgetItem(name);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        serviceInfoTableWidget->setItem(siCurrentRow, 0, item);
        if(val.type() == QVariant::Url) {
            kDebug() << "url: " << val.toUrl();
            item = new QTableWidgetItem(val.toUrl().toString());
        } else if(val.canConvert(QVariant::String)) {
            item = new QTableWidgetItem(val.toString());
        } else {
            kError() << "Can not convert property to representable value";
            item = new QTableWidgetItem("Unrepresentable");
        }
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        serviceInfoTableWidget->setItem(siCurrentRow, 1, item);

        siCurrentRow++;
    }
    serviceInfoTableWidget->resizeColumnsToContents();
}

void InfoPage::onUriResourceInfoChanged(const QString & text)
{
    QUrl uri(text);
    Nepomuk::Resource res(uri);
    updateExaminedInfo(res);
    updateServiceInfo(res);
}

void InfoPage::updateUriResourceInfo(const Nepomuk::Resource & res)
{
    disconnect(
            this->uriResourceInfo, SIGNAL(textChanged(const QString &)),
            this, SLOT(onUriResourceInfoChanged(const QString &)) 
            );
    this->uriResourceInfo->setText(res.resourceUri().toString());
    connect(
            this->uriResourceInfo, SIGNAL(textChanged(const QString &)),
            this, SLOT(onUriResourceInfoChanged(const QString &)) 
            );
}
