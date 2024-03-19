#define viewDistance 16

#include <unistd.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "/usr/include/SDL2/SDL.h"
#include <termios.h>
#include <fcntl.h>
#include <chrono>

//#include <ncurses.h>

const char newline = '\n';
int const screenHohe = 40;
int screenBreite = 120;

float PlayerX = 8.0f;
float PlayerY = 8.0f;
float PlayerA = 0;

int MapHohe = 16;
int MapBreite = 16;

float FOV = 3.14159/4.0;
char* display = new char[screenHohe*screenBreite];
char* HUI = new char[screenHohe*screenBreite];

const float rotateSpeed = 0.46f;//3.0f * 0.16f ;
char inputbuffer;


int main(){
    //Set terminal mode with termios.h to read directly without pressing enter

    struct termios old,new1;

    tcgetattr(0,&old);
    new1 = old;
    new1.c_lflag &= ~ICANON;
    new1.c_lflag &= ~ECHO;
    //new1.c_cc[VEOL] = 1;
    //new1.c_cc[VEOF] = 2;
    tcsetattr(0,TCSANOW, &new1);

    //turn on nonblocking
    //  int flags = fcntl(0, F_GETFL, 0); /* get current file status flags */   
    //  flags |= O_NONBLOCK;          /* turn off blocking flag */
    //  fcntl(0, F_SETFL, flags); 

    std::wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#####..........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#...#..........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################"; 

    auto time1 = std::chrono::system_clock::now();
    auto time2 = std::chrono::system_clock::now();
    
    printf("Press W,A,S or D to render the view and update it\n you also move the camera with W,A,S,D\n");

    while(1)
    {
        // set a stable time with chrono
        auto time2 = std::chrono::system_clock::now();

        std::chrono::duration<float> elapseTime = time2-time1;
        float timeDif = elapseTime.count(); 
        //keyboard input
        read(0, &inputbuffer, 1);

        //check input
        if(inputbuffer == 'a'){
            PlayerA -= 0.03;
        }
        else if(inputbuffer == 'd'){
            PlayerA += 0.03;
        }
        else if(inputbuffer == 'w'){
            PlayerX += sin(PlayerA)*0.03f;
            PlayerY += cos(PlayerA)*0.03f;
        }
        else if(inputbuffer == 's'){
            PlayerX -= sin(PlayerA)*0.03f;
            PlayerY -= cos(PlayerA)*0.03f;
        }


        for(int i = 0; i<screenBreite; i++)
        {
            //calculate the ray
            float RayAngle = (PlayerA - FOV /2.0f)+((float)i/screenBreite) * FOV;

            bool HitWall = 0;

            float dist2Wall = 0;

            //Alculate Angle to a unit Vector ith sin and cos
            float EyeX = sinf(RayAngle);
            float EyeY = cosf(RayAngle);

            while(!HitWall && dist2Wall < viewDistance)
            {
                dist2Wall += 0.1;
                
                int testRayX = (int)(PlayerX+EyeX  * dist2Wall);
                int testRayY = (int)(PlayerY+EyeY * dist2Wall);

                if(testRayX < 0|| testRayX >= MapBreite || testRayY < 0|| testRayY >= MapHohe)
                {
                    HitWall = 1;
                    dist2Wall = viewDistance;
                }
                else{
                    if(map[testRayY*MapBreite+MapBreite+testRayX] == '#'){
                        HitWall = 1;
                    }
                }

            }
            //calculate ceiling and floor
            int Ceiling = (float)(screenHohe / 2.0f) - screenHohe/((float)dist2Wall);  
            int Floor = screenHohe-Ceiling;

            //calculate how shaded the walls are
            short shade = ' ';
            if(dist2Wall<viewDistance/4.0) shade = '#';//0x2588;
            else if(dist2Wall<viewDistance/3.0) shade = '?';//0x2593;
            else if(dist2Wall<viewDistance/2.0) shade = '!';//0x2592;
            else if(dist2Wall<=viewDistance) shade = '/';//0x2591;
            else shade = ' ';

            for(int y=0; y<screenHohe;y++ ){
                if(y<=Ceiling){
                    display[y*screenBreite+i] = ' ';
                }
                else if(y>Ceiling&&y<=Floor){
                    display[y*screenBreite+i]= shade;
                } 
                else{
                    //floor shading
                    float B = 1.0f - (((float)y-screenHohe/2.f)/(screenHohe/2.f)); 
                    if(B<0.2)display[y*screenBreite+i]= '=';
                    else if(B<0.4)display[y*screenBreite+i]= '~';
                    else if(B<0.7)display[y*screenBreite+i]= '-';
                    else if(B<0.85)display[y*screenBreite+i]= '.';
                    else display[y*screenBreite+i]= ' ';
                }
            }
            
            
            
            

        }
         //for(int x =0; x < screenBreite*screenHohe;x++){
        //    std::wcout<<display[x]; 

        //verbesserte ausgabe ohne abhängigkeit von der fenster größe
        //system("clear");
        printf("\x1b[H");
        for(int d = 0; d <= (screenBreite*screenHohe);d++){
            putchar(d%120 ? display[d]:newline);
            //fflush(stdout);
        }
        
        
    }

      

    return 0;
}
