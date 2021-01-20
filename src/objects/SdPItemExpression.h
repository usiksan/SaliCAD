/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Expression object. This is text object which contains formulaes and text descriptions.
  This object stores immediatelly in library and not part of project
*/
#ifndef SDPITEMEXPRESSION_H
#define SDPITEMEXPRESSION_H

#include "SdProjectItem.h"

#define SD_TYPE_EXPRESSION "Expression"

class SdPItemExpression : public SdProjectItem
  {
    QStringList mContents; //!< Contents of expression object. Contains strings with text description and formulaues
  public:
    SdPItemExpression();

    QStringList contentsGet() const { return mContents; }

    //!
    //! \brief contentsSet Setup new contents of expression object
    //! \param strList     New contents of expression object
    //!
    void        contentsSet( QStringList strList ) { mContents = strList; }


    // SdObject interface
  public:
    virtual QString getType() const override;
    virtual SdClass getClass() const override;
    virtual void cloneFrom(const SdObject *src) override;
    virtual void writeObject(QJsonObject &obj) const override;
    virtual void readObject(SdObjectMap *map, const QJsonObject obj) override;

    // SdProjectItem interface
  public:
    virtual QString getIconName() const override;
    virtual quint64 getAcceptedObjectsMask() const override;
  };

#endif // SDPITEMEXPRESSION_H
