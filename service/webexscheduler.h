/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

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

#ifndef _NEPOMUK_WEBEXTRCT_SCH_H_
#define _NEPOMUK_WEBEXTRCT_SCH_H_
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>
#include <QtCore/QHash>
#include <QtCore/QSet>
#include "webextractor_settings.h"
#include "webexcatscheduler.h"

namespace Nepomuk {
    class WebExtractorScheduler:public QObject
    {
	Q_OBJECT;
	public:
	    WebExtractorScheduler(Nepomuk::WebExtractorSettings*, QObject * parent=0);
	    // Return true if initialized successfully
	    bool isInitialized() const { return m_success; };
	    void start();

	public :
	    bool isIdle() const;
	    void suspend();
	    void resume();
	    void reconfigure();
	    void stop();
	    bool isExtracting() const ;
	    bool isSuspended() const ;


	private Q_SLOTS:
	    void readConfig();
	    void categoryFinished(const QString &);
	    void launchNext();
	    bool addToQueue(const QString &);
	    // TODO Should reloading config occur
	    // void configChanged();
	private:
	    // Remove finished categories from m_launchedQueries
	    void clearLaunched() const;
	    bool m_success;
	    bool m_stopped;
	    int m_maxCatSimult;
	    // Amount of categories enabled. 
	    // Cached for perfomance
	    //int m_queries;
	    WebExtractorSettings * m_conf;
	    QHash<QString,QString> m_askQueries;
	    QHash<QString,QString> m_selectQueries;
	    QHash<QString,QTimer*> m_timers;
	    QHash<QString,WebExtractorCategoryScheduler*> m_categories;
	    mutable QHash<QString,WebExtractorCategoryScheduler*> m_launchedQueries;
	    QSignalMapper * m_sm;
	    QSignalMapper * m_timerSM;
	    QTimer * m_launchTimer;
	    // This is queu of categories that is ready to launch
	    // (has data they will work on)
	    QSet<QString> m_launchQueue;
    };
}

#endif
