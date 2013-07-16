/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "main.h"
#include "world.h"
#include "update.h"
#include "button_events.h"
#include "holidays.h"
#include "options.h"
#include "message_log.h"
#include "score.h"
#include "achievement_tooltips.h"

#ifdef GAME_OS_OSX
    #include <CoreFoundation/CoreFoundation.h>
#endif

using namespace std;

void game_loop(){
    //Turn off key repeating.
    SDL_EnableKeyRepeat(0,0);
    SDL_EnableUNICODE(SDL_ENABLE);

    int things_loaded=0;
    int things_to_load=11;

    load_world();
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.prepare_tracks();

    music.reinitialize_menu_track();
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/alarm.ogg",MUSIC_ALARM);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/valley.ogg",MUSIC_WORLDMAP_VALLEY);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/castle.ogg",MUSIC_WORLDMAP_CASTLE);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/haunted.ogg",MUSIC_WORLDMAP_HAUNTED);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/mountain.ogg",MUSIC_WORLDMAP_MOUNTAIN);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/volcano.ogg",MUSIC_WORLDMAP_VOLCANO);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/desert.ogg",MUSIC_WORLDMAP_DESERT);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/ocean.ogg",MUSIC_WORLDMAP_OCEAN);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/mountain_peak.ogg",MUSIC_WORLDMAP_MOUNTAIN_PEAK);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.load_track("data/music/lighthouse.ogg",MUSIC_WORLDMAP_LIGHTHOUSE);
    render_loading_screen((double)++things_loaded/(double)things_to_load);

    music.restart_track(MUSIC_MENU);
    music.play_track(MUSIC_MENU,1.0);

    player.load_data();

    //If no profile exists.
    if(player.name=="\x1F"){
        button_event_open_window_create_profile(NULL,0);
    }
    //If a profile already exists.
    else{
        profile.load_profile_global_data();
        vector_windows[WINDOW_MAIN_MENU].turn_on();

        window_manager.set_main_menu_current_button();
    }

    play_game_start_sound();

    player.set_firework_rate();

    //The maximum number of frames to be skipped.
    const double MAX_FRAMESKIP=5.0;

    //In our logic update while() loop, we consume SKIP_TICKS sized chunks of time, which are added to next_game_tick.
    Uint32 next_game_tick=SDL_GetTicks();

    //The number of logic updates that have occured since the last render.
    int number_of_updates=0;

    //Declare and start the frame rate timer. This keeps track of the frame rate, as well as milliseconds spent per frame.
    //See the explanation at the beginning of the game loop for more details.
    Timer timer_frame_rate;
    timer_frame_rate.start();

    //The following three variables are used for displaying the FPS to the player.
    //This integer keeps track of the number of frame each second. Every time a second has passed, frame_count's value is given to frame_rate, and frame_count is reset to 0.
    int frame_count=0;

    //At any given time (after the first second of runtime), frame_rate represents the number of frames that were rendered in the past second.
    int frame_rate=0;

    //For added performance information, ms_per_frame takes the FPS value each second and converts it to the number of milliseconds spent on each frame in the past second.
    double ms_per_frame=0;

    //Keeps track of the logic frames each second.
    int logic_frame_count=0;
    ////At any given time (after the first second of runtime), logic_frame_rate represents the number of logic frames that were processed in the past second.
    int logic_frame_rate=0;

    //Here we have the game loop.
    while(true){
        //If at least a second(preferably exactly one second) has passed since the last restart of the frame rate timer, we do several things.
        //First, we set the frame rate to the current frame count, which has been counting up since the last timer restart.
        //Second, we reset the frame count to 0, to count the number of frames succesfully completed in the next second.
        //Third, we set the milliseconds per frame to 1000/our current frame rate. Since our frame rate is in seconds, this gives us the number of milliseconds being spent on
        //each frame. Finally, we restart the frame rate timer.
        if(timer_frame_rate.get_ticks()>=1000){
            logic_frame_rate=logic_frame_count;
            logic_frame_count=0;

            frame_rate=frame_count;
            frame_count=0;
            ms_per_frame=1000.0/frame_rate;
            timer_frame_rate.start();
        }

        //Here we are just incrementing the frame counter, which gives us the frames per second in the above if() statement.
        frame_count++;

        //For our game loop, we first update game logic, and then render. The two are kept separate and independent.

        //First we reset our updates counter to 0.
        number_of_updates=0;

        //We consume SKIP_TICKS sized chunks of time, which ultimately translates to updating the logic UPDATE_LIMIT times a second.
        //We also check to see if we've updated logic MAX_FRAMESKIP times without rendering, at which point we render.
        while(SDL_GetTicks()>next_game_tick && number_of_updates<MAX_FRAMESKIP){
            //We are doing another game logic update.
            //If this reaches MAX_FRAMESKIP, we will render.
            number_of_updates++;

            //Increment the logic frame counter.
            logic_frame_count++;

            //Clamp the time step to something reasonable.
            if(fabs(SDL_GetTicks()-next_game_tick)>SKIP_TICKS*2){
                next_game_tick=SDL_GetTicks()-SKIP_TICKS*2;
            }

            //Consume another SKIP_TICKS sized chunk of time.
            next_game_tick+=SKIP_TICKS;

            //Now we update the game logic:

            if(player.need_to_reinit){
                player.need_to_reinit=false;
                main_window.reinitialize();
            }

            //First, we check for input from the player.
            input();

            //Then, we move all of the objects.
            movement();

            //Once everything has had its chance to move, we handle events (collision detection).
            events();

            //Handle camera movement.
            camera(frame_rate,ms_per_frame,logic_frame_rate);

            //Animate everything. This entails first making sure that the appropriate animation state is being displayed, and then incrementing the frame for each sprite.
            animation();
        }

        //Now that we've handled logic updates, we do our rendering.

        //Finally, we render everything that is currently within the camera bounds to the screen.
        //We pass frame_rate and ms_per_frame so they can be displayed onscreen.
        render(frame_rate,ms_per_frame,logic_frame_rate);
    }
}

/**
 * Function called by libfov to apply light to a cell.
 *
 * \param map Pointer to map data structure passed to function such as
 *            fov_circle.
 * \param x   Absolute x-axis position of cell.
 * \param y   Absolute x-axis position of cell.
 * \param dx  Offset of cell from source cell on x-axis.
 * \param dy  Offset of cell from source cell on y-axis.
 * \param src Pointer to source data structure passed to function such
 *            as fov_circle.
 */
void apply_map(void *level,int x,int y,int dx,int dy,void *src){
	if(((Level *)level)->onMap_map(x,y)){
		((Level *)level)->setSeen_map(x,y);
	}
}
void apply_lighting(void *level,int x,int y,int dx,int dy,void *src){
	if(((Level *)level)->onMap_lighting(x,y)){
		((Level *)level)->setSeen_lighting(x,y,(Light_Source*)src);
	}
}

/**
 * Function called by libfov to determine whether light can pass
 * through a cell. Return zero if light can pass though the cell at
 * (x,y), non-zero if it cannot.
 *
 * \param map Pointer to map data structure passed to function such as
 *            fov_circle.
 * \param x   Absolute x-axis position of cell.
 * \param y   Absolute x-axis position of cell.
 */
bool opaque_map(void *level,int x,int y){
	return ((Level *)level)->blockLOS_map(x,y);
}
bool opaque_lighting(void *level,int x,int y){
	return ((Level *)level)->blockLOS_lighting(x,y);
}

//Apparently, SDL likes main() to take these arguments, so that is what we will do.
int main(int argc,char* args[]){
    #ifdef GAME_OS_OSX
        //Set the working directory to the Resources directory of our bundle.
        char path[PATH_MAX];
        CFURLRef url=CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        CFURLGetFileSystemRepresentation(url,true,(uint8_t*)path,PATH_MAX);
        CFRelease(url);
        chdir(path);
    #endif

    #ifdef GAME_DEMO
        demo_mode=true;
    #endif

    time_t seconds;
    uint32_t random_seed=(uint32_t)time(&seconds);
    rng.mrand_main.seed(random_seed);
    rng.mrand_render.seed(random_seed);

    image.current_texture=0;

    determine_holiday();

    if(!save_location_load()){
        return 1;
    }

    profile.make_directories();

    if(load_current_profile()){
        if(!options_load()){
            profile.version_mismatch=true;
            player.reset();
        }
        if(!profile.load_profile_list()){
            return 1;
        }
    }

    if(!global_options_load()){
        return 1;
    }

    //Initialize the various subsystems.
    if(!main_window.init()){
        update_error_log("Failed to initialize the window.");
        return 1;
    }

    fov_settings_init(&profile.fov_settings_map);
    fov_settings_set_opacity_test_function(&profile.fov_settings_map,opaque_map);
    fov_settings_set_apply_lighting_function(&profile.fov_settings_map,apply_map);

    fov_settings_init(&player.fov_settings_lighting);
    fov_settings_set_opacity_test_function(&player.fov_settings_lighting,opaque_lighting);
    fov_settings_set_apply_lighting_function(&player.fov_settings_lighting,apply_lighting);

    prepare_scores();
    set_achievement_tooltips();

    player.menu_backgrounds.push_back(Menu_Background(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,10.0,10.0));
    player.menu_backgrounds.push_back(Menu_Background(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,8.0,8.0));
    player.menu_backgrounds.push_back(Menu_Background(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,6.0,6.0));

    player.overlay_snow.push_back(Overlay(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,2.0,2.0,&image.overlay_snow_images));
    player.overlay_rain.push_back(Overlay(main_window.SCREEN_WIDTH,main_window.SCREEN_HEIGHT,2.0,2.0,&image.overlay_rain_images));

    //Begin the game by entering the game loop.
    game_loop();
    return 0;
}
