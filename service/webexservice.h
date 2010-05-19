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
#ifndef _NEPOMUK_WEBEXTRCT_SERVICE_H_
#define _NEPOMUK_WEBEXTRCT_SERVICE_H_

#include <Nepomuk/Service>
#include <QtCore/QString>

namespace Nepomuk{

    class WebExtractorScheduler;

    class WebExtractorService : public Nepomuk::Service {
        //private
        Q_OBJECT;
	Q_CLASSINFO("D-Bus Interface", "org.kde.nepomuk.WebExtractorService");

        public:
            WebExtractorService(QObject * parent, const QList<QVariant> &);
	    ~WebExtractorService();

	private Q_SLOTS:
	    void finishInitialization();
	    // Debug purpos only
	    void __stop();

	public Q_SLOTS:
	    bool isIdle() const;
	    void setSuspended( bool );
	    bool isSuspended() const;
	    void reconfigure();


	private:
	    Nepomuk::WebExtractorScheduler* m_extractScheduler;

    };

};
#endif
