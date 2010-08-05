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

#ifndef _NEPOMUK_WEBEXTRCT_DATA_PP_PROXY_H_
#define _NEPOMUK_WEBEXTRCT_DATA_PP_PROXY_H_

#include "datapp.h"
#include "webextractor_export.h"
namespace Nepomuk
{
    namespace WebExtractor
    {
        class WEBEXTRACTOR_EXPORT DataPPWrapper
        {
            public:
                DataPPWrapper(DataPP*, const QString & , double, double);
                DataPP * data() const {
                    return m_data;
                }
                double rank() const {
                    return m_rank;
                }
                double coff() const {
                    return m_scaleCoff;
                }
                void setRank(double val) {
                    m_rank = val;
                }
                QString  pluginName() const {
                    return m_data->pluginName();
                }
                float  pluginVersion() const {
                    return m_data->pluginVersion();
                }
                DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res)const;
            private:
                DataPP * m_data;
                double m_rank;
                double m_scaleCoff;
        };
        typedef QHash< const DataPP*, DataPPWrapper* > DataPPKeeper;
    }
}

#endif
