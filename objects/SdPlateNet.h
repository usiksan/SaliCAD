#ifndef SDPLATENET_H
#define SDPLATENET_H

#include "SdPoint.h"
#include "SdStratum.h"
#include <QMap>
#include <QList>
#include <QVector>


//Distance from one sub net to another
struct SdSubNetDistance {
    int     mDestSubNet; //Id of another sub net
    double  mDistance;   //Distance to another sub net
    SdPoint mSour;       //Point of this sub net
    SdPoint mDest;       //Point of another sub net

    //Default constructor creates empty distance. We dont use it
    SdSubNetDistance() : mDestSubNet(-1),mDistance(0),mSour(),mDest() {}

    //Constructor creates distance with all fields
    SdSubNetDistance( int destSubNet, double dist, SdPoint s, SdPoint d ) : mDestSubNet(destSubNet), mDistance(dist), mSour(s), mDest(d) {}
  };



//Sub net contains distances to all another sub nets
class SdSubNet : public QMap<int,SdSubNetDistance> {
  public:
    //Find minimal distance
    SdSubNetDistance findMinimalDistance();


    //Add distance from this sub net to another (dest) sub net
    //Calculate distance and call addDistance( int mDestSubNet, double distance, SdPoint sour, SdPoint dest );
    void             addDistance( int mDestSubNet, SdPoint sour, SdPoint dest );

    //Add distance from this sub net to another (dest) sub net
    //When addition we check if distance is minimal
    void             addDistance( int mDestSubNet, double distance, SdPoint sour, SdPoint dest );

    //Add all distances from net to current sub net. When addition we check if distance is minimal
    void             addSubNet( int curSubNet, const SdSubNet &net );
  };



//Vertex point for net
struct SdPlateNetPoint {
    SdStratum mStratum; //Stratum of point. Define flat form or throwput
    SdPoint   mOrigin;  //Origin of vertex
    int       mSubNet;  //Sub net index
  };



//Rat net pair define point pair witch is unconnected link
struct SdRatNetPair {
    SdPoint a,b;

    SdRatNetPair() : a(), b() {}
    SdRatNetPair( SdPoint sa, SdPoint sb ) : a(sa), b(sb) {}
  };

typedef QVector<SdRatNetPair> SdRatNet;


class SdPlateNet
  {
    QMap<int,SdSubNet>       mSubNetList;    //Sub net distance beatween each to antoher
    QVector<SdPlateNetPoint> mNetPointList;  //Vertex point list for net
  public:
    SdPlateNet();

    int addNetPoint( SdStratum s, SdPoint org, int subNet );

    void buildRatNet( SdRatNet *ratNet );

  private:
    //For each points if point subnet equals oldSubNet then replace on newSubNet
    void replaceSubNet( int newSubNet, int oldSubNet );
  };


//List of plate nets
class SdPlateNetList {
    QMap<QString,SdPlateNet*> mNetList;
  public:
    SdPlateNetList();
    ~SdPlateNetList();

    int  addNetPoint(const QString netName, SdStratum s, SdPoint org , int subNetIndex = 0);

    void buildRatNet( SdRatNet *ratNet );
  };

#endif // SDPLATENET_H
