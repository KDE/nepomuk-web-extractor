#include <webextractor/parameters.h>
#include <KDebug>
#include <QtCore/QSharedData>

class Nepomuk::WebExtractor::ExtractParameters::Private : public QSharedData
{
    public:
	WE::MergePolitics m_mergePolitics;
	WE::LaunchPolitics m_launchPolitics;
	unsigned int m_pss;
	double acrit, ucrit;
	DataPPKeeper dataPPlugins;

};


Nepomuk::WebExtractor::ExtractParameters::ExtractParameters()
{
    this->d = QSharedDataPointer<Private>( 
	    new Nepomuk::WebExtractor::ExtractParameters::Private()
	    );
}

Nepomuk::WebExtractor::ExtractParameters::~ExtractParameters()
{;}

Nepomuk::WebExtractor::ExtractParameters::ExtractParameters( const ExtractParameters & rhs)
{
    d = rhs.d;
}

const Nepomuk::WebExtractor::ExtractParameters & Nepomuk::WebExtractor::ExtractParameters::operator=( const ExtractParameters & rhs)
{
    d = rhs.d;
    return *this;
}

bool Nepomuk::WebExtractor::ExtractParameters::hasAnyDataPP() const
{return (d->dataPPlugins.size() != 0);}


Nepomuk::WebExtractor::WE::MergePolitics Nepomuk::WebExtractor::ExtractParameters::mergePolitics() const
{
    return d->m_mergePolitics;
}

void Nepomuk::WebExtractor::ExtractParameters::setMergePolitics(WE::MergePolitics val) 
{
    if ( ( val < WE::MergePolitics_MIN) or (val > WE::MergePolitics_MAX) )
    {
	kDebug() << "Unknow merge politics :" << val << "Defaulted to MergePolitics::Highest";
	val = WE::Highest;
    }
    d->m_mergePolitics = val; 
}

Nepomuk::WebExtractor::WE::LaunchPolitics Nepomuk::WebExtractor::ExtractParameters::launchPolitics() const
{
    return d->m_launchPolitics;
}

void Nepomuk::WebExtractor::ExtractParameters::setLaunchPolitics(WE::LaunchPolitics val) 
{
    if ( ( val < WE::LaunchPolitics_MIN) or (val > WE::LaunchPolitics_MAX) )
    {
	kDebug() << "Unknow merge politics :" << val << "Defaulted to LaunchPolitics::Highest";
	val = WE::StepWise;
    }
    d->m_launchPolitics= val; 
}

unsigned int Nepomuk::WebExtractor::ExtractParameters::pluginSelectStep() const
{return d->m_pss;}

void Nepomuk::WebExtractor::ExtractParameters::setPluginSelectStep(unsigned int step)
{
    if (step == 0) {
	kDebug() << "Zero step as plugin select step";
	step = 1;
    }
    d->m_pss = step;
}

double Nepomuk::WebExtractor::ExtractParameters::aCrit() const
{ return d->acrit; }

void Nepomuk::WebExtractor::ExtractParameters::setACrit(double val)
{
    if ( val > 1)
	val = 1;
    
    if (val < 0 )
	val = 0;

    d->acrit = val;

}

double Nepomuk::WebExtractor::ExtractParameters::uCrit() const
{ return d->ucrit; }

void Nepomuk::WebExtractor::ExtractParameters::setUCrit(double val)
{
    if ( val > 1)
	val = 0.999;
    
    if (val < 0 )
	val = 0;

    d->ucrit = val;

}

const Nepomuk::WebExtractor::DataPPKeeper & Nepomuk::WebExtractor::ExtractParameters::plugins()  const
{ 
    return d->dataPPlugins;
}

void Nepomuk::WebExtractor::ExtractParameters::addDataPP( DataPPWrapper * pp )
{ 
    d->dataPPlugins.insert(pp->data(), pp);
}

/*
double Nepomuk::WebExtractor::ExtractParameters::scaleCoff(DataPP* pp)  const
{
    if (!d->coffs.contains(pp) ) {
	kDebug() << "Unknown DataPP.";
	return 0;
    }
    else { 
	return d->coffs[pp];
    }
}

void Nepomuk::WebExtractor::ExtractParameters::setScaleCoff(DataPP* pp, double coff)
{
    if (!d->coffs.contains(pp) ) {
	kDebug() << "Unknown DataPP.";
    }
    else { 
	 d->coffs[pp] = coff;;
    }
} 
*/

QDebug Nepomuk::WebExtractor::operator<<( QDebug dbg,  const Nepomuk::WebExtractor::ExtractParameters & p)
{
    dbg<<"Extract parameters:\n";
    dbg<<"aCrit: "<<p.aCrit()<<"\n";
    dbg<<"uCrit: "<<p.uCrit()<<"\n";
    if ( p.launchPolitics() == WE::StepWise ) {
	dbg << "Step wise launch politics. Step: "<<p.pluginSelectStep()<<"\n";
    }
    else {
	dbg << "All launch politics\n";
    }
    dbg<<"Plugins: "<<p.d->dataPPlugins.size()<<'\n';
    return dbg;
}
