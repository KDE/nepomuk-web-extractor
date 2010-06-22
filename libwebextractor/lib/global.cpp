#include <webextractor/global.h>
#include <KDebug>

double Nepomuk::WebExtractor::WE::minACrit()
{
    static double val = 0.0;
    return val;
}

double Nepomuk::WebExtractor::WE::maxACrit()
{
    static double val = 1.0;
    return val;
}

double Nepomuk::WebExtractor::WE::minUCrit()
{
    return minACrit();
}

double Nepomuk::WebExtractor::WE::maxUCrit()
{
    static double val = 0.999;
    return val;
}

double Nepomuk::WebExtractor::WE::boundACrit(double val)
{
    if ( val < minACrit() ) {
	kDebug() << "ACrit value is less than minimun. Fixing";
	return minACrit();
    }
    else if ( val > maxACrit() ) {
	kDebug() << "ACrit value is greater than maximum. Fixing";
	return maxACrit();
    }

    return val;

}

double Nepomuk::WebExtractor::WE::boundUCrit(double val)
{
    if ( val < minUCrit() ) {
	kDebug() << "UCrit value is less than minimun. Fixing";
	return minUCrit();
    }
    else if ( val > maxUCrit() ) {
	kDebug() << "UCrit value is greater than maximum. Fixing";
	return maxUCrit();
    }

    return val;

}

double Nepomuk::WebExtractor::WE::minRank()
{
    return 0.0;
}

double Nepomuk::WebExtractor::WE::maxRank()
{
    return 0.9999;
}


double Nepomuk::WebExtractor::WE::boundRank(double val)
{
    return qBound(minRank(), maxRank(), val);
}

double Nepomuk::WebExtractor::WE::minScaleCoff()
{
    return -0.9999;
}

double Nepomuk::WebExtractor::WE::maxScaleCoff()
{
    return 0.9999;
}


double Nepomuk::WebExtractor::WE::boundScaleCoff(double val)
{
    return qBound(minScaleCoff(), maxScaleCoff(), val);
}
