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

#ifndef __webexgui_console_laungh_page_h_
#define __webexgui_console_laungh_page_h_

#include "ui_launchPage.h"
#include <webextractor/parameters.h>
#include <QWidget>
#include <QQueue>
#include <Nepomuk/Resource>
#include "decisionlist.h"

namespace Nepomuk {
    namespace WebExtractor {
        class ResourceAnalyzer;
    }
}
namespace Soprano {
    class Backend;
}

class QThread;
class KTempDir;

class LaunchPage : public QWidget, public Ui_launchPage
{
    Q_OBJECT;
    public:
        LaunchPage(
                const QString & uri = QString(),
                const QStringList & datapps = QStringList(),
                bool autostart = false,
                QWidget * parent = 0
                );
        ~LaunchPage();
    private Q_SLOTS:
        void startExtracting();
        void extractingFinished();
        void onApplyDecision();

        //void updateIdentificationInfo();
        void updateDecisionsInfo();
        void onCurrentDecisionChanged(const QListWidgetItem *,const QListWidgetItem*);
    private:
        enum SelectTargetsTabs { LineEditSelect = 0, GuiSelect = 1, CategorySelect = 2 };
        enum AnalyzationType { Single, Set, Query };
        void cleanAfterAnalyzing();
        void setButtonApply();
        void setButtonAbort();

        QThread * workThread;
        Nepomuk::WebExtractor::ResourceAnalyzer * m_currentAnalyzer;
        // Store extract paramters ptr for debugging proposes
        Nepomuk::WebExtractor::ExtractParameters m_par;
        // This variable store what type of analization - single, set of 
        // resources or category we currently perform. 
        AnalyzationType m_currentAnalizationType;
        QQueue<Nepomuk::Resource> m_toAnalyze;
        Nepomuk::Decision::DecisionList m_result;
        bool m_abort;

        // Map that store current list of the Decisions. Key is the url.
        //QMap< QUrl, Nepomuk::Decision::Decision > m_decisionMap;
        //QHash< QUrl, Nepomuk::Decision::DecisionApplicationRequest* > m_requestsHash;

};
#endif

