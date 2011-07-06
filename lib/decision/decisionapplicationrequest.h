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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_APPLICATION_REQUEST_H_

#include "decision_export.h"
#include "decision.h"
#include <QObject>
#include <QHash>
#include <QSet>
#include <Soprano/Node> // for qHash(QUrl)
#include <QSharedPointer>

namespace Nepomuk
{
    namespace Decision
    {
        class DecisionApplicationRequestPrivate;
        /*! \brief This class is used for applying Decision
         * The purpose of this class is to apply changes that were introduced
         * in Decision to the target resource.
         */
        class DECISION_EXPORT DecisionApplicationRequest : public QObject
        {
                Q_OBJECT;
            public Q_SLOTS:
                bool apply();
            public:
                ~DecisionApplicationRequest();
                /*! \brief Return the list of the url of the main Decision resources
                 * Only resources that are necessary for this request will be returned
                 * \return List of the url of the main Decision resources in the Decision
                 * storage model that are used in this request.
                 */
                QSet<QUrl> targetResources() const;
                KJob * applyJob();
                friend class Decision;
            Q_SIGNALS:
                void finished(KJob *);
            private:
                DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, QObject * parent = 0);
                // Disable assignement and copy constructor
                DecisionApplicationRequest(const DecisionApplicationRequest & rhs);
                DecisionApplicationRequest & operator=(const DecisionApplicationRequest & rhs);
            protected:
                DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, DecisionApplicationRequestPrivate & p, QObject * parent = 0);
            private:
                DecisionApplicationRequestPrivate * d_ptr;
                Q_DECLARE_PRIVATE(DecisionApplicationRequest);

        };
    }
}

#endif

