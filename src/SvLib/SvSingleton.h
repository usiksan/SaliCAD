/*
  Project "SvLib common library."
  Author
    Alexander Sibilev
  Internet
    www.salilab.com

  Description
    Singleton pattern

    We make constructor for some class private but public svInstance fuction which contains
    and return single pointer to class object

    class SomeClass {
        //SomeClass constructor is private to disable create class object outside svInstance function
        SomeClass() {}
      public:
        friend SomeClass *svInstance<SomeClass>();

        //Singleton function
        static SomeClass *instance() { return svInstance<SomeClass>(); }
      };
*/
#ifndef SVSINGLETON_H
#define SVSINGLETON_H

//!
//! \brief svInstance template function to release singleton pattern
//!                   In it we create new object and return it or return previously created object if it already exist
//! \return           Instance of SvClass
//!
template <typename SvClass>
static SvClass *svInstance()
  {
  static SvClass *instance;
  if( instance == nullptr )
    instance = new SvClass{};
  return instance;
  }

#endif // SVSINGLETON_H
