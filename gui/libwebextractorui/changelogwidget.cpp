#include "changelogwidget.h"
#include "ui_changeLogForm.h"
#include <nepomuk/changelog.h>

namespace NS = Nepomuk::Sync;

class ChangeLogWidget::Private
{
    public:
	Ui_changeLogForm * ui;
	NS::ChangeLog log;
};

ChangeLogWidget::ChangeLogWidget(QWidget * parent):
    QWidget(parent),
    d( new Private() )
{
    d->ui = new Ui_changeLogForm();
    d->ui->setupUi(this);
}

ChangeLogWidget::ChangeLogWidget( const NS::ChangeLog & log,QWidget * parent):
    QWidget(parent),
    d( new Private() )
{
    d->ui = new Ui_changeLogForm();
    d->ui->setupUi(this);
    setLog(log);
}

ChangeLogWidget::~ChangeLogWidget()
{
    delete d->ui;
    delete d;
}
void ChangeLogWidget::setLog( const NS::ChangeLog & log)
{

    // Fill form
    QString text;
    QTextStream stream(&text);
    stream << log;
    d->ui->changeLogBrowser->setPlainText(text);

    d->log = log;
}

void ChangeLogWidget::clear()
{
    setLog( NS::ChangeLog() );
}
