/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (c) 2009 Sebastian Trueg <trueg@kde.org>

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
#include "pluginItemDelegate.h"
#include <KDoubleNumInput>
#include <QDoubleSpinBox>
#include <QPainter>
#include <KDebug>
#include <QTextDocument>
#include "ui_pluginDelegate.h"

CategoryPluginItemDelegate::CategoryPluginItemDelegate(QAbstractItemView *itemView, QObject * parent ):
    KWidgetItemDelegate(itemView, parent)
{
    ui = new Ui_pluginDelegate();
    //ui->description->setVisible(false);
}
CategoryPluginItemDelegate::~CategoryPluginItemDelegate()
{
    delete ui;
}

QList<QWidget*> CategoryPluginItemDelegate::createItemWidgets() const 
{
    //KDoubleNumInput * coff = new KDoubleNumInput();
    /*
    QDoubleSpinBox * coff = new QDoubleSpinBox();
    */
    QWidget * w = new QWidget();
    ui->setupUi(w);
    m_sizeHint = w->sizeHint();
    // Fix it
    if (m_sizeHint.width() < w->minimumSize().width())
	m_sizeHint.setWidth(w->minimumSize().width());
    if (m_sizeHint.height() < w->minimumSize().height())
	m_sizeHint.setHeight(w->minimumSize().height());

    connect(ui->descr_button, SIGNAL(clicked()), this, SLOT( descriptionToggled()));
    connect(ui->scaleCoff, SIGNAL(valueChanged(double)), this, SLOT( coffChanged(double)));


    kDebug() << "Size hint "<< m_sizeHint;
    setBlockedEventTypes( w,
                          QList<QEvent::Type>()
                          << QEvent::MouseButtonPress
                          << QEvent::MouseButtonRelease
                          << QEvent::MouseButtonDblClick
                          << QEvent::KeyPress
                          << QEvent::KeyRelease );

    return QList<QWidget*>() << w;
}
void CategoryPluginItemDelegate::updateItemWidgets(
		    const QList<QWidget*> widgets,
		    const QStyleOptionViewItem &option,
		    const QPersistentModelIndex &index
		    ) const
{
    QWidget * w =    widgets.at(0);
    w->resize(m_sizeHint);
    
    if ( index.isValid()) {
	if ( index.data(IsClassRole).toBool()) {
		w->setHidden(true);
	}
	else {
	//ui->description->setVisible( index.data(DescriptionToggledRole).toBool());
	ui->scaleCoff->setValue(index.data(CoffRole).toDouble());
	//ui->descr_button->setDown(index.data(DescriptionToggledRole).toBool());
	}
    }
    w->move( /*option.rect.left()*/0, 0 /*option.rect.height()*/);
#if 0
    //KDoubleNumInput * coff = qobject_cast<KDoubleNumInput*>( widgets.at(0) );
    QDoubleSpinBox * coff = qobject_cast<QDoubleSpinBox*>( widgets.at(0) );
    coff->setDecimals(4);
    coff->setMinimum(0.0001);
    coff->setMaximum(10.0);
    coff->setSingleStep(0.0001);
    //coff->setLabel("Scale cofficient");
    //coff->setSliderEnabled(false);
    //coff->setValue( index.data(Qt::UserRole).toDouble() );
    //coff->setVisible(true);
    QSize numInputSize = coff->sizeHint();
    coff->resize( numInputSize );
    //coff->move( option.rect.right() - 10 - ( 2*numInputSize.width() ),
    //                   ( option.rect.height() - numInputSize.height() )/2 );
    coff->move( option.rect.left() ,   
                       ( option.rect.height() - numInputSize.height() )/2 );
#endif
}

void CategoryPluginItemDelegate::paint(
      QPainter *painter,
      const QStyleOptionViewItem &option,
      const QModelIndex &index
      ) const
{
    /*
    QTextDocument doc;
    doc.setHtml( QString("<html><body><div style=\"color:%1\">%2</div></body></html>")
                     .arg( option.state & QStyle::State_Selected
                           ? option.palette.color( QPalette::HighlightedText ).name()
                           : option.palette.color( QPalette::Text ).name() )
                 .arg( "Damn It" ) );
		 */

    painter->save();

    if ( option.state & QStyle::State_Selected ) {
        painter->fillRect( option.rect, option.palette.highlight() );
    }

    if ( index.data(IsClassRole).toBool()) {
        painter->drawText(option.fontMetrics.height() + option.rect.left(),
                          //option.fontMetrics.height() * 3 + 
                          //Canvas::sizeHint(option, index).height() +
                          option.rect.top(),
                          QString("Category:") + 
                          index.data(Qt::DisplayRole).toString());
    }
    else {
	if ( index.data(DescriptionToggledRole).toBool()) {
	    QTextDocument doc;
	    doc.setPlainText("Description");
	    painter->translate( option.rect.topLeft() + QPoint(0,m_sizeHint.height()) );
	    doc.drawContents( painter );
	}
    }
 
#if 0
 
    /*
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(option.palette.highlightedText().color()));
        }
	*/
 
/*
        painter->drawText(option.fontMetrics.height() + option.rect.left(),
                          option.fontMetrics.height() * 3 + 
                          Canvas::sizeHint(option, index).height() +
                          option.rect.top(),
                          QString("This is the index in row number ") + 
                          QString::number(index.row() + 1));
 */
#endif
        painter->restore();
       // KWidgetItemDelegate::paintWidgets(painter, option, index);
}
QSize CategoryPluginItemDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&index ) const
{
	QTextDocument doc;
	doc.setPlainText("Description");
	if ( index.data(DescriptionToggledRole).toBool() ) {
	    return m_sizeHint + QSize(0, doc.size().toSize().height());
//	    return m_sizeHint;
	}
	else {
	    return m_sizeHint;
	}
    //return m_sizeHint; 
   // return QSize(1000,200); 
}
void CategoryPluginItemDelegate::descriptionToggled()
{
    kDebug() << "Description button pressed";
    itemView()->model()->setData(
		    focusedIndex(),
		    DescriptionToggledRole,
		    !focusedIndex().data(DescriptionToggledRole).toBool()
		    );
}

void CategoryPluginItemDelegate::coffChanged(double val)
{
    kDebug() << "Coff changed";
    itemView()->model()->setData(focusedIndex(), CoffRole, val);
}
