/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (C) 2008 by Dario Freddi <drf@kde.org>                      

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

#include "main.h"
#include <KAboutData>
#include <klocalizedstring.h>
#include <KPluginFactory>

K_PLUGIN_FACTORY(WebExtractorKCMFactory,
                 registerPlugin<WebExtractorKCM>();
                )
K_EXPORT_PLUGIN(WebExtractorKCMFactory("kcmwebextractor"))

WebExtractorKCM::WebExtractorKCM(QWidget * parent, const QVariantList &args):
        KCModule(WebExtractorKCMFactory::componentData(), parent)
{
    KAboutData *about = new KAboutData(
            "kcm_webextractor", 0, ki18n("Web Extractor Configuration Module"),
            KDE_VERSION_STRING, KLocalizedString(), KAboutData::License_GPL,
            ki18n("Copyright 2010 Serebriyskiy Artem\nCopyright 2008-2010 Sebastian Trueg"));
    about->addAuthor(ki18n("Serebriyskiy Artem"), KLocalizedString(), "v.for.vandal@gmail.com");
    setAboutData(about);

    m_config = new Nepomuk::WebExtractorConfig();

    m_configWidget = new ConfigWidget(m_config, this);
    connect(m_configWidget, SIGNAL(changed(bool)), this, SIGNAL(changed(bool)));

    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->addWidget(m_configWidget);
}

void WebExtractorKCM::load()
{
    m_configWidget->load();
}
void WebExtractorKCM::save()
{
    m_configWidget->save();
    m_config->writeConfig();
}
void WebExtractorKCM::defaults()
{
    m_configWidget->defaults();
}
