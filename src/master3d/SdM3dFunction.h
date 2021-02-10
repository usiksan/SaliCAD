#ifndef SDM3DFUNCTION_H
#define SDM3DFUNCTION_H

#include "SdM3dValue.h"

#define SDM3D_MAX_PARAM 8

class SdM3dFunction : public SdM3dValue
  {
  protected:
    SdM3dValuePtr mParamList[SDM3D_MAX_PARAM];
  public:
    SdM3dFunction();
    ~SdM3dFunction();

    void         paramSet( int index, SdM3dValuePtr param );

    virtual int  paramCount() const = 0;
    virtual char paramType( int index ) const = 0;

  };

#endif // SDM3DFUNCTION_H
