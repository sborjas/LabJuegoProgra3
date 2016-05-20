
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<list>
#include <math.h>
#include <stdlib.h>
#include "Bullet.h"
#include "Enemy.h"

#define PI 3.14159265

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character, *bullet, *enemy, *enemy2;
SDL_Rect rect_background,rect_character, rect_bullet, rect_enemy, rect_enemy2;

int screen_width = 500;
int screen_height= 250;
int cont = 0;
double getAngle(double distance_x, double distance_y)
{
    double angle = atan (distance_y/distance_x) * 180 / PI;
    if(distance_x<0)
    {
        angle=180+angle;
    }
    if(distance_x>0 && distance_y<0)
    {
        angle=360+angle;
    }
    return angle;
}

bool isOutOfBounds(int x, int y)
{
    return x>screen_width || x<0 || y>screen_height || y<0;
}

bool collisionCheck(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x > r2.x+r2.w)//Muy a la derecha
        return false;
    if(r1.x+r1.w < r2.x)//Muy a la izquierda
        return false;
    if(r1.y > r2.y+r2.h)//Muy abajo
        return false;
    if(r1.y+r1.h < r2.y)//Muy arriba
        return false;
    return true;
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, screen_width/*WIDTH*/, screen_height/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    character = IMG_LoadTexture(renderer, "personaje.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = 0;
    rect_character.y = 100;
    rect_character.w = w;
    rect_character.h = h;

    bullet = IMG_LoadTexture(renderer, "bullet.png");
    SDL_QueryTexture(bullet, NULL, NULL, &w, &h);
    rect_bullet.x = 0;
    rect_bullet.y = 100;
    rect_bullet.w = w;
    rect_bullet.h = h;

    enemy = IMG_LoadTexture(renderer, "enemy.png");
    SDL_QueryTexture(enemy, NULL, NULL, &w, &h);
    rect_enemy.x = 0;
    rect_enemy.y = 100;
    rect_enemy.w = w;
    rect_enemy.h = h;

    enemy2 = IMG_LoadTexture(renderer, "enemy2.png");
    SDL_QueryTexture(enemy2, NULL, NULL, &w, &h);
    rect_enemy2.x = 0;
    rect_enemy2.y = 100;
    rect_enemy2.w = w;
    rect_enemy2.h = h;

    list<Bullet*>bullets;
    list<Enemy*>enemies, enemies2;
    int frame=0;

    //Main Loop
    double player_velocity=3;
    double bullet_velocity=8;
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
         if( currentKeyStates[ SDL_SCANCODE_LSHIFT ] )
        {
            player_velocity = 5;
        }else
            player_velocity = 3;

        if( currentKeyStates[ SDL_SCANCODE_UP ]  && rect_character.y > 0 )
        {
            rect_character.y-=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_DOWN ] && rect_character.y < screen_height - rect_character.h )
        {
            rect_character.y+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] && rect_character.x < screen_width - rect_character.w)
        {
            rect_character.x+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_LEFT ] && rect_character.x > 0  )
        {
            rect_character.x-=player_velocity;
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        if(frame%30==0)
        {
            double dist_x = mouse_x - rect_character.x;
            double dist_y = rect_character.y - mouse_y;

            bullets.push_back(new Bullet(rect_character.x,rect_character.y,getAngle(dist_x,dist_y)));

//            if(currentKeyStates[ SDL_SCANCODE_SPACE ]){
//            }

        }

        if(frame%100==0)
        {
            enemies.push_back(new Enemy(0,125));

            enemies2.push_back(new Enemy(0,200));
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Bullet*>::iterator i=bullets.begin();
            i!=bullets.end();
            i++)
        {
            Bullet* bullet_actual = *i;
            rect_bullet.x=bullet_actual->x;
            rect_bullet.y=bullet_actual->y;
            SDL_RenderCopy(renderer, bullet, NULL, &rect_bullet);
            bullet_actual->x+=cos(-bullet_actual->angle*PI/180)*bullet_velocity;
            bullet_actual->y+=sin(-bullet_actual->angle*PI/180)*bullet_velocity;

            if(isOutOfBounds(bullet_actual->x,bullet_actual->y))
            {
                list<Bullet*>::iterator to_erase=i;
                i--;
                bullets.erase(to_erase);
                delete bullet_actual;
            }
        }


        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy* current_enemy = *i;
            current_enemy->x++;
            rect_enemy.x = current_enemy->x;
            rect_enemy.y = current_enemy->y;
            SDL_RenderCopy(renderer, enemy, NULL, &rect_enemy);

            if(isOutOfBounds(current_enemy->x,current_enemy->y))
            {
                list<Enemy*>::iterator to_erase=i;
                i--;
                enemies.erase(to_erase);
                delete current_enemy;
            }
        }
        for(list<Enemy*>::iterator i=enemies2.begin();
            i!=enemies2.end();
            i++)
        {
            Enemy* current_enemy = *i;
            current_enemy->x++;
            rect_enemy2.x = current_enemy->x;
            rect_enemy2.y = current_enemy->y;
            SDL_RenderCopy(renderer, enemy2, NULL, &rect_enemy2);

            if(isOutOfBounds(current_enemy->x,current_enemy->y))
            {
                list<Enemy*>::iterator to_erase=i;
                i--;
                enemies2.erase(to_erase);
                delete current_enemy;
            }
        }

        SDL_RenderCopy(renderer, character, NULL, &rect_character);

        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy*current_enemy = *i;
            for(list<Bullet*>::iterator j=bullets.begin();
                j!=bullets.end();
                j++)
            {
                Bullet*current_bullet = *j;

                rect_enemy.x = current_enemy->x;
                rect_enemy.y = current_enemy->y;

                rect_bullet.x = current_bullet->x;
                rect_bullet.y = current_bullet->y;

                if(collisionCheck(rect_enemy,rect_bullet))
                {
                    list<Bullet*>::iterator to_erase=j;
                    j--;
                    bullets.erase(to_erase);
                    delete current_bullet;

                    list<Enemy*>::iterator to_erase_enemy=i;
                    i--;
                    enemies.erase(to_erase_enemy);
                    cont+= 10;
                    cout << "SCORE: "<<cont<<endl;
                    delete current_enemy;
                    break;
                }
                if(collisionCheck(rect_character,rect_enemy))
                    cout << "CHOQUE 1" << endl;
                    cout.flush();
            }
        }
        for(list<Enemy*>::iterator i=enemies2.begin();
            i!=enemies2.end();
            i++)
        {
            Enemy*current_enemy = *i;
            for(list<Bullet*>::iterator j=bullets.begin();
                j!=bullets.end();
                j++)
            {
                Bullet*current_bullet = *j;

                rect_enemy2.x = current_enemy->x;
                rect_enemy2.y = current_enemy->y;

                rect_bullet.x = current_bullet->x;
                rect_bullet.y = current_bullet->y;

                if(collisionCheck(rect_enemy2,rect_bullet))
                {
                    list<Bullet*>::iterator to_erase=j;
                    j--;
                    bullets.erase(to_erase);
                    delete current_bullet;

                    list<Enemy*>::iterator to_erase_enemy=i;
                    i--;
                    enemies.erase(to_erase_enemy);
                    cont+= 10;
                    cout << "SCORE: "<<cont<<endl;
                    delete current_enemy;
                    break;
                }
                if(collisionCheck(rect_character,rect_enemy2))
                    cout << "CHOQUE 2"<<endl;
                    cout.flush();

            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(17);
        frame++;
    }

	return 0;
}
