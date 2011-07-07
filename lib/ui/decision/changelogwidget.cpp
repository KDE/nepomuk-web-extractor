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
#include <nepomuk/simpleresourcegraph.h>
#include <nepomuk/simpleresource.h>


class ChangeLogWidget::Private
{
    public:
        Ui_changeLogForm * ui;
        Nepomuk::SimpleResourceGraph log;
};

ChangeLogWidget::ChangeLogWidget(QWidget * parent):
    QWidget(parent),
    d(new Private())
{
    d->ui = new Ui_changeLogForm();
    d->ui->setupUi(this);
}

ChangeLogWidget::ChangeLogWidget(const Nepomuk::SimpleResourceGraph & log, QWidget * parent):
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
void ChangeLogWidget::setLog(const Nepomuk::SimpleResourceGraph & log)
{

    // Fill form
    QString text;
    QTextStream stream(&text);

    foreach( const Nepomuk::SimpleResource & resource, log.toList() )
    {
        // Print subject
        QUrl subject = resource.uri();
        if ( subject.scheme().startsWith("_") ) {
            // It is blank node
            stream << subject.host() << subject.path();
        }
        else {
            stream << "<a HREF=\"" << subject << "\">" <<
                   subject.host() <<
                   subject.path() <<
                   "</a>";
        }

        stream << "  ";

        Nepomuk::PropertyHash properties = resource.properties();
        Nepomuk::PropertyHash::const_iterator cit = properties.begin(),
            cit_end = properties.end();

        for(; cit != cit_end; ++cit)
        {
            stream << "&nbsp;&nbsp;&nbsp;";
            // Property
            QUrl property = cit.key();
            stream << "<a HREF=\"" << property << "\">" <<
                   property.fragment() <<
                   "</a>";

            stream << "  ";

            // Value
            if ( cit.value().type() == QVariant::Url ) {
                QUrl value = cit.value().toUrl();

                if ( value.scheme().startsWith("_") ) {
                    // It is blank node
                    stream << value.host() << value.path();
                }
                else {
                    stream << "<a HREF=\"" << value << "\">" <<
                           value.host() <<
                           value.path() <<
                           value.fragment() << 
                           "</a>";
                }
            }
            else {
                stream << cit.value().toString();
            }

            stream << "<br><hr>";
        }

    }
#if 0
    foreach(const Soprano::Statement & rec, resource.toStatementList()) {

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
#endif
    //stream << log;
    d->ui->changeLogBrowser->setHtml(text);

    d->log = log;
}

void ChangeLogWidget::clear()
{
    setLog(Nepomuk::SimpleResourceGraph());
}
