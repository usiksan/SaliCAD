#ifndef SDGRAPHSCRIPTREF_H
#define SDGRAPHSCRIPTREF_H

#include "SdJsonIO.h"
#include "SdGraphParam.h"


class SdGraphScriptRef
  {
    mutable SdContainer  *mRefOwner;
    mutable SdGraphParam *mRef;
            QString       mName;
            QString       mParam;
            QString       mValue;
            SdRect        mOverName;
            SdRect        mOverValue;
            QPoint        mOriginValue;
            bool          mCalculated;   //!< If this flag true then value referenced this variable is calculated by script
                                         //!  and must not be changed by user because it will be replaced by calculated value
  public:
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
    //! \brief overName Return over name rectangle
    //! \return         Over name rectangle
    //!
    //! When variable drawing simultaneously calculated over rect filled by referenced name of variable
    //! This function return this rectangle
    SdRect  overName() const { return mOverName; }

    SdRect  overValue() const { return mOverValue; }

    SdPoint originValue() const { return mOriginValue; }

    void    draw( SdPoint p, const SdPropText &prop, SdContext *dc, bool drawValue );

    QString valueGet() const;

    void    valueSet( const QString &v );

    void    assign( SdContainer *refOwner, SdGraphParam *ref, QString param );

    void    jsonWrite( SdJsonWriter &js ) const;

    void    jsonRead( SdJsonReader &js );

  private:
    bool    checkRef() const;
  };

#endif // SDGRAPHSCRIPTREF_H
