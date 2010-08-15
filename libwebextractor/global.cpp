#include "global.h"
#include <KDebug>

double Nepomuk::WebExtractor::minACrit()
{
    static double val = 0.0;
    return val;
}

double Nepomuk::WebExtractor::maxACrit()
{
    static double val = 1.0;
    return val;
}

double Nepomuk::WebExtractor::minUCrit()
{
    return minACrit();
}

double Nepomuk::WebExtractor::maxUCrit()
{
    static double val = 0.999;
    return val;
}

double Nepomuk::WebExtractor::boundACrit(double val)
{
    if(val < minACrit()) {
        kDebug() << "ACrit value is less than minimun. Fixing";
        return minACrit();
    } else if(val > maxACrit()) {
        kDebug() << "ACrit value is greater than maximum. Fixing";
        return maxACrit();
    }

    return val;

}

double Nepomuk::WebExtractor::boundUCrit(double val)
{
    if(val < minUCrit()) {
        kDebug() << "UCrit value is less than minimun. Fixing";
        return minUCrit();
    } else if(val > maxUCrit()) {
        kDebug() << "UCrit value is greater than maximum. Fixing";
        return maxUCrit();
    }

    return val;

}

double Nepomuk::WebExtractor::minRank()
{
    return 0.0;
}

double Nepomuk::WebExtractor::maxRank()
{
    return 0.9999;
}


double Nepomuk::WebExtractor::boundRank(double val)
{
    return qBound(minRank(), maxRank(), val);
}

double Nepomuk::WebExtractor::minScaleCoff()
{
    return -0.9999;
}

double Nepomuk::WebExtractor::maxScaleCoff()
{
    return 0.9999;
}


double Nepomuk::WebExtractor::boundScaleCoff(double val)
{
    return qBound(minScaleCoff(), maxScaleCoff(), val);
}

/*
Soprano::Model * NW::decisionsModel()
{
    // Find necessary backend
    static Soprano::Backend * backend = Soprano::discoverBackendByFeatures(Soprano::BackendOptionStorageMemory);
    static Soprano::Model * val = backend->createModel(Soprano::BackendOptionStorageMemory);
    return val;
}

*/
