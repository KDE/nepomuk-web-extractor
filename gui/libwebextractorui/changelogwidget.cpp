#include "changelogwidget.h"
#include "ui_changeLogForm.h"
#include <nepomuk/changelog.h>
#include <nepomuk/changelogrecord.h>

namespace NS = Nepomuk::Sync;

class ChangeLogWidget::Private
{
    public:
        Ui_changeLogForm * ui;
        NS::ChangeLog log;
};

ChangeLogWidget::ChangeLogWidget(QWidget * parent):
    QWidget(parent),
    d(new Private())
{
    d->ui = new Ui_changeLogForm();
    d->ui->setupUi(this);
}

ChangeLogWidget::ChangeLogWidget(const NS::ChangeLog & log, QWidget * parent):
    QWidget(parent),
    d(new Private())
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
void ChangeLogWidget::setLog(const NS::ChangeLog & log)
{

    // Fill form
    QString text;
    QTextStream stream(&text);

    foreach(const NS::ChangeLogRecord & rec, log.toList()) {

        Soprano::Node subject = rec.subject();
        if(subject.isResource())  {
            stream << "<a HREF=\"" << subject.uri() << "\">" <<
                   subject.uri().host() <<
                   subject.uri().path() <<
                   "</a>";
        } else {
            stream << subject.literal().toString();
        }

        stream << " ";


        Soprano::Node predicate = rec.predicate().uri();
        if(predicate.isResource()) {
            stream << "<a HREF=\"" << predicate.uri() << "\">";
            if(predicate.uri().fragment().isEmpty())
                stream << "Unknown property";
            else
                stream << predicate.uri().fragment();
            stream << "</a>";
        }

        stream << " ";

        Soprano::Node object = rec.object();
        if(object.isResource())  {
            stream << "<a HREF=\"" << object.uri() << "\">" <<
                   object.uri().host() <<
                   object.uri().path() <<
                   "</a>";
        } else {
            stream << object.literal().toString();
        }

        stream << "<br><hr>";



    }
    //stream << log;
    d->ui->changeLogBrowser->setHtml(text);

    d->log = log;
}

void ChangeLogWidget::clear()
{
    setLog(NS::ChangeLog());
}
