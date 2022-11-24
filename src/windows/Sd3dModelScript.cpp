#include "Sd3dModelScript.h"

#include <QObject>

const char16_t *scriptSmdResistor =
uR"VVV(
#f?w
w = inputFloat( "part width", 0.8 )
#f?l
l = inputFloat( "part lenght", 1.5 )
#f?h
h = inputFloat( "part height", 0.45 )
#f?pl
pl = inputFloat( "pin lenght", 0.2)
#f?sh
sh = inputFloat( "step height", 0.01 )
#f?pdist
pdist = inputFloat( "pin distance", 1.6 )
#c?bodyColor
bodyColor = inputColor( "body color", "#d3d7cf" )
#c?topColor
topColor = inputColor( "top color", "#3e3436" )
#c?pinColor
pinColor = inputColor( "pin color", "#f3f3f3" )

#p?pinPad
pinPad = inputPad( "pin pad", "r0.7x0.8m0.1s0.04" )

#pin height
ph = h - sh

#half width
hw = w / 2

#half lenght
hl = l / 2

#top lenght
tl = l - 2 * pl

#Indexes of vertex
#      3+---------+4
#       |         |
# 1 +---+ 2      5+---+6
#   |                 |
# 0 +-----------------+7

v0 = vertex( -hl, -hw, 0 )
frontFace = faceFlat( v0, 2, [ 0,ph,  pl,0, 0,sh, tl,0, 0,-sh, pl,0, 0,-ph, -pl,0, -tl,0 ] )
backFace = faceDuplicateShift( frontFace, -w )
contour = faceListWall( frontFace, backFace, true )

pinTops = faceListIndexed( contour, [1,5] )
bodyTop = faceListIndexed( contour, [3] )
contour = faceListIndexed( contour, [0,2,4,6,7] )

partFlat = graphRect( [-hl,-hw], [hl,hw] )

body = [frontFace, backFace]
body = faceListAppend( body, contour )

partModel = model( topColor, topColor, topColor, bodyTop, matrix1 )
partModel = model( pinColor, pinColor, pinColor, pinTops, matrix1 )
partModel = model( bodyColor, bodyColor, bodyColor, body, matrix1 )

#-
hl = pdist / 2
#-
partFlat = graphPin( [-hl,0], pinPad, [-hl,hw/4], "1", "CB0", [-hl,-hw/4], "CT0" )
#-
partFlat = graphPin( [hl,0], pinPad, [hl,hw/4], "2", "CB0", [hl,-hw/4], "CT0" )

)VVV";














const char16_t *scriptLeadedCapacitor =
uR"VVV(

#Capasitor model
#f?bodyDiametr
bodyDiametr = inputFloat( "Body diametr", 16 )
#f?bodyHeight
bodyHeight = inputFloat( "Body height", 36 )
#f?rubberRadius
rubberRadius = inputFloat( "Rubber radius", 0.8 )
#f?rubberOffsetZ
rubberOffsetZ = inputFloat( "Rubber offset Z", 0.7 )
bodyBevel = 0.2
vinylThickness = 0.3
#c?alumColor
alumColor = inputColor( "Aluminium color", "#a0a0a0" )
#c?bodyColor
bodyColor = inputColor( "Body vinyl color", "#0b66b3" )
#c?stripeColor
stripeColor = inputColor( "Strip for minus mark color", "#9aeaf9" )

#Second place source params of pins
#f?pinDiametr
pinDiametr = inputFloat( "Pin diametr", 0.7 )
pinLenght = 3
#f?pinDistance
pinDistance = inputFloat( "Distance between pins", 7.5 )
#c?pinColor
pinColor = inputColor( "Pin color", "#d0d0d0" )

#-
padPlus = inputPad( "Pad plus", stringPadRectThrough(1.0,1.0,pinDiametr*1.2,0.1) )
#-
padMinus = inputPad( "Pad minus", "c1.2d0.6m0.1" )


#========================================================
#Body construction
#Construct body faces. See help on embedded functions
bodyRadius = bodyDiametr / 2
path = [
  bodyRadius-vinylThickness,0,
  arc( bodyBevel, 180, 90, 10),
  0,rubberOffsetZ,
#Rubber
  arc( rubberRadius, 90, 45, 10 ),
  arc( rubberRadius, -180+45, -45, 10 ),
  arc( rubberRadius, 180-45, 90, 10 ),
#Main body
  0,bodyHeight-3*rubberRadius-2*bodyBevel,
  arc( bodyBevel, 90, 0, 10 ),
  -vinylThickness,0,
  0,-vinylThickness ]
layers = faceListRotation( 15, 345, 30, matrix1, path )
stripe = faceListRotation( -15, 15, 10, matrix1, path )

topbot = faceListIndexed( layers, [0, -1] ) + faceListIndexed( stripe, [0, -1] )

bodyMatrix = matrixTranslate( matrix1, pinDistance/2, 0, 0 )
#Model is set of faces with associated with it colors
bodyModel = model( bodyColor, bodyColor, bodyColor, faceListWalls( layers, false ), bodyMatrix )
bodyModel = modelAppend( bodyModel, stripeColor, stripeColor, stripeColor, faceListWalls( stripe, false ) )
bodyModel = modelAppend( bodyModel, alumColor, alumColor, alumColor, topbot )

#Place model into destignation model. After this you can see your construction
partModel = bodyModel

#=========================================================
#Pin construction
pinTopFace = faceCircle( pinDiametr/2, 10, matrix1 )
pinBotFace = faceDuplicateShift( pinTopFace, -pinLenght )

pin = [pinBotFace]
pin = faceListUnion( pin, faceListWall( pinTopFace, pinBotFace, true ) )

#First pin placement
pinModel = model( pinColor, pinColor, pinColor, pin, matrix1 )

pinMatrix = matrixTranslate( matrix1, pinDistance, 0, 0 )
pinModel = modelCopy( pinModel, pinMatrix )

#Place pin model into destignation model
partModel = pinModel

#- 2D Graphics
partFlat = graphCircle( [ pinDistance/2, 0, bodyRadius ] )
#-
partFlat = graphPin( [0,0], padPlus, [0,0], "1", "CM0", [0,0], "CM0" )
#-
partFlat = graphPin( [pinDistance,0], padMinus, [pinDistance,0], "2", "CM0", [pinDistance,0], "CM0" )


)VVV";








const char16_t *scriptSoic =
uR"VVV(
#part dimensions
partWidth = inputFloat( "part width", 6 )

#body dimensions
bodyLenght = inputFloat( "body lenght", 6 )
bodyWidth = inputFloat( "body width", 4.4 )
bodyBevelFront = inputFloat( "body bevel front", 0.8 )
bodyBevelSide = inputFloat( "body bevel side", 0.2 )
bodyHeight = inputFloat( "body height", 1.75 )
bodyColor = inputColor( "body color", "#555555" )
markColor = inputColor( "mark color", "#808080" )

#pin dimensions
pinThickness = inputFloat( "pin thickness", 0.2 )
pinWidth = inputFloat( "pin width", 0.3 )
pinHeight = inputFloat( "pin height", bodyHeight / 2 )
pinLenght = inputFloat( "pin lenght", (partWidth - bodyWidth) / 2 )
pinPlateLenght = inputFloat( "pin plate lenght", 0.4 )
pinColor = inputColor( "pin color", "#f0f0f0" )

#pins
pinsFrontCount = inputFloat( "front side pin count", 4 )
pinsFrontDistance = inputFloat( "front side pin distance", 1.27 )
pinsRearCount = inputFloat( "rear side pin count", 4 )
pinsRearDistance = inputFloat( "rear side pin distance", 1.27 )

#body
bodyWidthLow = bodyWidth - 2 * bodyBevelSide
bodyBevelHeight = bodyHeight * 0.4
bodyMiddleHeight = bodyHeight * 0.2

leftFace = faceFlat( vertex(-bodyLenght/2.0, -bodyWidthLow / 2, 0), 1,
 [ bodyWidthLow,0, bodyBevelSide,bodyBevelHeight, 0,bodyMiddleHeight, -bodyBevelSide,bodyBevelHeight,
   -(bodyWidth - bodyBevelSide - bodyBevelFront),0, -bodyBevelFront,-bodyBevelHeight, 0,-bodyMiddleHeight] )

body = faceListExtrudeShift( leftFace, -bodyLenght )
bodyModel = model( bodyColor, bodyColor, bodyColor, body, matrix1 )

#first pin mark
markDiam = bodyWidth / 10.0
mark = faceCircle( markDiam/2.0, 10, matrix( 0,0,0,0, -bodyLenght / 2 + markDiam * 3, -bodyWidth / 2 + markDiam * 3, bodyHeight + 0.01) )
partModel = modelAppend( bodyModel, markColor, markColor, markColor, [mark] )

pinFace = faceRect( pinThickness, pinWidth, matrix1 )
pin = faceListPinCurveTwo( pinFace, (pinLenght - pinPlateLenght) * 0.9, pinHeight, pinPlateLenght, -pinThickness, 70, -70, 20 )

pinFrontOffset = -bodyLenght / 2 + (bodyLenght - (pinsFrontCount - 1) * pinsFrontDistance) / 2.0

pinMatrix = matrixTranslate( matrix1, pinFrontOffset, -bodyWidth / 2.0, pinHeight )
pinMatrix = matrixRotate( pinMatrix, -90, 1, 0, 0 )
pinMatrix = matrixRotate( pinMatrix, -90, 0, 0, 1 )

pinModel = model( pinColor, pinColor, pinColor, pin, pinMatrix )



i = 1
while( i < pinsFrontCount ) {
  pinMatrix = matrixTranslate( matrix1, pinFrontOffset + i * pinsFrontDistance, -bodyWidth / 2.0, pinHeight )
  pinMatrix = matrixRotate( pinMatrix, -90, 1, 0, 0 )
  pinMatrix = matrixRotate( pinMatrix, -90, 0, 0, 1 )

  pinModel = modelCopy( pinModel, pinMatrix )
  i = i + 1.0
  }

pinRearOffset = -bodyLenght / 2 + (bodyLenght - (pinsRearCount - 1) * pinsRearDistance) / 2.0

i = 0
while( i < pinsRearCount ) {
  pinMatrix = matrixTranslate( matrix1, pinRearOffset + i * pinsRearDistance, bodyWidth / 2.0, pinHeight )
  pinMatrix = matrixRotate( pinMatrix, 90, 1, 0, 0 )
  pinMatrix = matrixRotate( pinMatrix, 90, 0, 0, 1 )

  pinModel = modelCopy( pinModel, pinMatrix )
  i = i + 1.0
  }

partModel = pinModel

)VVV";






const char16_t *scriptModule =
uR"VVV(
# First place source params of part
#f?bodyLenght
bodyLenght = inputFloat( "Plate length (X)", 13 )

#f?bodyWidth
bodyWidth = inputFloat( "Plate width (Y)", 27 )

#f?bodyHeight
bodyHeight = inputFloat( "Plate height (H)", 1 )

#c?bodyColor
bodyColor = inputColor( "Plate color", "#008080" )


# Second place source params of pins
#f?pinDiametr
pinDiametr = inputFloat( "Pin width", 1.2 )

#f?pinHoleDiametr
pinHoleDiametr = inputFloat( "Pin hole diameter", 1 )

#f?pinDistance
pinDistance = inputFloat( "Pin distance", 1.5 )

#f?pinCountLeft
pinCountLeft = inputFloat( "Pin count at left", 13 )

#f?pinCountBottom
pinCountBottom = inputFloat( "Pin count at bottom", 8 )

#f?pinCountRight
pinCountRight = inputFloat( "Pin count at right", 12 )

#f?pinOffsetLeftY
pinOffsetLeftY = inputFloat( "Pin offset at left on Y", 1.0 )

#f?pinOffsetBottomX
pinOffsetBottomX = inputFloat( "Pin offset at bottom on X", 0 )

#f?pinOffsetRightY
pinOffsetRightY = inputFloat( "Pin offset at right on Y", 1.0 )

# -2 bottom, 0 center, 2 top
#f?pinLeftAlign
pinLeftAlign = inputFloat( "Pins alignment at left (-2 bottom, 0 center, 2 top)", -2.0 )

# -2 left, 0 center, 2 right
#f?pinBottomAlign
pinBottomAlign = inputFloat( "Pins alignment at bottom (-2 left, 0 center, 2 right)", 0 )

# -2 bottom, 0 center, 2 top
#f?pinRightAlign
pinRightAlign = inputFloat( "Pins alignment at right (-2 bottom, 0 center, 2 top)", -2.0 )

#c?pinColor
pinColor = inputColor( "Pin color", "#edd400" )

#-
padLeft = inputPad( "Pad left", "r2x1.2m0.1s0.04" )

#-
padBottom = inputPad( "Pad bottom", "r1.2x2m0.1s0.04" )

#-
padRight = inputPad( "Pad right", "r2x1.2m0.1s0.04" )


#Third place source params of cover
#f?coverLength
coverLength = inputFloat( "Cover length (X)", 10.5 )

#f?coverWidth
coverWidth = inputFloat( "Cover width (Y)", 16.5 )

#f?coverHeight
coverHeight = inputFloat( "Cover height (H)", 2 )

#f?coverOffsetCenter
coverOffsetCenter = inputFloat( "Cover offset on horizontal center (X)", 0 )

#f?coverOffsetBottom
coverOffsetBottom = inputFloat( "Cover offset on vertical bottom (Y)", 1.75 )

#c?coverColor
coverColor = inputColor( "Pin color", "#f3f3f3" )


#========================================================
#Body construction
pinOutHoleHalf = (pinDistance - pinHoleDiametr) / 2.0
pinHoleRadius = pinHoleDiametr / 2.0

#Left side
#Left pins length
leftPinsLength = pinDistance * pinCountLeft
#Center align
leftPinsStartY = (bodyWidth - leftPinsLength) / 2.0 + pinOffsetLeftY
if( pinLeftAlign < -1.0 )
  #Bottom align
  leftPinsStartY = bodyWidth - leftPinsLength - pinOffsetLeftY

if( pinLeftAlign > 1 )
  #Top align
  leftPinsStartY = pinOffsetLeftY

leftPinsStopY = bodyWidth - leftPinsStartY - leftPinsLength

#Left side of bottom body
bodyBottomVertex = [ 0,bodyWidth ]
if( leftPinsStartY > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [ 0,-leftPinsStartY] )

i = 0
while( i < pinCountLeft ) {
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [ 0,-pinOutHoleHalf, pinHoleRadius,0, 0,-pinHoleDiametr,
      -pinHoleRadius,0, 0,-pinOutHoleHalf] )
  i = i + 1.0
  }
if( leftPinsStopY > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [0,-leftPinsStopY] )


#Bottom side
bottomPinsLength = pinDistance * pinCountBottom
#Center align
bottomPinsStartX = (bodyLenght - bottomPinsLength) / 2.0 + pinOffsetBottomX
if( pinBottomAlign < -1 )
  #Left align
  bottomPinsStartX = pinOffsetBottomX

if( pinBottomAlign > 1 )
  #Right align
  bottomPinsStartX = bodyLenght - bottomPinsLength - pinOffsetBottomX

bottomPinsStopX = bodyLenght - bottomPinsStartX - bottomPinsLength

#Bottom side of bottom body
if( bottomPinsStartX > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [bottomPinsStartX,0] )

i = 0
while( i < pinCountBottom ) {
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [ pinOutHoleHalf,0, 0,pinHoleRadius, pinHoleDiametr,0,
      0,-pinHoleRadius, pinOutHoleHalf,0] )
  i = i + 1.0
  }
if( bottomPinsStopX > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [bottomPinsStopX,0] )


#Right side
rightPinsLength = pinDistance * pinCountRight
#Center align
rightPinsStartY = (bodyWidth - rightPinsLength) / 2.0 + pinOffsetRightY
if( pinRightAlign < 0 )
  #Bottom align
  rightPinsStartY = bodyWidth - rightPinsLength - pinOffsetLeftY

if( pinRightAlign > 0 )
  #Top align
  rightPinsStartY = pinOffsetRightY

rightPinsStopY = bodyWidth - rightPinsStartY - rightPinsLength

#Right side of bottom body
if( leftPinsStopY > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [ 0,rightPinsStopY] )

i = 0
while( i < pinCountRight ) {
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [ 0,pinOutHoleHalf, -pinHoleRadius,0, 0,pinHoleDiametr,
      pinHoleRadius,0, 0,pinOutHoleHalf] )
  i = i + 1.0
  }
if( rightPinsStartY > 0 )
  bodyBottomVertex = floatListAppend( bodyBottomVertex, [0,rightPinsStartY] )

#Build bottom face
#Construct body faces. See help on embedded functions
bodyBottomFace = faceFlatMatrix( bodyBottomVertex, matrix1 )
bodyTopFace = faceDuplicate( bodyBottomFace, matrixTranslate( matrix1, 0, 0, bodyHeight) )

#Set of faces with same color
body = faceListSimplify( bodyBottomFace )
body = faceListAppend( body, faceListSimplify( bodyTopFace ) )
body = faceListAppend( body, faceListWall( bodyBottomFace, bodyTopFace, true ) )

#Model is set of faces with associated with it colors
bodyModel = model( bodyColor, bodyColor, bodyColor, body, matrix1 )

#Place model into destignation model. After this you can see your construction
partModel = bodyModel


#- 2D Graphics
partFlat = graphRect( [ 0, 0], [bodyLenght, bodyWidth] )
#-
gCoverX = (bodyLenght - coverLength) / 2 + coverOffsetCenter
#-
partFlat = graphRect( [ gCoverX, coverOffsetBottom ], [gCoverX+coverLength,coverOffsetBottom+coverWidth] )
#-
gLineY = 2.0 * coverOffsetBottom + coverWidth
#-
partFlat = graphLine( [ 0, gLineY ], [bodyLenght,gLineY] )
#-
partFlat = graphCircle( [ gCoverX + 2, coverOffsetBottom+coverWidth-2, 1 ] )

#=========================================================
#Pin construction

pinBottomArcVertex = arc( pinHoleRadius, -90, 90, 10 )

pinDiametrHalf = pinDiametr / 2.0

pinOutHalf = (pinDiametr - pinHoleDiametr) / 2.0

pinBottomVertex = [ -pinDiametrHalf,0, pinOutHalf,0, pinBottomArcVertex, pinOutHalf,0,
  0,pinDiametrHalf, -pinDiametr,0 ]

pinBottomFace = faceFlatMatrix( pinBottomVertex, matrixTranslate( matrix1, 0,0, -0.001 ) )
pinTopFace = faceFlatMatrix( pinBottomVertex, matrixTranslate( matrix1, 0,0, bodyHeight + 0.001 ))

pin = faceListSimplify( pinBottomFace )
pin = faceListAppend( pin, faceListSimplify( pinTopFace ) )

pinBottomVertex = [ -pinHoleRadius,0, pinBottomArcVertex ]

pinBottomFace = faceFlatMatrix( pinBottomVertex, matrixTranslate( matrix1, 0,0, -0.001 ) )
pinTopFace = faceFlatMatrix( pinBottomVertex, matrixTranslate( matrix1, 0,0, bodyHeight + 0.001 ))

pin = faceListAppend( pin, faceListWall( pinBottomFace, pinTopFace, false ) )

pinDistanceHalf = pinDistance / 2.0

pinPosY = bodyWidth - (leftPinsStartY + pinDistanceHalf)
pinMatrix = matrixTranslate( matrix1, 0, pinPosY, 0 )
pinMatrix = matrixRotate( pinMatrix, -90, 0, 0, 1.0 )
#First pin placement
pinModel = model( pinColor, pinColor, pinColor, pin, pinMatrix )

pinIndex = 1
#-
partFlat = graphPin( [0,pinPosY], padLeft, [0,pinPosY+1], stringPinIndex(pinIndex), "CM0", [1,pinPosY], "CM0" )


#Left side pins
i = 1
while( i < pinCountLeft ) {
  pinPosY = bodyWidth - (leftPinsStartY + pinDistanceHalf + i * pinDistance)
  pinMatrix = matrixTranslate( matrix1, 0, pinPosY, 0 )
  pinMatrix = matrixRotate( pinMatrix, -90, 0, 0, 1.0 )
  pinModel = modelCopy( pinModel, pinMatrix )
  i = i + 1
  pinIndex = pinIndex + 1
#-
  partFlat = graphPin( [0,pinPosY], padLeft, [0,pinPosY+1], stringPinIndex(pinIndex), "CM0", [1,pinPosY], "CM0" )
  }

#Bottom side pins
pinPosX = 0
i = 0
while( i < pinCountBottom ) {
  pinPosX = bottomPinsStartX + pinDistanceHalf + i * pinDistance
  pinMatrix = matrixTranslate( matrix1, pinPosX, 0, 0 )
  pinModel = modelCopy( pinModel, pinMatrix )
  i = i + 1
  pinIndex = pinIndex + 1
#-
  partFlat = graphPin( [pinPosX,0], padBottom, [pinPosX,-1], stringPinIndex(pinIndex), "CM0", [pinPosX,1], "CM0" )
  }

#Right side pins
pinIndex = pinIndex + pinCountRight
i = 0
while( i < pinCountRight ) {
  pinPosY = bodyWidth - (rightPinsStartY + pinDistanceHalf + i * pinDistance)
  pinMatrix = matrixTranslate( matrix1, bodyLenght, pinPosY, 0 )
  pinMatrix = matrixRotate( pinMatrix, 90, 0, 0, 1.0 )
  pinModel = modelCopy( pinModel, pinMatrix )
  i = i + 1
#-
  partFlat = graphPin( [bodyLenght,pinPosY], padRight, [bodyLenght,pinPosY+1], stringPinIndex(pinIndex), "CM0", [bodyLenght-1,pinPosY], "CM0" )
  pinIndex = pinIndex - 1
  }

#Place pin model into destignation model
partModel = pinModel


#=========================================================
#Cover construction
cover = faceListBox( coverLength, coverWidth, -coverHeight )

partModel = model( coverColor, coverColor, coverColor, cover, matrixTranslate( matrix1, bodyLenght / 2.0 + coverOffsetCenter, coverWidth / 2.0 + coverOffsetBottom, bodyHeight ) )



)VVV";






Sd3dModelScript::Sd3dModelScriptList Sd3dModelScript::scriptList()
  {
  static Sd3dModelScriptList list;

  if( list.count() == 0 ) {
    Sd3dModelScript ms;
    ms.mName = QObject::tr("smd resistor 3d model");
    ms.mDescription = QObject::tr("This master creates smd resistor 3d part model");
    ms.mScript = QString::fromUtf16( scriptSmdResistor );
    list.append( ms );

    ms.mName = QObject::tr("leaded polar electrolytic capacitor");
    ms.mDescription = QObject::tr("This master builds 3d model for polar electrolytic capacitor");
    ms.mScript = QString::fromUtf16( scriptLeadedCapacitor );
    list.append( ms );

    ms.mName = QObject::tr("soic 2-row chip");
    ms.mDescription = QObject::tr("This master builds 3d model of soic chip with 2 rows of pins");
    ms.mScript = QString::fromUtf16( scriptSoic );
    list.append( ms );

    ms.mName = QObject::tr("module on pcb");
    ms.mDescription = QObject::tr("This master builds 3d model and 2d graphics for pcb modules like esp32 wifi");
    ms.mScript = QString::fromUtf16( scriptModule );
    list.append( ms );
    }
  return list;
  }
