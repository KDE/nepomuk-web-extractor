/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RESOURCEANALYZERTHREAD_H
#define RESOURCEANALYZERTHREAD_H

#include <QThread>
#include <QQueue>
#include <QUrl>
#include <Nepomuk/Resource>

#include "decision.h"
#include "decisionlist.h"


namespace Nepomuk {
class Resource;
}
class Category;

class ResourceAnalyzerThread : public QThread
{
    Q_OBJECT

public:
    ResourceAnalyzerThread(QObject *parent = 0);
    void run();

public Q_SLOTS:
    void start(Category* cat);

private Q_SLOTS:
    void slotAnalyzingFinished();

Q_SIGNALS:
    void infoMessage(const QString& s);
    void analyzingResource(const Nepomuk::Resource&);
    void newDecisions(const Nepomuk::Decision::DecisionList&);

private:
    Category* m_category;
    QQueue<QUrl> m_resourceQueue;
};

#endif // RESOURCEANALYZERTHREAD_H
