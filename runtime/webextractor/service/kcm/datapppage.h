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


#ifndef DATAPPPAGE_H
#define DATAPPPAGE_H

#include <QWidget>
#include <QPersistentModelIndex>

#include "ui_datappPage.h"
#include "webexconfigbase.h"
#include "webextractor_kcm.h"


class DataPPPage : public QWidget, private Ui_datappPage
{
    Q_OBJECT

    public:
	DataPPPage(QWidget *parent = 0 );
	~DataPPPage();

    public Q_SLOTS:
        virtual void load();
        void save();
        void defaults();
        void emitChanged();
        void saveDataPP();
        /*! \brief Load given DataPP to widget.
         * This function will not ask anything - it will simply remove old config
         * and load new one
         */
        void loadDataPP(const QString & );
        /*! \brief This function will switch to given DataPP 
         * It will correctly handle the situation when changes to the current DataPP
         * weren't saved - it will ask user what to do - save,discard,or cancel
         * Function returns true if DataPP was changed, and false otherwise( it is 
         * possible in situations when user cancel switching)
         */
        bool switchDataPP(const QString &);
        bool isDataPP(const QModelIndex&);
        void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
        void dataPPSettingsChanged(bool);
        void newDataPP();
        void removeDataPP();
    Q_SIGNALS:
        void changed(bool ch);
    private:
        QPersistentModelIndex previousDataPP;
        /* KCM subsection */
        Nepomuk::WebExtractorPluginKCM::Ptr m_currentKcm;
        bool m_kcmChanged;
};

#endif
