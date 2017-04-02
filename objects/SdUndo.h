/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDUNDO_H
#define SDUNDO_H

class SdContainer;
class SdObject;

class SdUndo
  {
  public:
    SdUndo();

    void insertObject( SdContainer *container, SdObject *object );
    void deleteObject( SdContainer *container, SdObject *object );
    void properies( SdObject *object );
  };

#endif // SDUNDO_H
