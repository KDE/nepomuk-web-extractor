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


#include "resourceanalyzerfactory.h"
#include <KDebug>

Nepomuk::WebExtractor::ResourceAnalyzerFactory::ResourceAnalyzerFactory(
	const DataPPKeeper & dataPPKeeper, 
	ResourceAnalyzer::LaunchPolitics launchPolitics,
	DecisionList::MergePolitics mergePolitics,
	unsigned int step,
	double acrit,
	double ucrit,
	QObject * parent
	):
    QObject(parent),
    dataPPKeeper(dataPPKeeper),
    m_step(step),
    m_launchPolitics(launchPolitics),
    m_mergePolitics(mergePolitics)
{
    if (m_step < 1)
       m_step = 1;

    if (ucrit < 0)
	ucrit = 0;
    else if ( ucrit > 1)
	ucrit = 0.99;
    m_ucrit = ucrit;

    if (acrit < 0)
	acrit = 0;
    else if ( acrit > 1)
	acrit = 1;
    m_acrit = acrit;
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer()
{
    DecisionFactory * fct = new DecisionFactory();
    fct->setThreshold(m_ucrit);
    return new Nepomuk::WebExtractor::ResourceAnalyzer(
	    dataPPKeeper,
	    fct,
	    this->m_mergePolitics,
	    this->m_launchPolitics,
	    m_acrit,
	    m_ucrit,
	    this->m_step
	    );
}

void Nepomuk::WebExtractor::ResourceAnalyzerFactory::deleteAnalyzer(Nepomuk::WebExtractor::ResourceAnalyzer * res)
{if (res) res->deleteLater();}
