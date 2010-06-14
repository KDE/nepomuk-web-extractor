#ifndef NEPOMUK_DATAPPCONFIG_H
#define NEPOMUK_DATAPPCONFIG_H


#include "datappconfigbase.h" 
namespace Nepomuk {

class DataPPConfig : public DataPPConfigBase
{
  public:

    DataPPConfig( KSharedConfigPtr );
    DataPPConfig( const QString &  );
    static KSharedConfigPtr config(const QString & name);
    virtual ~DataPPConfig();
  private:
    static QString path;
};

}

#endif

