#ifndef SDSCANERVRML_H
#define SDSCANERVRML_H

#include "VrmlTypes.h"
#include "VrmlVector.h"
#include "SdScanerMultyline.h"
#include "VrmlNode.h"

class SdScanerVrml : public SdScanerMultyline
  {
    VrmlNodePtrList mRootList;
    VrmlNodePtrMap  mRootMap;
  public:
    SdScanerVrml();
    ~SdScanerVrml();

    void clear();

    bool parseFile( const QString &path );

    void insert( const QString name, VrmlNodePtr node ) { mRootMap.insert( name, node ); }

    VrmlNode *node( const QString name ) const { return mRootMap.value(name); }


    bool      tokenNeedValueBool( bool &value, const QString errorMsg );

    bool      parseInt32Table( VrmlInt32List &table, const QString errorMsg );

    bool      parseVectorTable( VrmlVectorList &table, const QString errorMsg );

  private:
    bool parseVrml2_0();
    bool parseVrml1_0();

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };

#endif // SDSCANERVRML_H
