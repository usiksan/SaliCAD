#ifndef SDM3DFUNCTION_H
#define SDM3DFUNCTION_H

#include "SdScriptValue.h"

#include <initializer_list>

#define SDM3D_MAX_PARAM 8

class SdM3dFunction : public SdScriptValue
  {
    char mResultType;
    int  mParamCount;
    char mParamTypes[SDM3D_MAX_PARAM];
  protected:
    SdM3dValuePtr mParamList[SDM3D_MAX_PARAM];
  public:
    SdM3dFunction( char resultType, char paramType0 );
    SdM3dFunction( char resultType, char paramType0, char paramType1 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6 );
    SdM3dFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6, char paramType7 );
    ~SdM3dFunction();

    void paramSet( int index, SdM3dValuePtr param ) { mParamList[index] = param; }

    int  paramCount() const { return mParamCount; }

    char paramType( int index ) const { return mParamTypes[index]; }


    // SdM3dValue interface
  public:
    virtual char type() const override { return mResultType; }

  private:
    void clearParamList();
  };

#endif // SDM3DFUNCTION_H
