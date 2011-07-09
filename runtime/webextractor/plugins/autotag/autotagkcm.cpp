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

#include "autotagkcm.h"
#include "autotagplugin_config.h"
#include "datapp.h"
#include "ui_kcm.h"

#include <KConfigGroup>
#include <KPluginFactory>

K_PLUGIN_FACTORY(AutotagKCMFactory,
                 registerPlugin<Nepomuk::AutotagKCM>();
                )
K_EXPORT_PLUGIN(AutotagKCMFactory("autotag_kcm"))

class Nepomuk::AutotagKCM::Private 
{
    public:
	Private();
	Ui_Form * ui;
};

Nepomuk::AutotagKCM::Private::Private():
    ui(0)
{;}


Nepomuk::AutotagKCM::AutotagKCM( QWidget * parent, const QVariantList & args ):
    WebExtractorPluginKCM(AutotagKCMFactory::componentData(), parent, args ),
    d(new Private())
{
    d->ui = new Ui_Form();
    d->ui->setupUi(this);
}

void Nepomuk::AutotagKCM::load()
{
    KConfigGroup cfg = currentConfig();
    if (!cfg.isValid())
	return;

    // Get group
    KConfigGroup grp = cfg.group(AUTOTAG_CONFIG_GROUP);
    // Read params
    QString regexpString = grp.readEntry(AUTOTAG_CONFIG_REGEXP_KEY, QString());
    QString tag = grp.readEntry(AUTOTAG_CONFIG_TAG_KEY, QString());
    QString tagDescription = grp.readEntry(AUTOTAG_CONFIG_DESCRIPTION_KEY, QString());

    // Set them in config
    disconnectAll();
    this->d->ui->regexpLineEdit->setText(regexpString);
    this->d->ui->tagLineEdit->setText(tag);
    this->d->ui->descriptionTextEdit->setPlainText(tagDescription);
    connectAll();
     
}

void Nepomuk::AutotagKCM::save()
{
    KConfigGroup cfg = currentConfig();
    if (!cfg.isValid())
	return;

    // Get group
    KConfigGroup grp = cfg.group(AUTOTAG_CONFIG_GROUP);
    // Read params
    QString regexpString = d->ui->regexpLineEdit->text();
    QString tag = d->ui->tagLineEdit->text();
    QString description = d->ui->descriptionTextEdit->toPlainText();

    grp.writeEntry(AUTOTAG_CONFIG_REGEXP_KEY, regexpString);
    grp.writeEntry(AUTOTAG_CONFIG_TAG_KEY, tag);
    grp.writeEntry(AUTOTAG_CONFIG_DESCRIPTION_KEY,description);

    cfg.sync();
}

void Nepomuk::AutotagKCM::defaults()
{
    KConfigGroup cfg = currentConfig();
    if (!cfg.isValid()) {
	return;
    }

    // Get group
    KConfigGroup grp = cfg.group(AUTOTAG_CONFIG_GROUP);
    // Read params
    QString regexpString = ".*";
    QString tag = "autotag";

    // Write to file and to KCM
    disconnectAll();
    grp.writeEntry(AUTOTAG_CONFIG_REGEXP_KEY, regexpString);
    grp.writeEntry(AUTOTAG_CONFIG_TAG_KEY, tag);
    grp.writeEntry(AUTOTAG_CONFIG_DESCRIPTION_KEY,QString());
    this->d->ui->regexpLineEdit->setText(regexpString);
    this->d->ui->tagLineEdit->setText(tag);
    this->d->ui->descriptionTextEdit->setPlainText(QString());
    connectAll();

    cfg.sync();
}

void Nepomuk::AutotagKCM::connectAll()
{
    connect( this->d->ui->regexpLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT( changed() ));
    connect( this->d->ui->tagLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT( changed() ));
    connect( this->d->ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT( changed() ));
}

void Nepomuk::AutotagKCM::disconnectAll()
{
    disconnect( this->d->ui->regexpLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT( changed() ));
    disconnect( this->d->ui->tagLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT( changed() ));
    disconnect( this->d->ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT( changed() ));
}

Nepomuk::AutotagKCM::~AutotagKCM()
{
    delete d->ui;
    delete d;
}
