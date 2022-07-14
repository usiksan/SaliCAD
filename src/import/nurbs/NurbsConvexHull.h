#ifndef NURBSCONVEXHULL_H
#define NURBSCONVEXHULL_H

#include "NurbsFace.h"

#include <QVector3D>
#include <QList>

class NurbsConvexHull
  {
    struct twoset {
        void insert(int x) { (a == -1 ? a : b) = x; }
        bool contains(int x) const { return a == x || b == x; }
        void erase(int x) { (a == x ? a : b) = -1; }
        int  size() const { return ((a != -1) ? 1 : 0) + ((b != -1) ? 1 : 0); }
        int  a, b;
      };

    QList<NurbsFace> mFaces;
    QList<QVector3D> mArray;
    twoset           mE[100][100];
  public:
    NurbsConvexHull() {}
    NurbsConvexHull( const QList< QList<QVector3D> > &vertex );

    /* Compute the half plane {x : c^T norm < disc}
    * defined by the three points A[i], A[j], A[k] where
    * A[inside_i] is considered to be on the 'interior' side of the face. */

  private:
    //!
    //! \brief makeFace Build face with three vertex indexes and direction to outside figure
    //! \param i        Index of first vertex
    //! \param j        Index of second vertex
    //! \param k        Index of third vertex
    //! \param inside_i Index of vertex inside figure
    //! \return         Builded face
    //!
    NurbsFace makeFace( int i, int j, int k, int inside_i );
  };

#endif // NURBSCONVEXHULL_H
