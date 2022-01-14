/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Script language is C-style hi-level language for programming 3d models, 2d parametric graphics and
  schematic calculations.

  Embedded function.

  Script language not supported definition of functions. Available only embedded functions. This structure
  represents souch functions.
*/
#ifndef SDSCRIPTVALUEFUNCTION_H
#define SDSCRIPTVALUEFUNCTION_H

#include "SdScriptValue.h"

#include <initializer_list>

#define SDSCRIPT_FUN_MAX_PARAM 8 //!< Maximal parameters available for functions

class SdScriptValueFunction : public SdScriptValue
  {
    char mResultType;                         //!< Result type of function
    int  mParamCount;                         //!< Param count for function 0 - (SDSCRIPT_FUN_MAX_PARAM-1)
    char mParamTypes[SDSCRIPT_FUN_MAX_PARAM]; //!< Type of each param
  protected:
    SdScriptValuePtr mParamList[SDSCRIPT_FUN_MAX_PARAM];
  public:
    SdScriptValueFunction( char resultType, char paramType0 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6 );
    SdScriptValueFunction( char resultType, char paramType0, char paramType1, char paramType2, char paramType3, char paramType4, char paramType5, char paramType6, char paramType7 );
    ~SdScriptValueFunction();

    //!
    //! \brief paramSet Set param for function
    //! \param index    Index of param
    //! \param param    Param value ptr
    //!
    void paramSet( int index, SdScriptValuePtr param ) { mParamList[index] = param; }

    //!
    //! \brief paramCount Return function param count
    //! \return           Function param count
    //!
    int  paramCount() const { return mParamCount; }

    //!
    //! \brief paramType Return param type
    //! \param index     Index of param
    //! \return          Type of param with index
    //!
    char paramType( int index ) const { return mParamTypes[index]; }


    // SdM3dValue interface
  public:
    //!
    //! \brief type Return type of object
    //! \return     Type of object
    //!
    virtual char type() const override { return mResultType; }

  private:
    void clearParamList();
  };

#endif // SDSCRIPTVALUEFUNCTION_H
