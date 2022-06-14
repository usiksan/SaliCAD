#ifndef VRMLNODE_H
#define VRMLNODE_H

#include "VrmlVector.h"
#include "VrmlColor.h"

#include <QString>
#include <QList>
#include <QMap>
#include <functional>

//Forward scaner declaration
class SdScanerVrml;
class VrmlNode;
class VrmlNodeMaterial;
class Sd3dModel;
class Sd3drInstance;
class Sd3drBody;

using VrmlNodePtr = VrmlNode*;

using VrmlNodePtrList = QList<VrmlNodePtr>;

using VrmlNodePtrMap = QMap<QString,VrmlNodePtr>;

//using VrmlFaceAppendFunction

class VrmlNode
  {
  public:
    VrmlNode() {}
    VrmlNode( const VrmlNode* ) {}
    virtual ~VrmlNode() {}

    virtual void      generateFaces( Sd3dModel *model, Sd3drInstance &instance, Sd3drBody &body ) const;
    virtual bool      parse( SdScanerVrml *scaner, const QString &fieldType ) = 0;
    virtual VrmlNode *copy() const = 0;

    static VrmlNode  *parse2Declaration(SdScanerVrml *scaner);
    static VrmlNode  *parse2Node( SdScanerVrml *scaner, const QString nodeType );
    static VrmlNode  *build2Node( const QString nodeType );
    static VrmlNode  *makeCopy( const VrmlNode *node );
    static void       deleteNode( VrmlNode *node );
  };



#endif // VRMLNODE_H
