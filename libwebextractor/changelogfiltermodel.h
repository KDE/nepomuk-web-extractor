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


#ifndef __nepomuk_chaneglogfiltermodel_h_
#define __nepomuk_chaneglogfiltermodel_h_


#include <Soprano/FilterModel>
#include <Soprano/Statement>
#include <QUrl>
#include <QSet>
#include <kdebug.h>
#include "changelogrecord.h"
#include "changelog.h"

#include "webextractor_export.h"

namespace Nepomuk
{
    namespace Sync
    {
        //template < typename ChangeLogType>
        //typedef ChangeLog ChangeLogType;
        class ChangeLogFilterModel : public Soprano::FilterModel
        {
            public:
                enum Action { Accept, Decline };

                ChangeLogFilterModel(ChangeLog * t, Soprano::Model * parent, const QSet<QUrl> &  targets = QSet<QUrl>(), Action defaultAction = Accept):
                    Soprano::FilterModel(parent) {
                    m_log = t;
                    m_targets = targets;
                    m_defaultAction = defaultAction;
                }

                void setLog(ChangeLog * t) {
                    m_log = t;
                }

                void addTarget(const QUrl & target) {
                    m_targets.insert(target);
		    kDebug() << "Targets are: " << m_targets;
                }

                ChangeLog * log() const {
                    return m_log;
                }

                QSet<QUrl> targets() const {
                    return m_targets;
                }

                virtual Soprano::Error::ErrorCode addStatement(const Soprano::Statement &statement) {
                    Soprano::Error::ErrorCode c = Soprano::FilterModel::addStatement(statement);

		    //kDebug() << "Add statement: " << statement;


                    // If log not set
                    if(!m_log) {
			//kDebug() << "Log not set";
                        return c;
		    }

                    if(c != Soprano::Error::ErrorNone)
                        return c;

                    // Add to log only if targets are empty( aka log everything ) or
                    // if subject, object or predicate is in targets
                    if(
                        (m_targets.isEmpty() and(m_defaultAction == Accept)) or
                        (m_targets.contains(statement.subject().uri())) or
                        (m_targets.contains(statement.object().uri()))
                    ) {
			//kDebug() << "Log statement: " << statement;
                        *m_log << ChangeLogRecord(statement);
                    }
                    return c;
                }

                virtual Soprano::Error::ErrorCode removeStatement(const Soprano::Statement &statement) {
                    Soprano::Error::ErrorCode c = Soprano::FilterModel::removeStatement(statement);

                    // If log not set
                    if(!m_log)
                        return c;

                    if(c != Soprano::Error::ErrorNone)
                        return c;

                    // Add to log
                    if(
                        (m_targets.isEmpty() and(m_defaultAction == Accept)) or
                        (m_targets.contains(statement.subject().uri())) or
                        (m_targets.contains(statement.object().uri()))
                    ) {
                        *m_log << ChangeLogRecord(QDateTime::currentDateTime(), false, statement);
                    }
                    /* m_log->add(
                        ChangeLogRecord(QDateTime::currentDateTime(), false, statement)
                        );*/
                    return c;
                }

            private:
                ChangeLog * m_log;
                QSet<QUrl> m_targets;
                Action m_defaultAction;

        };
    }
}

#endif

