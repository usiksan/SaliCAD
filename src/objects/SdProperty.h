/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Property template

  Property can contains empty value, single value or many values.
  Empty value is when no any values assigned to property.
  Single value is when exactly one value assigned to property.
  Many values is when some (two or more) different values assigned to property.
  This possibilities used when some object selected with different values of property
*/
#ifndef SDPROPERTY_H
#define SDPROPERTY_H

template <class Prop>
class SdProperty {
    enum {
      SdpNoValue,
      SdpSingleValue,
      SdpManyValues
      }  mStatus;     //!< Property status (not assigned, assigned exact one value or many different values)
    Prop mValue;      //!< Value for status equal to SdpSingleValue
  public:
    SdProperty() : mStatus(SdpNoValue) {}
    SdProperty( Prop val ) : mStatus(SdpSingleValue), mValue(val) {}

    //!
    //! \brief value Return current value. Warning! Value is valid only if isSingle returns true
    //!              in other cases it invalid!
    //! \return      Current value
    //!
    Prop value() const { return mValue; }

    //!
    //! \brief isEmpty Returns true when no values assigned to property
    //! \return        true when no values assigned to property
    //!
    bool isEmpty() const { return mStatus == SdpNoValue; }

    //!
    //! \brief isSingle Returns true when exactly one value assigned to property
    //! \return         true when exactly one value assigned to property
    //!
    bool isSingle() const { return mStatus == SdpSingleValue; }

    //!
    //! \brief isMany Returns true when many different values assigned to property
    //! \return       true when many different values assigned to property
    //!
    bool isMany() const { return mStatus == SdpManyValues; }

    //!
    //! \brief clear Clears property. So, no any values assigned to property
    //!
    void clear() { mStatus = SdpNoValue; }

    //!
    //! \brief set Set property to exact one value
    //! \param val Value which assigned to property
    //!
    void set( Prop val ) { mStatus = SdpSingleValue; mValue = val; }

    //!
    //! \brief append Append new value to property
    //! \param val    Value appended to property
    //!
    //! When property contains no values (i.e. cleared) then property appears as single value.
    //! When property contains single value and appended value not equal to property value
    //! then property appears as many values
    void append( Prop val )
      {
      if( mStatus == SdpNoValue ) {
        //When property contains no values (i.e. cleared) then property appears as single value.
        mStatus = SdpSingleValue;
        mValue = val;
        }
      else if( mStatus == SdpSingleValue && mValue != val )
        //When property contains single value and appended value not equal to property value
        //then property appears as many values
        mStatus = SdpManyValues;
      }

    //!
    //! \brief append Appends other property to this property
    //! \param src    Other property
    //!
    //! If only other property contains single value then this value appended to this property
    void append( const SdProperty<Prop> &src )
      {
      if( src.mStatus == SdpSingleValue ) append( src.mValue );
      }

    //!
    //! \brief match Two properties matched only if both contains single equal values
    //! \param src   Source property
    //! \return      true if both contains single equal values
    //!
    bool match( const SdProperty<Prop> &src ) const { return src.mStatus == SdpSingleValue && mStatus == SdpSingleValue && src.mValue == mValue; }

    //!
    //! \brief matchValue Value matched to this property if this property contains single values equals to val
    //! \param val        Matched value
    //! \return           true if this property contains single values equals to val
    //!
    bool matchValue( Prop val ) const { return mStatus == SdpSingleValue && mValue == val; }

    //!
    //! \brief swap Swap value with internal value of property. At this also status set as single value
    //! \param val  Swapped value
    //! \return     Previous value of property
    //!
    Prop swap( Prop val ) { Prop tmp(mValue); mValue = val; mStatus = SdpSingleValue; return tmp; }
  };

#endif // SDPROPERTY_H
