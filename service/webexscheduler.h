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
#include <QtCore/QCache>
#include <QtCore/QSet>
#include "webextractor_settings.h"
#include "webexcatscheduler.h"

namespace Nepomuk {
    /*! \brief This class manages categories
     * 
     * Each category in Web Extractor Service has a scheduler. This class manages all
     * these schedulers, launch them when it is necessary, stop them, reconfigure,
     * restart, etc.
     */
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
	    /*! Stops executing of all currently running schedulers and go to the 
	     * 'suspended' state
	     */
	    void suspend();
	    
	    /*! Resume all suspended schedulers and go to 'resumed' state
	     */
	    void resume();

	    /*! \brief  Restarct all schedulers with new settings 
	     *
	     * Stop(), then re-read config file, then start()
	     */
	    void reconfigure();

	    /*! Stop all schedulers
	     */
	    void stop();

	    /*! Return true if any launched scheduler is extracing
	     */
	    bool isExtracting() const ;

	    /*! Return true if all schuduler are suspended
	     */
	    bool isSuspended() const ;


	private Q_SLOTS:
	    /*! Parse settings, fill m_askQueries and m_selectQueries
	     */ 
	    void readConfig();
	    
	    /*! \brief This slot is launched when any scheduler has finished
	     *
	     * 	This method only restart timer for this category
	     */
	    void categoryFinished(const QString &);

	    /*! \brief This method makes an attempt to launch new portion of schedulers 
	     * 
	     * It takes candidates from launch queue. If there are not any available 
	     * slots for new scheduler, this method will 
	     * silently return.
	     * Scheduler that have been already launched will not be restarted
	     * This slot is called by timeout
	     */
	    void launchNext();
	    
	    /*! \brief Add category to launch queue. 
	     *
	     * If this category is already in this queue, then queue wil stay unchanged
	     * \param name Name of the category
	     */
	    bool addToQueue(const QString & name);
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
	    
	    // Pointer to object that store all settings of web extractor
	    WebExtractorSettings * m_conf;
	    
	    // Cached ask and select queries
	    QHash<QString,QString> m_askQueries;
	    QHash<QString,QString> m_selectQueries;

	    // Timers for every category. When timer signals, category
	    // is added to launch queue
	    QHash<QString,QTimer*> m_timers;

	    // Schedulers for every category
	    QHash<QString,WebExtractorCategoryScheduler*> m_categories;

	    // Hash of already launched queries
	    mutable QHash<QString,WebExtractorCategoryScheduler*> m_launchedQueries;
	    QSignalMapper * m_sm;
	    QSignalMapper * m_timerSM;

	    // Timer for launching category. When timer signals, next portion of 
	    // schedulers is started
	    QTimer * m_launchTimer;

	    // This is queu of categories that is ready to launch
	    // (has data they will work on)
	    QSet<QString> m_launchQueue;
    };
}

#endif
