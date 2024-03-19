//This file serves for understanding and practicing the 
//implementation of Raycasting. This algorithm was used, for example, in Wolfenstein.
//with SDL2

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define ResolutionX  1200
#define ResolutionY  960

int neutralX = ResolutionX/2;
int neutralY = ResolutionY/2;
uint32_t *pixels;
int pitch = ResolutionX * 4;

int isRunning = true;

//Raycasting variables
const int MAP_WIDTH = 16;
//int MAP_LENGTH =16;
const int MAP_HIGTH = 16;

float PlayerX = 5;
float PlayerY = 5;

float P_Angel = 0;

const float FOV = 3.15/4;
float FOVDiv = FOV/2.0f;
float hightDiv = ResolutionY / 2.0f;

int map[16*16]= {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,1,0,0,0,0,3,0,1,
        1,0,0,0,0,0,1,1,0,0,0,0,0,3,0,1,
        1,0,0,0,0,1,2,1,0,0,0,0,0,3,0,1,
        1,0,0,0,0,1,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,3,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        };

float RayAngle = 0;
int TestRayX;
int TestRayY;

bool HitWall = false;
float  Dist2Wall = 0;

int mousx = 0,mousy=0;
int oldmousX = 0, oldmousy = 0;
int sense= 0;

int winx,winy;




int minimap_render(int i,int y,float PlayerAngle){
    //printf("MapY = %d\n");  
    //printf("map Mapy 16 = %d\n",map[MapY]);
    
    if(map[(int)((y/14) * 16 + i/14)] >0){
            *(pixels+(y*ResolutionX+i)) = 0b11111111111111111111111111111111;
    }

    //player render
    else if((int)(y/14) == (int)PlayerY && (int)(i/14) == (int)PlayerX){
        *(pixels+(y*ResolutionX+i)) = 0b00000000000000000000000000000000;
    }
    
    // //Fov render in minimapu
    // else if((int)(PlayerX*14)  == i || (int)(PlayerY*14) == y){
    //     *(pixels+(y*ResolutionX+i)) = 0b11111111111111111111111111111111;
    // }
        
    else{
        *(pixels+(y*ResolutionX+i)) = 0b11111111111110000011111111111111;
    }

    return 0;

        
        
}


int main(){
    
    
  //init
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *win;
  SDL_Renderer *rend;
  SDL_Texture *texture;

  SDL_Event ev;
  SDL_Event keyev;
  
  //define
  win = SDL_CreateWindow("DEMO", 
			 SDL_WINDOWPOS_CENTERED, 
			 SDL_WINDOWPOS_CENTERED, 
			 ResolutionX, ResolutionY,
             0);
  
  rend = SDL_CreateRenderer(win, 
                            -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  

  texture = SDL_CreateTexture(rend, 
			      SDL_PIXELFORMAT_ARGB8888, 
			      SDL_TEXTUREACCESS_STREAMING, 
			      ResolutionX, 
			      ResolutionY);
  

   SDL_SetRelativeMouseMode(SDL_TRUE);
    
  
 
  while(isRunning){
        //calculate Player view
        //rays
        const uint8_t *keystate = SDL_GetKeyboardState(NULL);
        SDL_GetWindowPosition(win,&winx,&winy);
        neutralX = winx+ResolutionX/2;
        while(SDL_PollEvent(&ev)){  
                if(ev.type == SDL_QUIT){
                    isRunning = false;
                }
        
                if(ev.type == SDL_MOUSEMOTION){
                    //    oldmousX = mousx;

                    //SDL_GetGlobalMouseState(&mousx,&mousy);
                    SDL_GetMouseState(&mousx,&mousy);
                    //SDL_GetRelativeMouseState(&mousx,&mousy);
                    //printf("mousex = %d, mousey = %d\n",mousx,mousy);

                    sense = (neutralX - mousx)*(-1);
                    
                    
                    P_Angel = P_Angel+((float)sense*0.001);
                    
                }
        }   
        
        
        

        if (keystate[SDL_SCANCODE_W]) {
        PlayerX += sin(P_Angel)*0.04f;
        PlayerY += cos(P_Angel)*0.04f;
            if(map[(int)PlayerY*16+(int)PlayerX]>0){
                PlayerX -= sin(P_Angel)*0.04f;
                PlayerY -= cos(P_Angel)*0.04f;
            }
        }
        if (keystate[SDL_SCANCODE_S]) {
            PlayerX -= sin(P_Angel)*0.04f;
            PlayerY -= cos(P_Angel)*0.04f;
            
            if(map[(int)PlayerY*16+(int)PlayerX]>0){
                PlayerX += sin(P_Angel)*0.04f;
                PlayerY += cos(P_Angel)*0.04f;
            }
        }
        if (keystate[SDL_SCANCODE_A]) {
            PlayerX += sin(P_Angel-90)*0.04f;
            PlayerY += cos(P_Angel-90)*0.04f;

            if(map[(int)PlayerY*16+(int)PlayerX]>0){
                PlayerX -= sin(P_Angel-90)*0.04f;
                PlayerY -= cos(P_Angel-90)*0.04f;
            }
        }
        if (keystate[SDL_SCANCODE_D]) {
            PlayerX += sin(P_Angel+90)*0.04f;
            PlayerY += cos(P_Angel+90)*0.04f;

            if(map[(int)PlayerY*16+(int)PlayerX]>0){
                PlayerX -= sin(P_Angel+90)*0.04f;
                PlayerY -= cos(P_Angel+90)*0.04f;
            }
        }
        
        if(keystate[SDL_SCANCODE_ESCAPE]){
            isRunning = false;
        }
                
            
      
        
        for(int i = 1; i<= ResolutionX; i++){
            
        
            RayAngle = (P_Angel - FOVDiv) + (((float)i/ResolutionX) * FOV);

            float RayVectorX = sinf(RayAngle);
            float RayvectorY = cosf(RayAngle);

            bool HitWall = false;
            float  Dist2Wall = 0;

            while(!HitWall && Dist2Wall < 16){
                Dist2Wall += 0.01;
                TestRayX = (int)(PlayerX+RayVectorX * Dist2Wall);
                TestRayY = (int)(PlayerY+RayvectorY * Dist2Wall);
                //printf("%d\n",map[TestRayY*16+TestRayX]);

                if(TestRayX < 0|| TestRayY < 0|| TestRayX > 16 || TestRayY >= 16){
                    HitWall = true;
                    Dist2Wall = 16;
                }

                else if(map[TestRayY*16+TestRayX] >= 1){
                    
                    HitWall = true;
                }
            }
            //printf("%f\n",Dist2Wall);
            //Display Ceiling and floor
            //calculate ceiling
            int Ceiling = (float)(hightDiv) - 960/((float)Dist2Wall);  
            int Floor = ResolutionY-Ceiling;
            //printf("%f",Dist2Wall);

            //write in Pixel array
            SDL_LockTexture(texture, NULL, (void **)&pixels, &pitch);
            
            for(int y = 0; y<ResolutionY;y++){
                if(y < 224 && i < 224){
                    minimap_render(i,y,P_Angel);
                }
                else if(y<=Ceiling){
                    *(pixels+(y*ResolutionX+i)) = 0b11100111000110011011000000110011;
                }
                
                else if(y>Ceiling&&y<=Floor){
                *(pixels+(y*ResolutionX+i)) = 0b00100111000110000011110011111111;
                }
                
                else *(pixels+(y*ResolutionX+i)) = 0b11111111111111110000000011111111;
            }
             
             
            
        }
            //Set mouse cursor to neutreal 
            

        
        SDL_UnlockTexture(texture);

            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, texture, NULL, NULL);
            SDL_RenderPresent(rend);
        
        
        
        //change texture (displayed) 


        // for (int i=0; i<800*600; i++) {
        //     *(pixels + i) = 0b11100111000110011011000000110011;
        // }
        SDL_WarpMouseInWindow(win, neutralX, neutralY);



        

        
    }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(rend);
  
  SDL_DestroyWindow(win);
  SDL_Quit();
  
  
  
  return 0;
} 

