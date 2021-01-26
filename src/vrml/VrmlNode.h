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
    QString mName;
  public:

    VrmlNode();
    virtual ~VrmlNode() {}

    virtual void      parse( SdScanerVrml *scaner ) = 0;
    virtual VrmlNode *copy() const = 0;
    void              cloneNode( VrmlNode *destNode ) const;

    static void       parse2Declaration(SdScanerVrml *scaner, VrmlNodePtrList *list);
    static VrmlNode  *parse2Node( SdScanerVrml *scaner, const QString nodeType );
    static VrmlNode  *buildNode( const QString nodeType );
  };



#endif // VRMLNODE_H
