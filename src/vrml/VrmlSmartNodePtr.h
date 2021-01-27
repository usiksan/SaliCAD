#ifndef VRMLSMARTNODEPTR_H
#define VRMLSMARTNODEPTR_H

#include "VrmlNode.h"

template <class Node>
class VrmlSmartNodePtr
  {
    Node *mPtr;
  public:
    VrmlSmartNodePtr() : mPtr(nullptr) {}
    ~VrmlSmartNodePtr() { free(); }

    void  operator = ( VrmlNode *ptr ) { free(); mPtr = dynamic_cast<Node*>(ptr); if( mPtr == nullptr && ptr != nullptr ) delete ptr; }

    Node *operator -> () { return mPtr; }

    bool  isValid() const { return mPtr != nullptr; }

    void  free() { if( mPtr != nullptr ) delete mPtr; }
  };

#endif // VRMLSMARTNODEPTR_H
