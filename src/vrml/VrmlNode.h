#ifndef VRMLNODE_H
#define VRMLNODE_H

#include <QString>
#include <QList>
#include <QMap>

//Forward scaner declaration
class SdScanerVrml;
class VrmlNode;

using VrmlNodePtr = VrmlNode*;

using VrmlNodePtrList = QList<VrmlNodePtr>;

using VrmlNodePtrMap = QMap<QString,VrmlNodePtr>;

class VrmlNode
  {
  public:
    VrmlNode() {}
    VrmlNode( const VrmlNode* ) {}
    virtual ~VrmlNode() {}

    virtual bool      parse( SdScanerVrml *scaner, const QString &fieldType ) = 0;
    virtual VrmlNode *copy() const = 0;

    static VrmlNode  *parse2Declaration(SdScanerVrml *scaner);
    static VrmlNode  *parse2Node( SdScanerVrml *scaner, const QString nodeType );
    static VrmlNode  *buildNode( const QString nodeType );
    static VrmlNode  *makeCopy( const VrmlNode *node );
    static void       deleteNode( VrmlNode *node );
  };



#endif // VRMLNODE_H
