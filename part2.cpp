#include <iostream>
#include <windows.h>
#include "CommonFunc.h"
#include "BaseObject.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDl_ttf.h>
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatObjects.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerHealth.h"
#include "BossObject.h"

BaseObject g_background;
TTF_Font* font_time = NULL;

Mix_Chunk* player_gun = NULL;


bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("PhamTrungDung_21020176",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else{
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
        {
            success = false;
        }
        else{
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags)&&imgFlags))
                success = false;
        }

        if (TTF_Init() == -1)
        {
            success = false;
        }

        font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
        if (font_time == NULL)
        {
            success = false;
        }
    }

    return success;
}

bool loadMedia()
{
    bool success = true;

      g_music = Mix_LoadMUS("sound//jungle_music.wav");
      if (g_music == NULL) success =false;

      player_gun = Mix_LoadWAV("sound//player_gun.wav");
      if (player_gun == NULL) success = false;

      return success;
}

bool LoadBackGround()
{
    bool ret = g_background.LoadImg("bg3.png", g_screen);
    if (ret == false)
        return false;
    return true;
}

void close()
{
    g_background.Free();

    Mix_FreeChunk(player_gun);

    player_gun = NULL;

    Mix_FreeMusic(g_music);
    g_music = NULL;

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
    Mix_Quit();

}
std::vector<BossObject*> MakeBossList()
{
    std::vector<BossObject*> list_boss;

    BossObject* boss_1st = new BossObject[20];

    for (int i=0; i<20; i++)
    {
        BossObject* p_boss = (boss_1st + i);
        if(p_boss != NULL)
        {
            p_boss->LoadImg("boss_object.png", g_screen);
            p_boss->set_clips();
            p_boss->set_xpos(MAX_MAP_X*TILE_SIZE -100);
            p_boss->set_ypos(10);

            BulletObject* p_bullet = new BulletObject();
            p_boss->InitBullet( g_screen);
            list_boss.push_back(p_boss);
        }
    }

    return list_boss;
}
std::vector<ThreatObjects*> MakeThreatList()
{
    std::vector<ThreatObjects*> list_threats;

    ThreatObjects* dynamic_threats = new ThreatObjects[20];

    for(int i=0; i<20;  i++)
    {
        ThreatObjects* p_threat = (dynamic_threats + i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("run_man_left.png", g_screen);
            p_threat->set_clips();
            p_threat->set_type_move(ThreatObjects::MOVE_IN_SPACE_THREAT);
            p_threat->set_x_pos(500 + i*500);
            p_threat->set_y_pos(200);

            int pos1 = p_threat->get_x_pos() - 256;
            int pos2 = p_threat->get_x_pos() + 256;
            p_threat->SetAnimationPos(pos1, pos2);
            p_threat->set_input_left(1);
            list_threats.push_back(p_threat);
        }

    }

    ThreatObjects* threat_objs = new ThreatObjects[20];

    for(int i=0; i<20; i++)
    {
        ThreatObjects* p_threat = (threat_objs+i);
        if(p_threat != NULL)
        {
            p_threat->LoadImg("sniper_block_1.png", g_screen);
            p_threat->set_clips();
            p_threat->set_x_pos(700 + i*1200);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatObjects::STATIC_THREAT);
            p_threat->set_input_left(0);

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}
int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    if (InitData() == false)
        return -1;
    if (LoadBackGround() == false)
        return -1;

    GameMap game_map;
    game_map.LoadMap("map/map01.txt");
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img_player//rig_move.png", g_screen);
    p_player.set_clips();

    PlayerHealth player_health;
    player_health.Init(g_screen);


    std::vector<ThreatObjects*> threats_list = MakeThreatList();
    std::vector<BossObject*> boss_list = MakeBossList();

    BossObject bossObject;
    bool ret = bossObject.LoadImg("boss_object.png",g_screen);
    bossObject.set_clips();


    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("exp2.png", g_screen);
    exp_threat.set_clip();
    if (!tRet) return -1;

    ExplosionObject exp_main;
    bool mRet = exp_main.LoadImg("exp2.png", g_screen);
    exp_main.set_clip();
    if(!mRet) return -1;

    int num_die = 0;

    TextObject time_game;
    time_game.SetColor(TextObject::WHITE_TEXT);

    int gun_select = p_player.get_money_count();


    bool is_quit = false;
    while(!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
          }

            p_player.HandelInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        Map map_data = game_map.getMap();

        p_player.HandleBullet(g_screen);
        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        p_player.DoPlayer(map_data);
        p_player.Show(g_screen);

        game_map.SetMap(map_data);
        game_map.DrawMap(g_screen);

        player_health.Show(g_screen);

        Mix_PlayMusic(g_music, -1);


        for(int i=0; i<threats_list.size(); i++)
        {
            ThreatObjects* p_threat = threats_list.at(i);
            if(p_threat != NULL)
            {
                p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
                p_threat->MakeBullet(g_screen, SCREEN_WIDTH,  SCREEN_HEIGHT);
                p_threat->Show(g_screen);


                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_threat->get_bulliet_list();
                for(int jj = 0; jj<tBullet_list.size(); jj++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if(pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if (bCol1)
                        {
                            p_threat->RemoveBullet(jj);
                            break;
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
                if(bCol1||bCol2)
                {
                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for(int ex = 0; ex < 8; ex++)
                    {
                        int x_pos = (p_player.GetRect().x + p_player.get_frame_width()*0.5) - width_exp_frame*0.5;
                        int y_pos = (p_player.GetRect().y + p_player.get_frame_height()*0.5) - height_exp_frame*0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                    num_die++;

                    if (num_die < 4)
                    {
                        gun_select = 0;
                        p_threat->Free();
                        threats_list.erase(threats_list.begin() + i);
                        p_player.SetRect(0,0);
                        SDL_Delay(1000);
                        p_player.set_comeback_time(5);
                        player_health.Descrease();
                        player_health.Render(g_screen);
                        continue;
                    }
                    else if(num_die == 4)
                    {
                        if(MessageBox(NULL, "Game Over", "Notification", MB_OK | MB_ICONSTOP) ==  IDOK)
                        {
                            p_threat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }

                }
            }

        }

        for(int i=0; i<boss_list.size(); i++)
        {
            BossObject* p_boss = boss_list.at(i);
            if(p_boss != NULL)
            {
                p_boss->SetMapXY(map_data.start_x_, map_data.start_y_);
                p_boss->DoPlayer(map_data);
                p_boss->MakeBullet(g_screen, SCREEN_WIDTH,  SCREEN_HEIGHT);
                p_boss->Show(g_screen);


                SDL_Rect rect_player = p_player.GetRectFrame();
                bool bCol1 = false;
                std::vector<BulletObject*> tBullet_list = p_boss->get_bullet_list();
                for(int jj = 0; jj<tBullet_list.size(); jj++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if(pt_bullet)
                    {
                        bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
                        if (bCol1)
                        {

                            p_boss->RemoveBullet(jj);
                            break;
                        }
                    }
                }

                SDL_Rect rect_boss = p_boss->GetRectFrame();
                bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_boss);
                if(bCol1||bCol2)
                {
                    int width_exp_frame = exp_main.get_frame_width();
                    int height_exp_frame = exp_main.get_frame_height();
                    for(int ex = 0; ex < 8; ex++)
                    {
                        int x_pos = (p_player.GetRect().x + p_player.get_frame_width()*0.5) - width_exp_frame*0.5;
                        int y_pos = (p_player.GetRect().y + p_player.get_frame_height()*0.5) - height_exp_frame*0.5;

                        exp_main.set_frame(ex);
                        exp_main.SetRect(x_pos, y_pos);
                        exp_main.Show(g_screen);
                        SDL_RenderPresent(g_screen);
                    }
                    num_die++;

                    if (num_die < 4)
                    {
                        p_player.SetRect(0,0);
                        p_player.set_comeback_time(10);
                        player_health.Descrease();
                        player_health.Render(g_screen);
                        continue;
                    }
                    else if(num_die == 4)
                    {
                        if(MessageBox(NULL, "Game Over", "Notification", MB_OK | MB_ICONSTOP) ==  IDOK)
                        {
                            p_boss->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }

                }
            }
            else if (p_boss == NULL)
            {
                if(MessageBox(NULL, "Game Over", "Notification", MB_OK | MB_ICONSTOP) ==  IDOK)
            {
                is_quit = true;
                break;
            }
            }
        }

        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
        for (int r=0; r< bullet_arr.size(); r++)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if(p_bullet != NULL)
            {
                for(int t=0; t<threats_list.size(); t++)
                {
                    ThreatObjects* obj_threat = threats_list.at(t);
                    if(obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.h = obj_threat->get_height_frame();
                        tRect.w = obj_threat->get_width_frame();

                        SDL_Rect bRect = p_bullet->GetRect();

                        bool bCol  = SDLCommonFunc::CheckCollision(bRect, tRect);

                        if(bCol)
                        {
                            for(int ex=0; ex<NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.3;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                            }

                            p_player.RemoveBullet(r);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin() + t);
                        }
                    }
                }
            }
        }

        for (int r=0; r< bullet_arr.size(); r++)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if(p_bullet != NULL)
            {
                for(int t=0; t<boss_list.size(); t++)
                {
                    BossObject* obj_boss = boss_list.at(t);
                    if(obj_boss != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_boss->GetRect().x;
                        tRect.y = obj_boss->GetRect().y;
                        tRect.h = obj_boss->get_height_frame();
                        tRect.w = obj_boss->get_width_frame();

                        SDL_Rect bRect = p_bullet->GetRect();

                        bool bCol  = SDLCommonFunc::CheckCollision(bRect, tRect);

                        if(bCol)
                        {
                            for(int ex=0; ex<NUM_FRAME_EXP; ex++)
                            {
                                int x_pos = p_bullet->GetRect().x; // - frame_exp_width*0.5;
                                int y_pos = p_bullet->GetRect().y - frame_exp_height*0.3;

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                            }

                            p_player.RemoveBullet(r);
                            obj_boss->Free();
                            boss_list.erase(boss_list.begin() + t);
                        }
                    }
                }
            }
        }

        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000;
        Uint32 val_time = 300 - time_val;
        if (val_time > 300)
        {
            if(MessageBox(NULL, "Game Over", "Notification", MB_OK | MB_ICONSTOP) ==  IDOK)
            {
                is_quit = true;
                break;
            }
            close();
            SDL_Quit();
            break;
        }
        else
        {
            std::string str_val = std::to_string(val_time);
            str_time += str_val;

            time_game.SetText(str_time);
            time_game.LoadFromRenderText(font_time, g_screen);
            time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
        }


        if (p_player.get_x_pos()/64 == 395)
        {
            if(MessageBox(NULL, "Game Win", "Notification", MB_OK | MB_ICONSTOP) ==  IDOK)
            {
                is_quit = true;
                break;
            }
            close();
            SDL_Quit();
            break;
        }


        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000/FRAME_PER_SECOND; // ms

        if(real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time>=0) SDL_Delay(delay_time);
        }
    }
    for (int i=0; i<threats_list.size(); i++)
    {
        ThreatObjects* p_threat = threats_list.at(i);
        if (p_threat != NULL)
        {
            p_threat->Free();
            p_threat = NULL;
        }
    }
    threats_list.clear();

    close();
    return 0;
}
