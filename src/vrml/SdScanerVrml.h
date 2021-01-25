#ifndef SDSCANERVRML_H
#define SDSCANERVRML_H

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
  private:
    bool parseRoot();
    bool parseVrml2_0();
    bool parseVrml1_0();

    bool parse2Declaration( VrmlNodePtrList *list );
    bool parse2DEF( VrmlNodePtrList *list );
    bool parse2USE( VrmlNodePtrList *list );
    bool parse2Group( VrmlNodePtrList *list );
    bool parse2Transform( VrmlNodePtrList *list );
  };

#endif // SDSCANERVRML_H
