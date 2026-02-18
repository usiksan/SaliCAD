#ifndef SDPVMULTY_H
#define SDPVMULTY_H

#include <type_traits>

//!
//! \brief The SdPvMulty class - Template class for managing properties that can have single or multiple values
//!        Used when selecting multiple objects with possibly different property values
//!
template <typename Prop>
class SdPvMulty
  {
    Prop mPropValue; //!< Stored property value (valid only when state is spmSingle)
    enum {
      spmNone,       //!< No value assigned to prop (initial state)
      spmSingle,     //!< Assigned single value (all selected objects have same property)
      spmMulty       //!< Assigned multiple values (selected objects have different properties)
    }    mState;     //!< Current assigned state

  public:
    //!
    //! \brief SdPvMulty Default constructor. Initializes state to spmNone.
    //!
    SdPvMulty() : mState(spmNone) {}


    const Prop &value() const { return mPropValue; }



    //!
    //! \brief clear Reset property to empty state (spmNone)
    //!
    void clear() { mState = spmNone; }



    //!
    //! \brief isMatch Check if given value matches current property state
    //! \param v Property value to check
    //! \return true if:
    //!         - State is spmMulty (multiple values - always match)
    //!         - State is spmSingle and stored value equals v
    //!         false if state is spmNone
    //!
    //bool isMatch( const Prop &v ) const { return !(mState != spmSingle || mPropValue != v); }

    bool isNeedUpdate( const Prop &v ) const { return (mState == spmSingle && mPropValue != v); }



    //!
    //! \brief isSingle Check if property has single value state
    //! \return true if state is spmSingle, false otherwise
    //!
    bool isSingle() const { return mState == spmSingle; }



    //!
    //! \brief reset Set property to single value mode
    //! \param v Value to assign
    //!
    void reset( const Prop &v )
      {
      mPropValue = v;
      mState = spmSingle;
      }



    //!
    //! \brief append Add another property value to current state
    //!        Updates state based on existing and new values:
    //!        - If state is spmNone: becomes spmSingle with v
    //!        - If state is spmSingle and v differs: becomes spmMulty
    //!        - Otherwise: state unchanged
    //! \param v Property value to append
    //!
    void append( const Prop &v )
      {
      if( mState == spmNone ) reset( v );
      else if( mState == spmSingle && mPropValue != v ) mState = spmMulty;
      //else mState == spmMulty or (mState == spmSingle && mPropValue == v)
      //in both cases we doing nothing
      }


    void append( const SdPvMulty<Prop> &pm )
      {
      if( mState == spmNone ) (*this) = pm;
      else if( mState == spmSingle && ((pm.mState == spmSingle && mPropValue != pm.mPropValue) || (pm.mState == spmMulty)) )
        mState = spmMulty;
      }



    //!
    //! \brief store Store current property value to destination if state is spmSingle
    //! \param dest Reference to destination property that receives stored value
    //!
    void store( Prop &dest ) const
      {
      if( mState == spmSingle )
        dest = mPropValue;
      }
  };






template <typename Obj, auto Member>
struct SdPropMultyField
  {
    using memberType = std::remove_reference_t< decltype( std::declval<Obj>().*Member) >;

    SdPvMulty<memberType> mField;
  };





template <typename Obj, auto ... Members>
class SdPropComposer : private SdPropMultyField<Obj,Members>...
  {
  public:
    void clear()
      {
      (SdPropMultyField<Obj,Members>::mField.clear(), ...);
      }

    void reset( const Obj &src )
      {
      (SdPropMultyField<Obj,Members>::mField.reset( src.*Members ), ...);
      }

    void append( const Obj &src )
      {
      (SdPropMultyField<Obj,Members>::mField.append( src.*Members ), ...);
      }

    void store( Obj &src ) const
      {
      (SdPropMultyField<Obj,Members>::mField.store( src.*Members ), ...);
      }

    // bool isMatch( const Obj &obj ) const
    //   {
    //   return (SdPropMultyField<Obj,Members>::mField.isMatch( obj.*Members ) && ...);
    //   }

    bool isNeedUpdate( const Obj &obj ) const
      {
      return (SdPropMultyField<Obj,Members>::mField.isNeedUpdate( obj.*Members ) || ...);
      }

    template<auto Member>
    auto& get()
      {
      using Base = SdPropMultyField<Obj,Member>;
      return static_cast<Base*>(this)->mField;
      }

    template<auto Member>
    const auto& get() const
      {
      using Base = SdPropMultyField<Obj,Member>;
      return static_cast<const Base*>(this)->mField;
      }

  };
#endif // SDPVMULTY_H
