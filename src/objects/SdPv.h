#ifndef SDPV_H
#define SDPV_H

#include <QString>

template <typename Val, typename Prop>
class SdPv
  {
  protected:
    Val mValue;
  public:
    SdPv( Val v ) : mValue(v) {}

    bool operator == ( const Prop &other ) const { return mValue == other.mValue; }

    bool operator != ( const Prop &other ) const { return mValue != other.mValue; }

    Val  value() const { return mValue; }

    //!
    //! \brief swap Swap values with another SdPropInt instance
    //! \param other Other integer property to swap with
    //!
    void swap( Prop &other )
      {
      if constexpr (std::is_trivially_copyable_v<Val>)
        { qSwap( mValue, other.mValue ); }
      else
        { mValue.swap( other.mValue ); }
      }


  };
#endif // SDPV_H
