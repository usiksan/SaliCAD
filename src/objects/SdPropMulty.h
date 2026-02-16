#ifndef SDPROPMULTY_H
#define SDPROPMULTY_H
//!
//! \brief The SdPropMulty class - Template class for managing properties that can have single or multiple values
//!        Used when selecting multiple objects with possibly different property values
//!
template <typename Prop>
class SdPropMulty
  {
    Prop mPropValue; //!< Stored property value (valid only when state is spmSingle)
    enum {
      spmNone,       //!< No value assigned to prop (initial state)
      spmSingle,     //!< Assigned single value (all selected objects have same property)
      spmMulty       //!< Assigned multiple values (selected objects have different properties)
    }    mState;     //!< Current assigned state

  public:
    //!
    //! \brief SdPropMulty Default constructor. Initializes state to spmNone.
    //!
    SdPropMulty() : mState(spmNone) {}



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
    bool isMatch( const Prop &v ) const
      {
      if( mState == spmMulty ) return true;
      if( mState == spmNone ) return false;
      return mPropValue == v;
      }



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



    //!
    //! \brief store Store current property value to destination if state is spmSingle
    //! \param dest Reference to destination property that receives stored value
    //!
    void store( Prop &dest )
      {
      if( mState == spmSingle )
        dest = mPropValue;
      }
  };
#endif // SDPROPMULTY_H
