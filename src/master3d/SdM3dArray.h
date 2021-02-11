#ifndef SDM3DARRAY_H
#define SDM3DARRAY_H

#include "SdM3dValue.h"

class SdM3dArray : public SdM3dValue
  {
  protected:
    SdM3dValuePtrList mArray;
  public:
    SdM3dArray() : SdM3dValue() {}
    ~SdM3dArray() { qDeleteAll(mArray); }

    void append( SdM3dValuePtr val ) { mArray.append( val ); }

    virtual char typeOfElement() const = 0;
  };

#endif // SDM3DARRAY_H
