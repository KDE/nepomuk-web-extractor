/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __simple_datapp_reply_h_
#define __simple_datapp_reply_h_

#include "executivereply.h"
#include "webextractor_export.h"
#include <Nepomuk/Resource>
#include <decision/decision.h>
#include <decision/decisionlist.h>

namespace Nepomuk
{
    namespace Decision {
	class DecisionFactory;
	class DecisionCreator;
    }


    namespace WebExtractor
    {
        class SimpleExecutiveReplyPrivate;
        /*! \brief This is convinience wrapper around ExecutiveReply.
         * It makes easy some common issueses that are met when subclassing ExecutiveReply.
         * This class is more usefull when you put all your functionality inside 
         * SimpleExecutiveReply subclass instance rather then when it serves mostly as
         * proxy to some other object.
         *
         * \section construction Constructor
         * Constructor accepts extra parameters comparing to ExecutiveReply class. It takes
         * Decision::DecisionFactory class and Nepomuk::Resource class. DecisionFactory is
         * used for creating new Decisions and resulting DecisionList. Nepomuk::Resource
         * instance is simply stored for your convinience and may be retrived with 
         * resource() function later.
         *
         * \section algorithm Advantages
         * The main diffenece is that SimpleExecutiveReply takes an instance of 
         * DecisionFactory and wrap this instance  methods into own. It creates a default
         * DecisionList and allow adding to it new Decisions. This list will be later
         * returned by decisions() function to the system. It also wraps 
         * DecisionFactory::newDecision() method into SimpleExecutiveReply::newDecision()
         * method, thus allowing more easy creation of decisions.
         * Second good thing that SimpleExecutiveReply do is wrapping emiting 
         * ExecutiveReply::finished() and ExecutiveReply::error(ExecutiveReply::Error) 
         * into one function SimpleExecutiveReply::finish(). This function will
         * check an error( set with ExecutiveReply::setError(ExecutiveReply::Error) )
         * and emit one of these signals acordingly. You can easily call this function
         * from constructor( unlike methods in ExecutiveReply).
         *
         * \section example1 Simple Example
         * In this section we present a simple example of SimpleExecutiveReply usage.
         * We will show how to implement subclass of SimpleExecutiveReply for 
         * plugin that assign tag
         * to the resource based on it's filename. This is simple, synchronious
         * reply, so everything is done right in constructor.
         * \code 
         * TagReply : public SimpleExecutiveReply
         * {
         *     TagReply::TagReply(TagExecutive * parent, const Decision::DecisionFactory *
         *        factory, const Nepomuk::Resource & res ):
         *        SimpleExecutiveReply(parent,factory,res)
         *        {
         * \endcode
         * First, let's check that resource is a file. We can skip this step if 
         * type filters was set with Executive::addTypeFilter( const QUrl &, FilterClassType, bool)
         * \code  
         *           if(!res.hasType(Nepomuk::Vocabulary::NFO::FileDataObject())) {
         *               kDebug() << "Resource is not a file";
         *               setError(ResourceTypeIncorrect);
         *               finish();
         *               return;
         *           }
         * \endcode
         * Now read settings from Executive - tag name, description and a regular 
         * expression. Because we do not change them, we 
         * can read them without protecting with mutexes.
         * \code
         *           QRegExp regexp = parent->m_regexp;
         *           QString tagName = parent->m_tag;
         *           QString tagDescription = parent->m_description;
         * \endcode
         * Now we will check that filename matches regexp.
         * \code
         *           QString filename = res.property(Nepomuk::Vocabulary::NFO::fileName()).toString();
         *           if(regexp.exactMatch(filename)) {
         * \endcode
         * Ok, filename matches. So, lets create a Decision and add it to the list. 
         * Decisions can not be created as is. To create proper decision one should use
         * DecisionCreator instance. This class will accept changes in different format
         * and convert them to format used in Decision itself. So, how changes are
         * stored inside Decision is hidden from you. Currently, the only supported
         * format for describing changes is SimpleResourceGraph, but it will change
         * soon.
         * How to create SimpleResourceGraph is not subject of current tutorial, see
         * documentation of SimpleResourceGraph itself
         * \code 
         *               ND::DecisionCreator d = newDecision();
         *               Nepomuk::SimpleResourceGraph changes;
         *               Nepomuk::SimpleResource resChanges(res.resourceUri());
         *               Nepomuk::SimpleResource tag;
         *               tag.addType(Soprano::Vocabulary::NAO::Tag());
         *               if ( !tagDescription.isEmpty() ) {
         *                   tag.addProperty(Soprano::Vocabulary::NAO::description(), tagDescription);
         *               }
         *               tag.addProperty(Soprano::Vocabulary::NAO::prefLabel(), tagName);
         *               resChanges.addProperty(Soprano::Vocabulary::NAO::hasTag(), tag);
         *               changes.insert(resChanges);
         *               changes.insert(tag);
         *               d.setChanges(changes);
         * \endcode
         * Now we should set some properties of Decision itself. Currently, required 
         * properties are rank and description.
         * \code
         *               d.setRank(Nepomuk::Decision::maxRank());
         *               static QString descriptionTemplate = QString("Assign tag %1 to the resource %2");
         *               d.setDescription(descriptionTemplate.arg(tagName, res.resourceUri().toString()));
         * \endcode
         * And finally we add Decision to the list of answers
         * \code
         *               addDecision(d);
         *           } else {
         *               kDebug() << "File name doesn't match parameter";
         *           }
         * \endcode
         * Finishing our job by calling finish() function
         * \code
         *           finish();
         * };
         * \endcode
         *
         * \section netexample Asynchronious
         * Creating of network plugins is mostly the same. In constructor you create
         * QNetworkRequest, connect you functions to it's signals, and launch it.
         * In connected functions you parse result of request, create, fill and add 
         * Decision as described above and launch more QNetworkRequests if necessary or
         * call finish() if extraction finished.
         * Same techique can be applied to any asynchronious system - network, creating
         * processes with QProcess and so on.
         *
         */
        class WEBEXTRACTOR_EXPORT SimpleExecutiveReply  : public ExecutiveReply
        {
                Q_OBJECT;
            public:
                SimpleExecutiveReply(Executive * parent, const  Decision::DecisionFactory * factory, const Nepomuk::Resource & res);
                /*! \brief Return new DecisionCreator that can be used to create a Decision
                 */
                Decision::DecisionCreator newDecision();
                /*! \brief Return new DecisionList.
                 * This method was implemented in hope that it can be used somewhere,
                 * but no real use case was found. 
                 */
                Decision::DecisionList newDecisionList();
                /*! \brief Add Decision
                 * You can get actuall Decision by calling DecisionCreator::data(). Yet,
                 * there is no need to do so - use addDecision( const Decision::DecisionCreator & ) instead of this function
                 */
                void addDecision(const Decision::Decision &);
                /*! \brief Add Decision with parameters
                 * It basically the same as simple addDecision(const Decision::Decision &)
                 * but it takes also some parameters that influence how equal Decision
                 * are merged together. Ignore this function for now.
                 */
                void addDecision(const Decision::Decision &, Decision::MergePolitics politics, double coff = 1, bool noAuto = false);
                void addDecision(const Decision::DecisionCreator &);
                void addDecision(const Decision::DecisionCreator & , Decision::MergePolitics politics, double coff = 1, bool noAuto = false);
                double approximateThreshold() const;
                virtual Decision::DecisionList decisions() const;
                const Nepomuk::Resource & resource();
                //DecisionList & mainDecisionList();
                virtual ~SimpleExecutiveReply() {
                    ;
                }
            public Q_SLOTS:
                /*! \brief Will finish Executive
                 * This function correctly handle situations when it is called from
                 * constructor of the ExecutiveReply.
                 * It will call realFinish() internaly
                 */
                void finish();
            protected Q_SLOTS:
                /*! \brief This function perform actual finishing.
                 * This function \b must be called from Qt event loop!
                 */
                void realFinish();
            protected:
                SimpleExecutiveReply(Executive * parent, const Decision::DecisionFactory * factory, const Nepomuk::Resource & res , SimpleExecutiveReplyPrivate & p);
            private:
                Q_DECLARE_PRIVATE(SimpleExecutiveReply)

        };
    }
}


#endif
