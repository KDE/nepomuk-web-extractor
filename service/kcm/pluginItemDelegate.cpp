#include "pluginItemDelegate.h"
#include <KDoubleNumInput>
#include <QPainter>

QList<QWidget*> CategoryPluginItemDelegate::createItemWidgets() const
{
    return QList<QWidget*>() << new KDoubleNumInput();
}
void CategoryPluginItemDelegate::updateItemWidgets(
		    const QList<QWidget*> widgets,
		    const QStyleOptionViewItem &option,
		    const QPersistentModelIndex &index
		    ) const
{
    KDoubleNumInput * coff = qobject_cast<KDoubleNumInput*>( widgets.at(0) );
    coff->setDecimals(4);
    coff->setMinimum(0.0001);
    coff->setMaximum(10.0);
    coff->setSingleStep(0.0001);
    coff->setLabel("Scale cofficient");
    coff->setSliderEnabled(true);
    coff->setValue( index.data(Qt::UserRole).toDouble() );
}

void CategoryPluginItemDelegate::paint(
      QPainter *painter,
      const QStyleOptionViewItem &option,
      const QModelIndex &index
      ) const
{
 if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
        }
 
        painter->save();
 
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(option.palette.highlightedText().color()));
        }
 
/*
        painter->drawText(option.fontMetrics.height() + option.rect.left(),
                          option.fontMetrics.height() * 3 + 
                          Canvas::sizeHint(option, index).height() +
                          option.rect.top(),
                          QString("This is the index in row number ") + 
                          QString::number(index.row() + 1));
 */
        painter->restore();
 
        KWidgetItemDelegate::paintWidgets(painter, option, index);
}
