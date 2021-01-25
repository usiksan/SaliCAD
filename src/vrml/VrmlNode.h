#ifndef VRMLNODE_H
#define VRMLNODE_H

#include <QString>
#include <QList>
#include <QMap>

class VrmlNode
  {
    QString mName;
  public:
    VrmlNode();
    virtual ~VrmlNode() {}
  };

using VrmlNodePtr = VrmlNode*;

using VrmlNodePtrList = QList<VrmlNodePtr>;

using VrmlNodePtrMap = QMap<QString,VrmlNodePtr>;


#endif // VRMLNODE_H
