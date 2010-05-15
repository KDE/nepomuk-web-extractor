/*
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
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
#include "webextractorserviceadaptor.h"
#include "webexservice.h"
#include "useractivitymonitor.h"
#include "webextractor_settings.h"
#include "webexscheduler.h"
#include "priority.h"
#include <QtCore/QTimer>
#include <KDebug>



Nepomuk::WebExtractorService::WebExtractorService( QObject * parent, const QList<QVariant> & ):
    Nepomuk::Service(parent,true)
{
    (void)new  WebExtractorServiceAdaptor(this); 
    //QTimer::singleShot( 2*60*1000, this, SLOT( finishInitialization() ) );
    // For debuging purpose use less timer interval
    QTimer::singleShot( 2, this, SLOT( finishInitialization() ) );

    // lower process priority - we do not want to spoil KDE usage
    // ==============================================================
    if ( !lowerPriority() )
        kDebug() << "Failed to lower priority.";
    if ( !lowerSchedulingPriority() )
        kDebug() << "Failed to lower scheduling priority.";
    if ( !lowerIOPriority() )
        kDebug() << "Failed to lower io priority.";


}

Nepomuk::WebExtractorService::~WebExtractorService()
{
    m_extractScheduler->stop();
    delete m_extractScheduler;
}

bool Nepomuk::WebExtractorService::isIdle() const
{
    return ( !m_extractScheduler->isExtracting() );
}
bool Nepomuk::WebExtractorService::isSuspended() const
{
    return ( m_extractScheduler->isSuspended() );
}

void Nepomuk::WebExtractorService::reconfigure() 
{
    m_extractScheduler->reconfigure();
}

void Nepomuk::WebExtractorService::setSuspended(bool suspend)
{
    if ( suspend ) {
        m_extractScheduler->suspend();
    }
    else {
        m_extractScheduler->resume();
    }
}

void Nepomuk::WebExtractorService::finishInitialization()
{

    Nepomuk::WebExtractorConfig * config = new Nepomuk::WebExtractorConfig();
    m_extractScheduler = new Nepomuk::WebExtractorScheduler(config,this);
    
    if ( !m_extractScheduler->isInitialized() ) {
	// Initialization failed
	setServiceInitialized(false);
	return;
    }
    setServiceInitialized(true);
    //m_extractScheduler = new Nepomuk::WebExtractorCategoryScheduler("select ?r where { ?r nao:hasTag ?tag }", this);
    m_extractScheduler->start();
    // TODO Using UserActivityMonitor cause segfault in Qt Code (
    // Program received signal SIGSEGV, Segmentation fault.
    // 0x00007ffff5ad80f6 in QCursor::pos () at kernel/qcursor_x11.cpp:153
    // 153         Display* dpy = X11->display;
    // Fix it or force somebody to fix it
    UserActivityMonitor* userActivityMonitor = new UserActivityMonitor( this );
    /*
    connect( userActivityMonitor, SIGNAL( userActive( bool ) ),
             m_extractScheduler, SLOT( setReducedExtractingSpeed( bool ) ) );
    userActivityMonitor->start();
    ;
    */
}


#include <kpluginfactory.h>
#include <kpluginloader.h>
NEPOMUK_EXPORT_SERVICE(Nepomuk::WebExtractorService, "webextractorServiceservice");
