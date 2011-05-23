/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "changelogwidget.h"
#include "ui_changeLogForm.h"
#include "changelog.h"
#include "changelogrecord.h"

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
