#ifndef SDM3DFUNCTION_H
#define SDM3DFUNCTION_H

#include "SdM3dValue.h"

#include <initializer_list>

#define SDM3D_MAX_PARAM 8

class SdM3dFunction : public SdM3dValue
  {
    char          mResultType;
    int           mParamCount;
    const char   *mParamTypes;
  protected:
    SdM3dValuePtr mParamList[SDM3D_MAX_PARAM];
  public:
    SdM3dFunction( char resultType, int paramCount, std::initializer_list<char> paramTypes );
    ~SdM3dFunction();

    void paramSet( int index, SdM3dValuePtr param ) { mParamList[index] = param; }

    int  paramCount() const { return mParamCount; }

    char paramType( int index ) const { return mParamTypes[index]; }


    // SdM3dValue interface
  public:
    virtual char type() const override { return mResultType; }
  };

#endif // SDM3DFUNCTION_H
