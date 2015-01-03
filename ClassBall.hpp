#ifndef CuteBall0101110_h
#define CuteBall0101110_h

#include <SFML/Graphics.hpp>
#include <vector>

struct TypeCartesian { float X; float Y; };
struct TypeRGB { int R; int G; int B; };

class TypeVec
{
private:
    //Private variables
    float Magn=0;
    float Dirct=0;                  //in degrees
    float TimeStamp=0.0;            //since a force last acted on the object
    TypeCartesian VecOrigin={0,0};  //position of vector origin

    //Private Methods
    float ToRad (float Deg);
    float ToDeg (float Rad);
    float CleanDir (float ThisDir, bool isPlane=false);

public:
    //Consrt/Destr
    TypeVec();
    TypeVec (float Mag, float Dir, float Xpos, float Ypos, float NewTimeStamp=0.0);
    ~TypeVec();

    //METHODS
    float ReflectionAngle (float PlaneAngle);
    float OpositeDir ();
    float CalcDirection (TypeCartesian Component);
    float CalcDirectionToPoint (TypeCartesian DestPoint);
    float CalcDistanceToPoint (TypeCartesian DestPoint);
    TypeVec CalcVectorToPoint (TypeCartesian DestPoint);

    //SET methods
    void setMagnitude (float Mag);
    void setDiraction (float Dir);
    void setFromCartesian (TypeCartesian NewVelocity, TypeCartesian NewPosition, float atTime=0.0);
    void setTimeStamp (float ThisTime);
    void setOrigin (TypeCartesian Origin);
    void setOrigin (float Xpos, float Ypos);

    //GET methods
    float getMagnitude ();
    float getDirection ();
    float getRadsDirection ();
    float getTimeStamp ();
    TypeCartesian getCartesianVel();
    TypeCartesian getOrigin ();

    //Operator overloading methods -> for vector arithmitic
    TypeVec operator+ (TypeVec& ThatVec);
    TypeVec operator- (TypeVec& ThatVec);
    //bool operator> (TypeVec& ThisVec);
    //bool operator< (TypeVec& ThisVec);
    //bool operator== (TypeVec& ThisVec);
    //TypeVec operator* (TypeVec& ThisVec); //Dot product I suppose
};

class TypeBall
{
private:
    //Private variables
    float   radius;
    int     sides;

    TypeVec CurVelocity;            //mathematical vector; contains Magnitude, Direction, Origin, and TimeStamp
    TypeVec TotalForce;
    std::vector <TypeVec> Acceler;  //Acceleration vectors m/s^2

    float   Elasticity;
    float   Mass;                   //Kg

    TypeRGB fillColor;
    TypeRGB outlineColor;

    sf::CircleShape sfmlCircle;

public:
    //Constructors
    TypeBall();
    TypeBall(float Radius);
    TypeBall(float Radius, TypeVec CurVel);

    //set Methods
    void setVelocity (TypeVec Velocity);
    void setSpeed (float VelMagnitude);
    void setPosition (TypeCartesian NewPos);
    void setPixelPosition (TypeCartesian PixPos);
    void setRadius (float NewRadius);
    void setSides (int NewCount);
    void setFillColor (unsigned short R, unsigned short G, unsigned short B);
    void setOutlineColor (unsigned short R, unsigned short G, unsigned short B);
    void setFromCartesian (TypeCartesian NewVelocity, TypeCartesian NewPosition, float atTime=0.0);

    void setSFMLCirclePref(float CirRadius, unsigned int Sides, TypeRGB fillcolor, TypeRGB OutlineColor, float OutlineWidth=1.0);

    //get Methods
    float getRadius ();
    float getSpeed ();
    TypeCartesian getPosition();
    TypeCartesian getPixelPosition();
    TypeVec getVelocity ();
    sf::CircleShape getSFMLcircle();

    TypeVec CalcDistanceToPoint (TypeCartesian Point);
    TypeCartesian PixelPosition (TypeCartesian MeterPos);
    TypeCartesian MeterPosition (TypeCartesian PixPos);
    void showData ();
};

#endif // CuteBall0101110_h
