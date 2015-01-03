#include "ClassBall.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>
#include <SFML/Graphics.hpp>

void Gravity (TypeBall& ThisBall, float currentTime);
bool CheckBoundCollision (TypeBall& ThisBall, TypeCartesian BoxSize);

int main ()
{
    using namespace sf;
    using namespace std;

    Clock TheClock;
    float CurrentTime=0.0, TimeTail=0.5;
    int FPSrate=2, Population=50;
    TypeCartesian wSize={800,500};

    RenderWindow MainWindow; //Declare an sfml render window
    MainWindow.create(VideoMode(wSize.X,wSize.Y), "Just a circle");

    vector <TypeBall*> Balls;
    srand(time(NULL));

    for (int i=0;i<Population;i++)
    {
        float Speed=(rand()%100)/10.0, Dir=(rand()%1200-600)/10.0, Xp=(rand()%20)/10.0, Yp=(rand()%20-20)/10.0;
        //cout<<Speed<<" "<<Dir<<" "<<Xp<<" "<<Yp<<"\n";
        TypeVec TempVel (Speed,Dir, Xp,Yp);
        TypeBall* OneBall = new TypeBall(15, TempVel);
        OneBall->setFillColor(80,80,80);
        OneBall->setOutlineColor(200,100,50);
        Balls.push_back(OneBall);
    }

    TheClock.restart();

    //Everything happens within this loop
    while (MainWindow.isOpen()==true)
    {
        Event Sensed;   //Create an object of type "Event" and give it a name: "Sensed"

        //CurrentTime=TheClock.getElapsedTime().asSeconds();
        CurrentTime=CurrentTime+0.00025;

        MainWindow.clear();
        for (int i=0;i<Population;i++)
        {
            TypeCartesian Pos=Balls[i]->getPosition();
            Gravity(*(Balls[i]), CurrentTime);
            CheckBoundCollision (*(Balls[i]), wSize);
            MainWindow.draw(Balls[i]->getSFMLcircle());
        }
        MainWindow.display();

        //Handle events
        while (MainWindow.pollEvent(Sensed)==true) {if (Sensed.type == Event::Closed) MainWindow.close();}
    }//end while isOpen

    for (int i=0;i<Population;i++) {delete Balls[i]; } cout<<"\nFinished\n";

    return 0;
}

void Gravity (TypeBall& ThisBall, float CurrentTime)
{
    const float g=9.8;

    TypeVec OldVelocity=ThisBall.getVelocity();
    TypeCartesian NewPos, NewVel, PixelPos, OldPos;

    float TimeDif=CurrentTime-OldVelocity.getTimeStamp();
    float InitSpeed=OldVelocity.getMagnitude();
    float InitDir=OldVelocity.getRadsDirection();

    OldPos=OldVelocity.getOrigin();

    //This is in meters.
    NewPos.X=OldPos.X+(InitSpeed*cos(InitDir))*TimeDif;
    NewPos.Y=OldPos.Y+(InitSpeed*sin(InitDir))*TimeDif-0.5*g*TimeDif*TimeDif;

    NewVel.X=(InitSpeed*cos(InitDir));
    NewVel.Y=(InitSpeed*sin(InitDir))-g*TimeDif;

    ThisBall.setFromCartesian(NewVel,NewPos,CurrentTime);
}

bool CheckBoundCollision (TypeBall& ThisBall, TypeCartesian BoxSize)
{
    bool Result=false;
    TypeCartesian BallPos=ThisBall.getPixelPosition();
    TypeVec BallVelocity=ThisBall.getVelocity();
    float IncidentDir=BallVelocity.getDirection();
    float Speed=BallVelocity.getMagnitude();
    float ReflectedDir;
    float Elast=0.9;

    if(BallPos.Y>BoxSize.Y-30)
        {
            BallPos.Y=BoxSize.Y-30;
            ReflectedDir=BallVelocity.ReflectionAngle(0.0);
            Result=true;
        }
        else if (BallPos.X>BoxSize.X-30)
        {
            BallPos.X=BoxSize.X-30;
            ReflectedDir=BallVelocity.ReflectionAngle(90.0);
            Result=true;
        }
        else if (BallPos.X<0)
        {
            BallPos.X=0;
            ReflectedDir=BallVelocity.ReflectionAngle(-90.0);
            Result=true;
        }

        if(Result==true)
        {
            BallVelocity.setDiraction(ReflectedDir);
            BallVelocity.setMagnitude(Speed*Elast);
            ThisBall.setVelocity(BallVelocity);
            ThisBall.setPixelPosition(BallPos);
        }

    return Result;
}

