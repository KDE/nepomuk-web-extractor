/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at kde.org>

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

#ifndef _NEPOMUK_WEBEXTRCT_CORE_H_
#define _NEPOMUK_WEBEXTRCT_CORE_H_


namespace Nepomuk{
    class WebExtractorScheduler : public QThread
    {
	Q_OBJECT;

	public:
	    WebExtractorScheduler(QObject * parent);
	    ~WebExtractorScheduler();

	    bool isSuspended() const;
	    bool isIndexing() const;

	    enum IndexingSpeed {
		/**
		 * Index at full speed, i.e. do not use any artificial
		 * delays.
		 *
		 * This is the mode used if the user is "away".
		 */
		FullSpeed = 0,

		/**
		 * Reduce the indexing speed mildly. This is the normal
		 * mode used while the user works. The indexer uses small
		 * delay between indexing two files in order to keep the
		 * load on CPU and IO down.
		 */
		ReducedSpeed,

		/**
		 * Like ReducedSpeed delays are used but they are much longer
		 * to get even less CPU and IO load. This mode is used for the
		 * first 2 minutes after startup to give the KDE session manager
		 * time to start up the KDE session rapidly.
		 */
		SnailPace
	    };

	    IndexingSpeed currentSpeed() const { return m_speed; }
	    public Q_SLOTS:
		void suspend();
		void resume();
		void stop();
		void restart();

		void setIndexingSpeed( IndexingSpeed speed );

		/**
		 * A convinience slot which calls setIndexingSpeed
		 * with either FullSpeed or ReducedSpeed, based on the
		 * value of \p reduced.
		 */
		void setReducedIndexingSpeed( bool reduced = false );

		void setSuspended( bool );

		bool m_suspended;
		bool m_stopped;
		bool m_indexing;

		QMutex m_resumeStopMutex;
		QWaitCondition m_resumeStopWc;

		IndexingSpeed m_speed;
    }

#endif
