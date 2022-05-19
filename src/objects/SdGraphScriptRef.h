/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object with script. Script used for in schematic calculations.
  It represents math script language like python or subset of C

  Example of script:
  #This is remark. All symbols after # sing - are ignored
  R1 = ref( "Om", "" )
  R2 = ref( "Om", "" )
  Rcom = ref( "Om", "" )
  Rcom = R1 + R2

  This script calculate common resistance of continuosly connected resistors.
  ref function used to declare visibility and editing possibility of variable.
  In schematic such script will be displayed as:

  R1 = 10kOm
  R2 = 10kOm
  Rcom = 20kOm

  Values 10kOm are editable, i.e. they can be edited by text mode. When they changed
  script automatically recalculated and display correct result.

  SdGraphScriptRef is single param of script (source or result).
  It consists from script name and associated with it value.
  Param may reference to extern param of any schematic object derived from SdGraphParam
  If reference precent then value taked and placed to it but internal value.

  The main problem is to save links during script copying operations. It is solved using
  the mRefOffset field. This field contains the offset on the diagram to the link relative
  to the current position of the script. When inserted into the schema, a search is
  performed for this offset and, if the reference object is found, it is written to the
  parameter structure.
*/
#ifndef SDGRAPHSCRIPTREF_H
#define SDGRAPHSCRIPTREF_H

#include "SdJsonIO.h"
#include "SdGraphParam.h"


class SdGraphScriptRef
  {
    mutable SdContainer  *mRefOwner;     //!< Owner of referenced object. In fact - sheet
    mutable SdGraphParam *mRef;          //!< Referenced object
            QString       mName;         //!< Name of param in original script (key)
            QString       mParam;        //!< Name of param in referenced object (for example "value")
            QString       mValue;        //!< Internal value of param. If present referenced object then this value not used
            SdRect        mOverName;     //!< Visual over rectangle over name of param
            SdRect        mOverValue;    //!< Visual over rectangle over value of param
            QPoint        mOriginValue;  //!< Visual origin point of value of param
            bool          mCalculated;   //!< If this flag true then value referenced this variable is calculated by script
                                         //!  and must not be changed by user because it will be replaced by calculated value

  public:
            QPoint        mRefOffset;    //!< Relative displacement of referenced param object

    SdGraphScriptRef();
    SdGraphScriptRef( const QString &nm );

    //!
    //! \brief name Return name of variable as it in original script
    //! \return     Name of variable in original script
    //!
    QString name() const { return mName; }

    //!
    //! \brief refName Return referenced name.
    //! \return        Referenced name.
    //!
    //! This variable can be referenced to any variable in any paramed object. So, if it referenced
    //! then this function return name of referenced paramed object and name of its param.
    //! If no referenced object then this function return original variable name from script
    QString refName() const;

    //!
    //! \brief refOffset Returns relative offset of referenced object relative org point
    //! \param org       Origin point
    //! \return          Relative offset of referenced object relative org point
    //!
    QPoint  refOffset( QPoint org ) const;

    //!
    //! \brief refParam Returns name of param for referenced object
    //! \return         Name of param for referenced object
    //!
    QString refParam() const { return mParam; }

    //!
    //! \brief overName Return over name rectangle
    //! \return         Over name rectangle
    //!
    //! When variable drawing simultaneously calculated over rect filled by referenced name of variable
    //! This function return this rectangle
    SdRect  overName() const { return mOverName; }

    //!
    //! \brief overValue Return over value rectangle
    //! \return          Over value rectangle
    //!
    //! When variable drawing simultaneously calculated over rect filled by value of variable
    //! This function return this rectangle
    SdRect  overValue() const { return mOverValue; }

    SdPoint originValue() const { return mOriginValue; }

    //!
    //! \brief draw      Draws variable at point p with visual property prop
    //! \param p         Origin of drawing
    //! \param prop      Property of drawing
    //! \param dc        Drawing context
    //! \param drawValue If true then variable drawing fully: name and value. If this
    //!                  false then drawing name only
    //!
    void    draw( SdPoint p, const SdPropText &prop, SdContext *dc, bool drawValue );

    //!
    //! \brief valueGet Returns current value of variable
    //! \return         Current value of variable
    //!
    //! If there reference then we extract value from it. Else we return internal value
    QString valueGet() const;

    //!
    //! \brief valueSet Replace value with new
    //! \param v        New value
    //!
    //! If there reference then we replace its param value with this new one. Else
    //! we write new value into internal value
    void    valueSet( const QString &v );

    //!
    //! \brief refAssign Assign new reference to variable
    //! \param refOwner  Owner of new reference (in fact - sheet). Can be nullptr
    //! \param ref       Referenced object. Can be nullptr
    //! \param param     Name of param in referenced object. If this string is empty
    //!                  then current param name is left unchanged
    //!
    void    refAssign( SdContainer *refOwner, SdGraphParam *ref, QString param );

    void    jsonWrite( SdJsonWriter &js ) const;

    void    jsonRead( SdJsonReader &js );

  private:
    bool    checkRef() const;
  };

#endif // SDGRAPHSCRIPTREF_H
