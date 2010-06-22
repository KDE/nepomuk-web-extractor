#include <webextractor/propertiesgroup.h>
#include <webextractor/soprano_statement_qhash.h>
#include <webextractor/decision.h>
#include <QtCore/QSet>
#include <QtCore/QSharedData>


namespace NW=Nepomuk::WebExtractor;

class NW::PropertiesGroup::Private : public QSharedData
{
    public:
    QSet< Soprano::Statement > statements;
    //Decision * parent;
    //const Soprano::Node mainTarget;
    double rank;
    unsigned int hash;
    /*Private(const Soprano::Node & mainTarget ):
	mainTarget(mainTarget)
    {;}
    */
};

NW::PropertiesGroup::PropertiesGroup()
{
    this->d = new NW::PropertiesGroup::Private();
    this->d->rank = 0;
    this->d->hash = 0;
    //d = new QSharedDataPointer<Private>(res);
    //d->parent = parent;
    //d->mainTarget = ;
    
}

NW::PropertiesGroup::~PropertiesGroup()
{;}

void NW::PropertiesGroup::addStatement(const Soprano::Statement & st)
{
    d->statements << st;
    
    // increase hash
    // TODO Analyze and implement more advanced hash function
    d->hash += qHash(st);
}

const QSet< Soprano::Statement > & NW::PropertiesGroup::data() const
{
    return d->statements;
}
/*
void NW::PropertiesGroup::addStatement(const Soprano::Node & predicate, const Soprano::Node & second, bool subject = true )
{
    if (subject) {
	d->statements << Soprano::Statement(d->mainTarget, predicate, second);
    }
    else {
	d->statements << Soprano::Statement(second, predicate, d->mainTarget);
    }


}
*/

double NW::PropertiesGroup::rank() const
{
    return d->rank;
}

void NW::PropertiesGroup::setRank(double newRank )
{
    d->rank = Decision::truncateRank(newRank);

}
NW::PropertiesGroup::PropertiesGroup( const PropertiesGroup & rhs)
{
    this->d = rhs.d;
}
const NW::PropertiesGroup & NW::PropertiesGroup::operator=(const PropertiesGroup & rhs)
{
    if (this == &rhs)
	return *this;
    this->d = rhs.d;
    return *this;
}

bool NW::PropertiesGroup::operator==(const PropertiesGroup & rhs) const
{
    if (this == &rhs)
	return true;

    return  /*(this->d->mainTarget == rhs.d.mainTarget ) and */
	(this->d->statements == rhs.d->statements);
}


bool NW::PropertiesGroup::operator!=(const PropertiesGroup & rhs) const
{
    return !(*this == rhs);
}


NW::PropertiesGroup & NW::operator<<( NW::PropertiesGroup & grp, const Soprano::Statement & st)
{
    grp.addStatement(st);
    return grp;
}

unsigned int NW::qHash( const Nepomuk::WebExtractor::PropertiesGroup & g)
{
    return g.d->hash;
}
