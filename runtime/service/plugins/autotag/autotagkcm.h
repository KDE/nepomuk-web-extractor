/*
   Copyright (C) 2010 by Artem Serebriyskiy <v.for.vandal@gmail.com>

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


#ifndef __webextractor_plugin_autotag_kcm_h_
#define __webextractor_plugin_autotag_kcm_h_

#include "webextractor_kcm.h"

namespace Nepomuk {
    class AutotagKCM : public WebExtractorPluginKCM
    {
	Q_OBJECT;

	private:
	    void connectAll();
	    void disconnectAll();



	public:
            AutotagKCM(QWidget *, const QVariantList &args);
	    virtual ~AutotagKCM();
	    virtual void load();
	    virtual void save();
	    virtual void defaults();

	private:
	    AutotagKCM( const AutotagKCM & ); // No realization
	    void operator=( const AutotagKCM &); // No realization;
	    class Private;
	    Private * d;
    };
}
#endif

