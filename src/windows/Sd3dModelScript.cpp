#include "Sd3dModelScript.h"

const char16_t *scriptSmdResistor =
uR"VVV(
w = inputFloat( "part width", 0.8 )
l = inputFloat( "part lenght", 1.5 )
h = inputFloat( "part height", 0.45 )
pl = inputFloat( "pin lenght", 0.2)
sh = inputFloat( "step height", 0.01 )
pdist = inputFloat( "pin distance", 1.6 )
bodyColor = inputColor( "body color", "#d3d7cf" )
topColor = inputColor( "top color", "#3e3436" )
pinColor = inputColor( "pin color", "#f3f3f3" )

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


hl = pdist / 2

partFlat = graphPin( [-hl,0], pinPad, [-hl,hw/4], "1",
"CB0", [-hl,-hw/4], "CT0" )

partFlat = graphPin( [hl,0], pinPad, [hl,hw/4], "2",
"CB0", [hl,-hw/4], "CT0" )

)VVV";



const char16_t *scriptLeadedCapacitor =
uR"VVV(
bodyDiametr = inputFloat( "Body diametr", 4 )
bodyHeight = inputFloat( "Body height", 4 )
bodyColor = inputColor( "Body color", "#4e9a06" )

pinDiametr = inputFloat( "Pin diametr", 0.5 )
pinDistance = inputFloat( "Distance between pins", 2.5 )
pinColor = inputColor( "Pin color", "#f0f0f0" )

padPlus = inputPad( "Pad plus", stringPadRectThrough(1.0,1.0,pinDiametr*1.2,0.1) )
padMinus = inputPad( "Pad minus", "c1.2d0.6m0.1" )

plusShow = inputFloat( "Show plus", 0 )

bodyRadius = bodyDiametr / 2

body = faceListCylinder( bodyRadius, bodyHeight )

pin = faceListCylinder( pinDiametr / 2, -2.0 )

partModel = model( bodyColor, bodyColor, bodyColor, body, matrix1 )


pinPlusX = -pinDistance / 2
pinMinusX = pinDistance / 2

pins = model( pinColor, pinColor, pinColor, pin, matrix( 0,0,0,0, pinPlusX,0,0 ) )
pins = modelCopy( pins, matrix( 0,0,0,0, pinMinusX,0,0 ) )
partModel = pins

partFlat = graphCircle( [ 0, 0, bodyRadius ] )

partFlat = graphPin( [pinPlusX,0], padPlus, [pinPlusX,0], "1", "CM0", [pinPlusX,0], "CM0" )

partFlat = graphPin( [pinMinusX,0], padMinus, [pinMinusX,0], "2", "CM0", [pinMinusX,0], "CM0" )

if( plusShow > 0 ) {
  #Draw plus sign
  plusCenter = -bodyRadius - 2
  partFlat = graphLine( [plusCenter, -1], [plusCenter, 1] )
  partFlat = graphLine( [plusCenter - 1,0], [plusCenter + 1, 0] )
  }

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



Sd3dModelScript::Sd3dModelScriptList Sd3dModelScript::scriptList()
  {

  }
