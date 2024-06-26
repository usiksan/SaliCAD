#ifndef SDSCANERVRML_H
#define SDSCANERVRML_H

#include "VrmlTypes.h"
#include "VrmlVector.h"
#include "import/SdScanerMultyline.h"
#include "VrmlNode.h"

class SdScanerVrml : public SdScanerMultyline
  {
    VrmlNodePtrList mRootList;
    VrmlNodePtrMap  mRootMap;
  public:
    SdScanerVrml();
    ~SdScanerVrml();

    void clear();

    //!
    //! \brief parseFile Parse source VRML file to mRootList
    //! \param path      Path to source VRML file
    //! \return          true if parse successfull
    //!
    bool      parseFile( const QString &path );

    //!
    //! \brief generateFaces Generates faces for all nodes in mRootList
    //! \param model         Model into which appended faces and so on
    //!
    void      generateFaces( Sd3drModel *model ) const;

    void      insert( const QString name, VrmlNodePtr node ) { mRootMap.insert( name, node ); }

    VrmlNode *node( const QString name ) const { return mRootMap.value(name); }


    bool      tokenNeedValueBool( bool &value, const QString errorMsg );

    bool      parseInt32Table( VrmlInt32List &table, const QString errorMsg );

    bool      parseVectorTable( VrmlVectorList &table, const QString errorMsg );

    bool      parseFloatTable( QList<float> &table, const QString errorMsg );

    bool      parseColorTable( VrmlColorList &table, const QString errorMsg );

  private:
    bool parseVrml2_0();
    bool parseVrml1_0();

    // SdScaner interface
  public:
    virtual void tokenNext() override;
  };

#endif // SDSCANERVRML_H
