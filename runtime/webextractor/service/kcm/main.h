/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#ifndef _KCM_WEBEXTRCT
#define _KCM_WEBEXTRCT
#include <kcmodule.h>
#include "webextractor_config.h"
#include "ConfigWidget.h"
//#include "


class WebExtractorKCM : public KCModule
{
    public:
	WebExtractorKCM(QWidget * parent, const QVariantList &args);
	virtual void load();
        void save();
        void defaults();

    private:
	ConfigWidget * m_configWidget;
	Nepomuk::WebExtractorConfig* m_config;
	QVBoxLayout *m_layout;
}; 

#endif
