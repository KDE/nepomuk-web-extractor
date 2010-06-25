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

#ifndef __dbpedia_music_datapp_h_
#define __dbpedia_music_datapp_h_

#include <webextractor/datapp.h>
#include <Soprano/SparqlModel>

namespace Nepomuk {
    class DbpMusicReply;
    class DbpMusicDataPP : public WebExtractor::DataPP 
    {
	Q_OBJECT;
	public:
	WebExtractor::DataPPReply * requestDecisions(const WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res) ;
	    DbpMusicDataPP(
		    // Actualy any async model can be used here. Unfortunately Soprano::Util::AsyncModel is 
		    // not a base class for SparqlModel and can not be used. What a pity ((. 
		    Soprano::SparqlModel * model,
		    // This parameter is passed only for convinience. It can be retrived automatically.
		    const QString & pluginVersion
		    );
	    ~DbpMusicDataPP();
	    friend class DbpMusicReply;
	private:
	    // Make a shared pointer
	    Soprano::SparqlModel * m_model;
    };
}

#endif
