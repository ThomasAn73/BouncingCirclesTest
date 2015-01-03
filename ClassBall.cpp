#include "ClassBall.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

#define pi 3.141592654

//***TYPEVEC CLASS implementation*****************************************************************************
//************************************************************************************************************

//---CONSTRUCTORS---------------------------------------------------------------------------------------------
TypeVec::TypeVec () { Dirct=0; Magn=0; TimeStamp=0.0; VecOrigin.X=0.0; VecOrigin.Y=0.0;}

TypeVec::TypeVec (float Mag, float Dir, float Xpos, float Ypos, float atTime)
{
    setDiraction(Dir);
    setMagnitude(Mag);
    setTimeStamp(atTime);
    setOrigin(Xpos, Ypos);
}

TypeVec::~TypeVec () {}


//---MEMBER functions-----------------------------------------------------------------------------------------

float TypeVec::ToRad (float Deg) { return 2.0 * pi * Deg/360.0; }

float TypeVec::ToDeg (float Rad) { return 360.0 * Rad / (2.0*pi); }

float TypeVec::CleanDir (float ThisDir, bool isPlane)
{
    if (std::abs(ThisDir)>360) {ThisDir=fmod(ThisDir,360.0);} //Put the Dirct back in the -360 to 360 range
    if (ThisDir<0) {ThisDir=360+ThisDir;}                     //Eliminate negative angles
    if (isPlane==true && ThisDir>=180) {ThisDir=ThisDir-180;}

    return ThisDir;
}

float TypeVec::OpositeDir()
{
    float Result;
    Result=Dirct+180.0;
    if (std::abs(Result)>360) Result=fmod(Result,360.0);

    return Result;
}

float TypeVec::ReflectionAngle (float PlaneAngle)
{
    float IncidentAngle, Result;
    Dirct=CleanDir(Dirct);
    PlaneAngle=CleanDir(PlaneAngle, true);
    IncidentAngle=Dirct-PlaneAngle;

    //the Dirct angle is always in reference to horizontal
    Result=-IncidentAngle + PlaneAngle;
    Result=CleanDir(Result);
    //std::cout<<"oldDir:"<<IncidentAngle<<" NewDir:"<<Result<<"\n";
    return Result;
}

//Direction angle in degrees from cartesian components
float TypeVec::CalcDirection (TypeCartesian Component)
{
    float Result;

    if (Component.X==0 && Component.Y>=0) {Result=acos(0.0);}
    else if (Component.X==0 && Component.Y<=0) {Result=-acos(0.0);}
    else if (Component.X>0 && Component.Y>=0) Result=atan(Component.Y/Component.X);
    else if (Component.X<0 && Component.Y<=0) Result=atan(Component.Y/Component.X)+ToRad(180);
    else if (Component.X<0 && Component.Y>=0) Result=atan(Component.Y/Component.X)+ToRad(180);
    else Result=atan(Component.Y/Component.X);

    Result=ToDeg(Result);
    return CleanDir(Result);
}

//Direction angle in degrees; given the current origin point and a destination point
float TypeVec::CalcDirectionToPoint (TypeCartesian DestPoint)
{ return CalcVectorToPoint(DestPoint).getDirection(); }

float TypeVec::CalcDistanceToPoint (TypeCartesian DestPoint)
{ return CalcVectorToPoint(DestPoint).getMagnitude(); }

TypeVec TypeVec::CalcVectorToPoint (TypeCartesian DestPoint)
{
    TypeVec ResultVec;
    float ResultDistance, Direction;
    TypeCartesian Distance;

    Distance.X=DestPoint.X-VecOrigin.X;
    Distance.Y=DestPoint.Y-VecOrigin.Y;

    ResultDistance=sqrt(Distance.X*Distance.X+Distance.Y*Distance.Y);
    Direction=CalcDirection(Distance);

    ResultVec.setDiraction(Direction);
    ResultVec.setMagnitude(ResultDistance);

    return ResultVec;
}
/*
float TypeVec::CalcDirectionToPoint (TypeCartesian DestPoint)
{
    float Result;
    TypeCartesian Distance;

    Distance.X=DestPoint.X-VecOrigin.X;
    Distance.Y=DestPoint.Y-VecOrigin.Y;

    Result=CalcDirection(Distance);
    return Result;
}*/

//---SET methods----------------------------------------------------------------------------------------------
void TypeVec::setOrigin (TypeCartesian Origin) { VecOrigin=Origin; }

void TypeVec::setOrigin (float Xpos, float Ypos) { VecOrigin.X=Xpos; VecOrigin.Y=Ypos; }

void TypeVec::setMagnitude (float Mag)
{
    if (Mag>=0) {Magn=Mag;}
    else { Magn=-Mag; Dirct=OpositeDir(); }
}

void TypeVec::setFromCartesian (TypeCartesian NewVelocity, TypeCartesian NewPosition, float NewTimeStamp)
{
    Magn=sqrt(NewVelocity.X*NewVelocity.X + NewVelocity.Y*NewVelocity.Y); //This will always be positive (no need to call setMagnitude)
    Dirct=CalcDirection(NewVelocity);
    VecOrigin=NewPosition;
    TimeStamp=NewTimeStamp;
}

void TypeVec::setTimeStamp (float ThisTime) {if (ThisTime>=0) TimeStamp=ThisTime; else TimeStamp=0;}

void TypeVec::setDiraction (float Dir) { Dirct=CleanDir(Dir); }

//---GET methods-------------------------------------------------------------------------------------------
float TypeVec::getMagnitude () { return Magn; }

float TypeVec::getDirection () { return Dirct; }

float TypeVec::getTimeStamp () {return TimeStamp;}

float TypeVec::getRadsDirection () { return ToRad(Dirct); }

TypeCartesian TypeVec::getOrigin () { return VecOrigin; }

TypeCartesian TypeVec::getCartesianVel()
{
    TypeCartesian Result;
    Result.X=cos(ToRad(Dirct)) * Magn;
    Result.Y=sin(ToRad(Dirct)) * Magn;
    return Result;
}

TypeVec TypeVec::operator+ (TypeVec& ThatVec)
{
    TypeVec ResultVec;
    TypeCartesian ThisXYvel=getCartesianVel();
    TypeCartesian ThatXYvel=ThatVec.getCartesianVel();
    TypeCartesian NewXYvel;

    NewXYvel.X=ThisXYvel.X+ThatXYvel.X;
    NewXYvel.Y=ThisXYvel.Y+ThatXYvel.Y;

    ResultVec.setFromCartesian(NewXYvel, VecOrigin, TimeStamp);
    return ResultVec;
}

TypeVec TypeVec::operator- (TypeVec& ThatVec)
{
    TypeVec ResultVec;
    TypeCartesian ThisXYvel=getCartesianVel();
    TypeCartesian ThatXYvel=ThatVec.getCartesianVel();
    TypeCartesian NewXYvel;

    NewXYvel.X=ThisXYvel.X-ThatXYvel.X;
    NewXYvel.Y=ThisXYvel.Y-ThatXYvel.Y;

    ResultVec.setFromCartesian(NewXYvel, VecOrigin, TimeStamp);
    return ResultVec;
}

//***TYPEBALL CLASS implementation****************************************************************************
//************************************************************************************************************

//---CONSTRUCTORS---------------------------------------------------------------------------------------------
TypeBall::TypeBall ()
{
    radius=2; sides=4;

    setFillColor(0,0,0); setOutlineColor(0,0,255);
    setSFMLCirclePref(radius,sides,fillColor, outlineColor);
}

TypeBall::TypeBall (float CirRadius)
{
    if (CirRadius<=0) CirRadius=2;
    radius=CirRadius; sides=CirRadius*2;

    setFillColor(0,0,0); setOutlineColor(0,0,255);
    setSFMLCirclePref(radius,sides,fillColor, outlineColor);
}

TypeBall::TypeBall (float CirRadius, TypeVec CurVel)
{
    if (CirRadius<=0) CirRadius=2;
    radius=CirRadius; sides=CirRadius*2;
    CurVelocity=CurVel; //Memberwise copy; I hope.

    setFillColor(0,0,0); setOutlineColor(0,0,255);
    setSFMLCirclePref(radius,sides,fillColor, outlineColor);
}

//---GET methods------------------------------------------------------------------------------------
float TypeBall::getRadius () { return radius; }

float TypeBall::getSpeed () { return CurVelocity.getMagnitude(); }

TypeCartesian TypeBall::getPosition() { return CurVelocity.getOrigin(); }

TypeVec TypeBall::getVelocity () { return CurVelocity; }

TypeCartesian TypeBall::getPixelPosition()
{
    TypeCartesian MetPos=CurVelocity.getOrigin();
    return PixelPosition(MetPos);
}

sf::CircleShape TypeBall::getSFMLcircle() { return sfmlCircle; }

//---SET methods--------------------------------------------------------------------------------------------
void TypeBall::setPixelPosition (TypeCartesian PixPos)
{
    TypeCartesian MetPos=MeterPosition(PixPos);
    setPosition(MetPos);
}

void TypeBall::setSFMLCirclePref(float CirRadius, unsigned int Sides, TypeRGB fillcolor, TypeRGB OutlineColor, float OutlineWidth)
{
    sfmlCircle.setRadius (CirRadius);
    sfmlCircle.setPointCount (Sides);
    sfmlCircle.setOutlineThickness(OutlineWidth);
    sfmlCircle.setFillColor(sf::Color(fillColor.R,fillColor.G,fillColor.B));
    sfmlCircle.setOutlineColor(sf::Color(outlineColor.R,outlineColor.G,outlineColor.B));
}

void TypeBall::setPosition (TypeCartesian NewPos)
{
    CurVelocity.setOrigin(NewPos);              //Store position in meters

    TypeCartesian PixPos=PixelPosition(NewPos); //Convert meters to pixels
    sfmlCircle.setPosition(PixPos.X,PixPos.Y);
}

void TypeBall::setFromCartesian (TypeCartesian NewVelocity, TypeCartesian NewPosition, float atTime)
{
    CurVelocity.setFromCartesian(NewVelocity, NewPosition, atTime);

    TypeCartesian PixPos=PixelPosition(NewPosition); //Convert meters to pixels
    sfmlCircle.setPosition(PixPos.X,PixPos.Y);
    //std::cout<<"X:"<<PixPos.X<<" Y:"<<PixPos.Y<<"\n";
}

void TypeBall::setSpeed (float VelMagnitude)
{ CurVelocity.setMagnitude(VelMagnitude); }

void TypeBall::setVelocity (TypeVec Velocity)
    { CurVelocity=Velocity; }

void TypeBall::setFillColor (unsigned short R, unsigned short G, unsigned short B)
{
    fillColor.R=R; fillColor.G=G; fillColor.B=B;
    sfmlCircle.setFillColor(sf::Color(R,G,B));
}

void TypeBall::setOutlineColor (unsigned short R, unsigned short G, unsigned short B)
{
    outlineColor.R=R; outlineColor.G=G; outlineColor.B=B;
    sfmlCircle.setOutlineColor(sf::Color(R,G,B));
}

void TypeBall::setRadius (float NewRadius)
    {if(NewRadius>=2) {radius=NewRadius;} else {radius=2;} sfmlCircle.setRadius(radius);}

void TypeBall::setSides (int NewCount)
    { if(NewCount>=3) {sides=NewCount;} else {sides=3;} sfmlCircle.setPointCount(sides);}

//---general methods---------------------------------------------------------------------------------
TypeCartesian TypeBall::PixelPosition (TypeCartesian MeterPos)
{
    TypeCartesian PixelPos;
    const float PixPerMeter=5*(100/2.54);

    PixelPos.X=MeterPos.X*PixPerMeter;
    PixelPos.Y=-MeterPos.Y*PixPerMeter;

    return PixelPos;
}

TypeCartesian TypeBall::MeterPosition (TypeCartesian PixPos)
{
    TypeCartesian MetPos;
    const float PixPerMeter=5*(100/2.54);

    MetPos.X=PixPos.X/PixPerMeter;
    MetPos.Y=-PixPos.Y/PixPerMeter;

    return MetPos;
}

TypeVec TypeBall::CalcDistanceToPoint (TypeCartesian Point)
{ return CurVelocity.CalcVectorToPoint(Point); }

void TypeBall::showData()
{
    TypeCartesian Origin=CurVelocity.getOrigin();
    using namespace std;
    cout<<"R:"<<radius<<" P:"<<sides<<" Vel:"<<CurVelocity.getMagnitude()<<" Dir:"<<CurVelocity.getDirection();
    cout<<" Xpos:"<<Origin.X<<" Ypos:"<<Origin.Y<<endl;

}
