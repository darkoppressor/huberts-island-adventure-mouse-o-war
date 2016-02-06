/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "npc.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "ai_keys.h"
#include "mirror.h"
#include "score.h"

using namespace std;

Npc::Npc(double get_x,double get_y,short get_type,bool make_traps){
    type=get_type;

    x=get_x;
    y=get_y;

    exists=true;

    BOSS=false;
    MAIN_BOSS=false;

    boss_phase=0;

    health=-1;
    health_max=-1;

    drops_items=true;

    facing=LEFT;

    color=COLOR_WHITE;

    w=32;
    h=32;

    FLYING=false;
    is_player=false;
    which_mp_player=-1;
    invulnerable=false;
    DYING=false;
    counter_bubble=0;
    delta_move_state=NONE;
    counter_swim_sound=0;

    sucked_left=false;
    sucked_right=false;

    undead=false;

    hitbox_size_modifier=-4;

    custom_physics=false;

    wears_tophat=false;
    tophat_when_dead=false;
    tophat_adjust_x=0.0;
    tophat_adjust_y=0.0;

    starts_dead=false;

    //Movement:
    move_state=0;
    run_speed=0.0;
    max_speed_in_air=-1.0;
    max_speed_during_special_attack=-1.0;
    max_speed_during_alarm=-1.0;
    max_swim_speed_at_surface=-1.0;
    max_speed=5.0;
    speedometer=0.0;

    //Shooting:
    shoot_state=0;
    SHOOTING=false;
    shoot_render_direction=0;

    //Start the player off with his feet on neither air nor ground.
    //handle_events() will determine where the player's feet are.
    touching_air=false;
    touching_ground=false;
    touching_sloped_ground=false;
    touched_slope_angle=0.0;
    touching_sloped_ground_last_check=false;
    on_cloud=false;
    cloud_npc=false;

    //Gravity:
    air_velocity=0.0;
    IN_AIR=false;
    gravity_max=10.0;
    gravity=0.5;

    //Jumping:
    jump_state=false;
    jump_max=10.0;
    jump_min=5.0;
    extra_jumps=0;

    //Swimming:
    SWIMMING=false;
    underwater=false;
    water_running=false;
    SWIM_CAN_JUMP=false;
    swimming_gear=false;
    oxygen_max_capacity=25;
    oxygen=oxygen_max_capacity;
    max_swim_speed=6.5;
    max_buoyancy=2.0;
    float_speed=0.0;
    swim_acceleration=0.1375;
    swim_deceleration=0.275;
    swim_friction=swim_acceleration;

    //Other Upgrades:
    suit_deadly_water=false;
    suit_sharp=false;
    suit_banana=false;
    shot_homing=false;
    translator=false;
    j_wing=false;

    //Climbing:
    CLIMBING=false;
    on_climbable_top_tile=false;
    climb_jump_timer=0;
    climb_jump_delay=20;
    climb_speed=3.5;
    climbing_jump_max=6.0;
    climbing_jump_min=4.0;
    CLIMBING_JUMP=false;
    climb_state=0;

    //Moving Platform:
    moving_platform_x=0.0;
    moving_platform_y=0.0;
    MOVING_PLATFORM_X_THIS_FRAME=false;
    MOVING_PLATFORM_IN_WATER=false;

    //Animation:
    frame_idle=0;
    frame_counter_idle=0;
    frame=0;
    frame_counter=0;
    frame_swim=0;
    frame_counter_swim=0;
    frame_jump=0;
    frame_counter_jump=0;
    frame_shoot=0;
    frame_counter_shoot=0;
    frame_climb=0;
    frame_counter_climb=0;
    frame_fly=0;
    frame_counter_fly=0;
    frame_special_attack=0;
    frame_counter_special_attack=0;
    frame_death=0;
    frame_counter_death=0;

    frame_speed_walk=10;
    frame_speed_special_attack=30;
    frame_speed_death=10;
    frame_speed_fly=20;
    frame_speed_swim=20;

    counter_fade=player.get_npc_fade();

    light_source.on=false;
    light_source.x=0;
    light_source.y=0;
    light_source.color=color_name_to_doubles(COLOR_WHITE);
    light_source.radius=3*(TILE_SIZE/LIGHTING_TILE_SIZE);
    light_source.dimness=0.0;
    light_source.falloff=0.375/(TILE_SIZE/LIGHTING_TILE_SIZE);

    special_attack_toggles_light=false;
    special_attack_end_toggles_light=false;

    NPC_WALK_SPRITES=0;
    NPC_JUMP_SPRITES=0;
    NPC_SWIM_SPRITES=0;
    NPC_SPECIAL_ATTACK_SPRITES=0;
    NPC_DEAD_SPRITES=0;

    act_as_platform=false;

    ethereal=false;
    ethereal_to_npcs=false;

    never_move=false;

    //AI defaults:
    counter_targeted=0;
    friendly=false;
    can_be_duplicated=true;
    counts_as_trap=false;
    deadly_to_touch=true;
    deadly_while_special_attacking=true;
    has_footsteps=false;
    footstep_sound_frequency=1;
    footstep_special_attack_sound_frequency=1;

    home_x=0;
    home_y=0;
    home_permanent=false;
    returning_home=false;

    wander=false;
    wander_notice_edges=false;
    wander_notice_walls=false;
    wander_pause=false;
    wander_frequency_min=200;
    wander_frequency_max=600;
    wander_counter=0;
    wander_direction=-1;
    wander_direction_up_down=-1;
    wander_weight=100;
    wander_up_and_down=false;
    wander_up_and_down_only=false;
    notice_wall_ignore_cloud=false;

    jump=false;
    jump_only=false;
    jump_frequency=0;
    jump_counter=0;
    jump_percent=1;

    use=false;
    use_frequency=0;
    use_counter=0;
    use_only_while_alarm_off=false;

    follow_player=false;
    follow_player_range_x=10;
    follow_player_range_y=10;
    follow_player_only_in_water=false;
    follow_player_only_in_air=false;
    follow_player_only_when_back_turned=false;
    follow_player_always_in_air=false;
    follow_player_during_alarm=false;
    player_back_turned_when_above=false;
    follow_player_only_when_angry=false;

    flee_player=false;
    flee_player_range=10;

    special_attack_type=SPECIAL_ATTACK_NONE;
    special_attack_when_chasing=false;
    special_attack_when_fleeing=false;
    special_attack_on_sight=false;
    special_attack_only_when_player_facing=false;
    special_attack_only_when_facing_player=false;
    special_attack_only_when_alarm_off=false;
    special_attack_anytime=false;
    special_attack_range_x=4;
    special_attack_range_y=4;
    special_attack_below_only=false;
    special_attack_above_only=false;
    special_attack_target_x=0;
    special_attack_target_y=0;
    special_attack_give_up_time=-1;
    special_attack_give_up_counter=0;
    special_attack_frequency=600;
    special_attack_counter=0;
    special_attack_pause_length=0;
    special_attack_pause_counter=0;
    special_attack_sound_frame=0;
    special_attack_in_progress=false;
    special_attack_ends_with_animation=false;
    special_attack_random_target=false;

    angry=false;

    crawls=false;

    //Setup NPC type-specific stuff.

    w=NPC_SPRITE_COUNTS[type].width;
    h=NPC_SPRITE_COUNTS[type].height;
    NPC_WALK_SPRITES=NPC_SPRITE_COUNTS[type].walk_sprites;
    NPC_JUMP_SPRITES=NPC_SPRITE_COUNTS[type].jump_sprites;
    NPC_SWIM_SPRITES=NPC_SPRITE_COUNTS[type].swim_sprites;
    NPC_FLY_SPRITES=NPC_SPRITE_COUNTS[type].fly_sprites;
    NPC_SPECIAL_ATTACK_SPRITES=NPC_SPRITE_COUNTS[type].special_attack_sprites;
    NPC_DEAD_SPRITES=NPC_SPRITE_COUNTS[type].dead_sprites;

    //Setup vectors to hold the npc's sprite sheet clipping data.

    for(short i=0;i<NPC_WALK_SPRITES;i++){
        vector_sprites_left.push_back(SDL_Rect());
        vector_sprites_left[i]=SPRITES_NPC[type].walk_left[i];
    }
    for(short i=0;i<NPC_WALK_SPRITES;i++){
        vector_sprites_right.push_back(SDL_Rect());
        vector_sprites_right[i]=SPRITES_NPC[type].walk_right[i];
    }

    for(short i=0;i<NPC_JUMP_SPRITES;i++){
        vector_sprites_jump_left.push_back(SDL_Rect());
        vector_sprites_jump_left[i]=SPRITES_NPC[type].jump_left[i];
    }
    for(short i=0;i<NPC_JUMP_SPRITES;i++){
        vector_sprites_jump_right.push_back(SDL_Rect());
        vector_sprites_jump_right[i]=SPRITES_NPC[type].jump_right[i];
    }

    for(short i=0;i<NPC_SWIM_SPRITES;i++){
        vector_sprites_swim_left.push_back(SDL_Rect());
        vector_sprites_swim_left[i]=SPRITES_NPC[type].swim_left[i];
    }
    for(short i=0;i<NPC_SWIM_SPRITES;i++){
        vector_sprites_swim_right.push_back(SDL_Rect());
        vector_sprites_swim_right[i]=SPRITES_NPC[type].swim_right[i];
    }

    for(short i=0;i<NPC_FLY_SPRITES;i++){
        vector_sprites_fly_left.push_back(SDL_Rect());
        vector_sprites_fly_left[i]=SPRITES_NPC[type].fly_left[i];
    }
    for(short i=0;i<NPC_FLY_SPRITES;i++){
        vector_sprites_fly_right.push_back(SDL_Rect());
        vector_sprites_fly_right[i]=SPRITES_NPC[type].fly_right[i];
    }

    for(short i=0;i<NPC_SPECIAL_ATTACK_SPRITES;i++){
        vector_sprites_special_attack_left.push_back(SDL_Rect());
        vector_sprites_special_attack_left[i]=SPRITES_NPC[type].special_attack_left[i];
    }
    for(short i=0;i<NPC_SPECIAL_ATTACK_SPRITES;i++){
        vector_sprites_special_attack_right.push_back(SDL_Rect());
        vector_sprites_special_attack_right[i]=SPRITES_NPC[type].special_attack_right[i];
    }

    for(short i=0;i<NPC_DEAD_SPRITES;i++){
        vector_sprites_dead.push_back(SDL_Rect());
        vector_sprites_dead[i]=SPRITES_NPC[type].dead[i];
    }

    sprite_sheet_image_npc=return_npc_image(type);

    //Slime
    if(type==NPC_SLIME){
        wears_tophat=true;

        //Physics:

        max_speed=0.5;

        //AI:

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_pause=true;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_weight=90;
    }
    //Cow
    else if(type==NPC_COW){
        wears_tophat=true;

        //Physics:

        max_speed=0.4;

        //AI:

        can_be_duplicated=false;

        friendly=true;

        has_footsteps=true;
        footstep_sound_frequency=3;

        act_as_platform=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_pause=true;
        wander_frequency_min=300;
        wander_frequency_max=1000;
        wander_weight=74;

        special_attack_type=SPECIAL_ATTACK_COW_MOO;
        special_attack_anytime=true;
        special_attack_frequency=random_range(300,1000);
        special_attack_sound_frame=2;
        special_attack_ends_with_animation=true;
    }
    //Bee
    else if(type==NPC_BEE){
        wears_tophat=true;
        tophat_adjust_x=8.0;
        tophat_adjust_y=0.0;

        //Physics:

        max_speed=5.0;
        FLYING=true;

        //AI:

        wander=true;
        wander_notice_edges=false;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=600;

        special_attack_type=SPECIAL_ATTACK_BEE_DIVEBOMB;
        special_attack_on_sight=true;
        special_attack_range_x=4;
        special_attack_range_y=8;
        special_attack_below_only=true;
        special_attack_frequency=600;
        special_attack_pause_length=30;
        special_attack_give_up_time=180;
    }
    //Replicator
    else if(type==NPC_REPLICATOR){
        //Physics:

        deadly_to_touch=false;
        deadly_while_special_attacking=false;
        invulnerable=true;
        ethereal_to_npcs=true;
        max_speed=random_range(1,3);

        health_max=2;
        health=health_max;

        //AI:

        can_be_duplicated=false;

        wander=true;
        wander_notice_edges=false;
        wander_notice_walls=true;
        wander_pause=false;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_weight=90;

        follow_player=true;
        follow_player_range_x=30;
        follow_player_range_y=30;
        follow_player_only_when_angry=true;

        use=true;
        use_frequency=300;

        special_attack_type=SPECIAL_ATTACK_REPLICATE;
        special_attack_anytime=true;
        special_attack_frequency=random_range(600,900);
        special_attack_counter=special_attack_frequency;

        crawls=true;
    }
    //Fish
    else if(type==NPC_FISH){
        //Physics:

        invulnerable=true;

        max_speed=0.3;
        max_swim_speed=0.5;
        max_buoyancy=1.0;

        swimming_gear=true;

        set_color();

        //AI:

        can_be_duplicated=false;

        friendly=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;
        wander_notice_edges=false;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_up_and_down=true;

        follow_player=true;
        follow_player_range_x=6;
        follow_player_range_y=6;
        follow_player_only_in_water=true;
    }
    //Monkey
    else if(type==NPC_MONKEY){
        wears_tophat=true;

        //Physics:

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        wander=false;

        special_attack_type=SPECIAL_ATTACK_MONKEY_THROW_FRUIT;
        special_attack_on_sight=true;
        special_attack_range_x=12;
        special_attack_range_y=12;
        special_attack_frequency=75;
        special_attack_ends_with_animation=true;
    }
    //Laser spinner
    else if(type==NPC_LASER_SPINNER){
        //Physics:

        invulnerable=true;

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        special_attack_type=SPECIAL_ATTACK_LASER_SPINNER_FIRE_LASER;
        special_attack_anytime=true;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;
        special_attack_random_target=true;

        //Rendering:

        frame_speed_special_attack=5;

        light_source.color=color_name_to_doubles(COLOR_RED);
        special_attack_toggles_light=true;
        special_attack_end_toggles_light=true;
    }
    //Penguin
    else if(type==NPC_PENGUIN){
        wears_tophat=true;

        //Physics:

        max_speed=4.5;

        //AI:

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
    }
    //Fish on ice
    else if(type==NPC_FISH_ON_ICE){
        wears_tophat=true;

        //Physics:

        max_speed=1.0;

        //AI:

        wander=true;
        wander_notice_walls=true;
        wander_frequency_min=120;
        wander_frequency_max=240;
        jump=true;
        jump_only=true;
    }
    //Penguin harmless
    else if(type==NPC_PENGUIN_HARMLESS){
        wears_tophat=true;
        tophat_when_dead=true;

        starts_dead=true;

        can_be_duplicated=false;

        //Physics:

        exists=false;
    }
    //Fish on ice harmless
    else if(type==NPC_FISH_ON_ICE_HARMLESS){
        starts_dead=true;

        can_be_duplicated=false;

        //Physics:

        exists=false;

        //Rendering:

        frame_speed_death=5;
    }
    //Sword knight
    else if(type==NPC_SWORD_KNIGHT){
        wears_tophat=true;

        //Physics:

        max_speed=2.5;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=7;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_pause=true;

        special_attack_type=SPECIAL_ATTACK_SWORD_KNIGHT_SWORD_SLASH;
        special_attack_on_sight=true;
        special_attack_range_x=3;
        special_attack_range_y=3;
        special_attack_pause_length=15;
        special_attack_frequency=75;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_walk=9;
        frame_speed_special_attack=4;
    }
    //Ball knight
    else if(type==NPC_BALL_KNIGHT){
        wears_tophat=true;

        //Physics:

        max_speed=2.5;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=7;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=300;
        wander_pause=true;

        special_attack_type=SPECIAL_ATTACK_BALL_KNIGHT_THROW_BALL;
        special_attack_on_sight=true;
        special_attack_range_x=2;
        special_attack_range_y=8;
        special_attack_frequency=75;
        special_attack_ends_with_animation=true;
        special_attack_above_only=true;

        //Rendering:

        frame_speed_walk=9;
        frame_speed_special_attack=2;
    }
    //Archer
    else if(type==NPC_ARCHER){
        wears_tophat=true;

        //Physics:

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;

        special_attack_type=SPECIAL_ATTACK_ARCHER_FIRE_ARROW;
        special_attack_on_sight=true;
        special_attack_range_x=12;
        special_attack_range_y=12;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_walk=120;
    }
    //Scarecrow
    else if(type==NPC_SCARECROW){
        //Physics:

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        deadly_to_touch=false;

        wander=true;

        special_attack_type=SPECIAL_ATTACK_SCARECROW_WHACK;
        special_attack_on_sight=true;
        special_attack_range_x=2;
        special_attack_range_y=3;
        special_attack_frequency=90;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_walk=15;
        frame_speed_special_attack=5;
        frame_speed_death=45;
    }
    //Pumpkin
    else if(type==NPC_PUMPKIN){
        wears_tophat=true;
        tophat_adjust_x=-2.0;
        tophat_adjust_y=6.0;

        //Physics:

        max_speed=3.75;

        //AI:

        jump=true;
        jump_only=true;
        jump_frequency=90;
        jump_percent=25;

        follow_player=true;
        follow_player_range_x=30;
        follow_player_range_y=6;
        follow_player_only_when_back_turned=true;
        follow_player_always_in_air=true;
        player_back_turned_when_above=true;
    }
    //Zombie cow
    else if(type==NPC_ZOMBIE_COW){
        wears_tophat=true;
        tophat_adjust_x=6.0;
        tophat_adjust_y=-2.0;

        //Physics:

        max_speed=1.0;

        custom_physics=true;
        acceleration=0.20625;
        deceleration=0.375;
        air_drag=0.3;
        air_drag_divisor=64.0;
        friction=acceleration;
        air_accel=acceleration;
        air_decel=deceleration;

        undead=true;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=5;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_pause=true;

        follow_player=true;
        follow_player_range_x=50;
        follow_player_range_y=20;

        special_attack_type=SPECIAL_ATTACK_ZOMBIE_COW_MOO;
        special_attack_anytime=true;
        special_attack_frequency=random_range(300,1000);
        special_attack_sound_frame=1;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;
    }
    //Mummy cow
    else if(type==NPC_MUMMY_COW){
        wears_tophat=true;
        tophat_adjust_x=6.0;
        tophat_adjust_y=-2.0;

        //Physics:

        max_speed=0.5;

        custom_physics=true;
        acceleration=0.20625;
        deceleration=0.375;
        air_drag=0.3;
        air_drag_divisor=64.0;
        friction=acceleration;
        air_accel=acceleration;
        air_decel=deceleration;

        undead=true;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=5;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;
        wander_pause=true;

        follow_player=true;
        follow_player_range_x=50;
        follow_player_range_y=20;

        special_attack_type=SPECIAL_ATTACK_MUMMY_COW_MOO;
        special_attack_anytime=true;
        special_attack_frequency=random_range(300,1000);
        special_attack_sound_frame=1;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;
    }
    //Ghost
    else if(type==NPC_GHOST){
        wears_tophat=true;
        tophat_adjust_x=6.0;
        tophat_adjust_y=-2.0;

        //Physics:

        max_speed=0.5;
        FLYING=true;

        custom_physics=true;
        acceleration=0.20625;
        deceleration=0.375;
        air_drag=0.3;
        air_drag_divisor=64.0;
        friction=acceleration;
        air_accel=acceleration;
        air_decel=deceleration;

        ethereal=true;

        deadly_while_special_attacking=false;

        undead=true;

        //AI:

        wander=true;
        wander_up_and_down=true;
        wander_frequency_min=200;
        wander_frequency_max=350;

        follow_player=true;
        follow_player_range_x=25;
        follow_player_range_y=15;
        follow_player_only_when_back_turned=true;

        special_attack_type=SPECIAL_ATTACK_GHOST_HIDE;
        special_attack_on_sight=true;
        special_attack_only_when_player_facing=true;
        special_attack_range_x=25;
        special_attack_range_y=15;
        special_attack_frequency=0;

        //Rendering:

        frame_speed_fly=120;
        frame_speed_special_attack=50;

        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_YELLOW);
        special_attack_toggles_light=true;
        special_attack_end_toggles_light=true;
    }
    //Spider
    else if(type==NPC_SPIDER){
        wears_tophat=true;

        //Physics:

        max_speed=1.0;
        FLYING=true;

        //AI:

        can_be_duplicated=false;

        wander=true;
        wander_notice_walls=true;
        wander_up_and_down_only=true;
        wander_frequency_min=200;
        wander_frequency_max=350;
    }
    //Cat
    else if(type==NPC_CAT){
        wears_tophat=true;
        tophat_adjust_x=18.0;
        tophat_adjust_y=4.0;

        //Physics:

        max_speed=2.0;
        max_speed_in_air=4.0;
        jump_max=12.0;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=2;

        wander=true;
        wander_notice_edges=true;
        wander_notice_walls=true;

        follow_player=true;
        follow_player_range_x=9600;
        follow_player_range_y=9600;
        follow_player_only_in_air=true;

        special_attack_type=SPECIAL_ATTACK_CAT_POUNCE;
        special_attack_on_sight=true;
        special_attack_only_when_facing_player=true;
        special_attack_range_x=8;
        special_attack_range_y=4;
        special_attack_frequency=300;

        //Rendering:

        frame_speed_special_attack=10;
    }
    //Spider walking
    else if(type==NPC_SPIDER_WALKING){
        wears_tophat=true;

        //Physics:

        max_speed=4.0;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=3;

        wander=true;
        wander_notice_walls=true;
        jump=true;
    }
    //Ram
    else if(type==NPC_RAM){
        //Physics:

        max_speed=1.0;
        max_speed_during_special_attack=9.0;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=3;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=true;

        special_attack_type=SPECIAL_ATTACK_RAM_CHARGE;
        special_attack_on_sight=true;
        special_attack_only_when_facing_player=true;
        special_attack_range_x=25;
        special_attack_range_y=2;
        special_attack_frequency=600;
        special_attack_pause_length=15;
    }
    //Eagle
    else if(type==NPC_EAGLE){
        //Physics:

        max_speed=0.0;
        max_speed_during_special_attack=3.0;
        FLYING=true;

        //AI:

        can_be_duplicated=false;

        home_x=x;
        home_y=y;
        home_permanent=true;

        wander=true;
        notice_wall_ignore_cloud=true;

        special_attack_type=SPECIAL_ATTACK_EAGLE_DIVEBOMB;
        special_attack_on_sight=true;
        special_attack_range_x=12;
        special_attack_range_y=12;
        special_attack_below_only=true;
        special_attack_frequency=300;

        //Rendering:

        frame_speed_special_attack=6;
    }
    //Snowman
    else if(type==NPC_SNOWMAN){
        //Physics:

        max_speed=2.0;
        jump_max=6.0;

        //AI:

        wander=true;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=300;
        wander_weight=100;

        jump=true;
        jump_only=true;
        jump_frequency=0;

        special_attack_type=SPECIAL_ATTACK_SNOWMAN_THROW_SNOWBALL;
        special_attack_on_sight=true;
        special_attack_range_x=8;
        special_attack_range_y=2;
        special_attack_frequency=60;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_special_attack=4;
    }
    //Fireball
    else if(type==NPC_FIREBALL){
        wears_tophat=true;
        tophat_adjust_x=32.0;
        tophat_adjust_y=0.0;

        //Physics:

        max_speed=2.0;
        FLYING=true;

        //AI:

        wander=true;
        wander_notice_walls=true;
        wander_frequency_min=400;
        wander_frequency_max=800;
        wander_up_and_down=true;

        //Rendering:

        frame_speed_fly=40;

        light_source.on=true;
        light_source.color=color_name_to_doubles(COLOR_ORANGE);
        light_source.radius=4*(TILE_SIZE/LIGHTING_TILE_SIZE);
        light_source.falloff=0.28/(TILE_SIZE/LIGHTING_TILE_SIZE);
    }
    //Dragon whelp
    else if(type==NPC_DRAGON_WHELP){
        wears_tophat=true;
        tophat_adjust_x=10.0;
        tophat_adjust_y=2.0;

        //Physics:

        max_speed=4.0;
        FLYING=true;

        //AI:

        wander=true;
        wander_notice_walls=true;
        wander_frequency_min=400;
        wander_frequency_max=800;

        special_attack_type=SPECIAL_ATTACK_DRAGON_WHELP_SHOOT_FIREBALL;
        special_attack_anytime=true;
        special_attack_frequency=60;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_fly=25;
    }
    //Fireball shooter left
    else if(type==NPC_FIREBALL_SHOOTER_LEFT){
        //Physics:

        invulnerable=true;

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        special_attack_type=SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_LEFT;
        special_attack_anytime=true;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;
    }
    //Fireball shooter right
    else if(type==NPC_FIREBALL_SHOOTER_RIGHT){
        //Physics:

        invulnerable=true;

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        special_attack_type=SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_RIGHT;
        special_attack_anytime=true;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;
    }
    //Fireball shooter up
    else if(type==NPC_FIREBALL_SHOOTER_UP){
        //Physics:

        invulnerable=true;

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        special_attack_type=SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_UP;
        special_attack_anytime=true;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;
    }
    //Fireball shooter down
    else if(type==NPC_FIREBALL_SHOOTER_DOWN){
        //Physics:

        invulnerable=true;

        never_move=true;

        max_speed=0.0;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        special_attack_type=SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_DOWN;
        special_attack_anytime=true;
        special_attack_frequency=150;
        special_attack_ends_with_animation=true;
    }
    //Dust devil
    else if(type==NPC_DUST_DEVIL){
        wears_tophat=true;

        //Physics:

        max_speed=5.0;
        FLYING=true;

        //AI:

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=true;
        wander_frequency_min=100;
        wander_frequency_max=200;
        wander_pause=true;
        wander_weight=80;
    }
    //Scorpion
    else if(type==NPC_SCORPION){
        wears_tophat=true;
        tophat_adjust_x=18.0;
        tophat_adjust_y=5.0;

        //Physics:

        max_speed=3.0;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=3;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=true;

        special_attack_type=SPECIAL_ATTACK_SCORPION_SHOOT_POISON;
        special_attack_on_sight=true;
        special_attack_range_x=8;
        special_attack_range_y=2;
        special_attack_only_when_facing_player=true;
        special_attack_frequency=60;
        special_attack_ends_with_animation=true;

        //Rendering:

        frame_speed_special_attack=4;
    }
    //Sphinx left
    else if(type==NPC_SPHINX_LEFT){
        //Physics:

        never_move=true;

        max_speed=0.0;
        invulnerable=true;
        facing=LEFT;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        special_attack_type=SPECIAL_ATTACK_SPHINX_SUCK_LEFT;
        special_attack_on_sight=true;
        special_attack_range_x=8;
        special_attack_range_y=2;
        special_attack_only_when_facing_player=true;
        special_attack_frequency=0;
    }
    //Sphinx right
    else if(type==NPC_SPHINX_RIGHT){
        //Physics:

        never_move=true;

        max_speed=0.0;
        invulnerable=true;
        facing=RIGHT;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        //AI:

        can_be_duplicated=false;

        counts_as_trap=true;

        special_attack_type=SPECIAL_ATTACK_SPHINX_SUCK_RIGHT;
        special_attack_on_sight=true;
        special_attack_range_x=8;
        special_attack_range_y=2;
        special_attack_only_when_facing_player=true;
        special_attack_frequency=0;
    }
    //Shark
    else if(type==NPC_SHARK){
        //Physics:

        invulnerable=true;

        max_speed=0.3;
        max_swim_speed=1.0;
        max_buoyancy=1.0;

        swimming_gear=true;

        //AI:

        deadly_to_touch=true;
        deadly_while_special_attacking=true;

        wander=true;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_up_and_down=true;

        follow_player=true;
        follow_player_range_x=24;
        follow_player_range_y=24;
        follow_player_only_in_water=true;

        //Rendering:

        frame_speed_swim=40;
    }
    //Henchmouse
    else if(type==NPC_HENCHMOUSE){
        wears_tophat=true;
        tophat_adjust_x=2.0;
        tophat_adjust_y=4.0;

        //Physics:

        max_speed=2.0;
        max_speed_during_special_attack=3.0;
        max_speed_during_alarm=4.0;
        jump_max=10.0;

        //AI:

        has_footsteps=true;
        footstep_sound_frequency=6;
        footstep_special_attack_sound_frequency=5;

        follow_player_range_x=9600;
        follow_player_range_y=9600;
        follow_player_during_alarm=true;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=true;

        jump=true;
        jump_frequency=60;
        jump_percent=10;

        use=true;
        use_frequency=15;
        use_only_while_alarm_off=true;

        special_attack_type=SPECIAL_ATTACK_HENCHMOUSE_SOUND_ALARM;
        special_attack_on_sight=true;
        special_attack_range_x=12;
        special_attack_range_y=6;
        special_attack_only_when_facing_player=true;
        special_attack_only_when_alarm_off=true;
        special_attack_frequency=0;

        //Render:

        frame_speed_walk=7;
        frame_speed_special_attack=3;
    }
    //Mechsuit mouse
    else if(type==NPC_MECHSUIT_MOUSE){
        //Physics:

        max_speed=2.0;
        FLYING=true;

        //AI:

        wander=true;
        wander_notice_walls=true;

        special_attack_type=SPECIAL_ATTACK_MECHSUIT_MOUSE_FIRE_MISSILE;
        special_attack_on_sight=true;
        special_attack_range_x=12;
        special_attack_range_y=12;
        special_attack_only_when_facing_player=true;
        special_attack_frequency=60;
        special_attack_ends_with_animation=true;
    }
    //Sally
    else if(type==NPC_SALLY){
        //Physics:

        max_speed=1.0;
        invulnerable=true;

        //AI:

        can_be_duplicated=false;

        friendly=true;

        has_footsteps=true;
        footstep_sound_frequency=7;

        act_as_platform=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=true;
        wander_frequency_min=100;
        wander_frequency_max=200;
        wander_pause=true;
        wander_weight=80;

        follow_player=true;
        follow_player_range_x=4;
        follow_player_range_y=4;
    }
    //Bouncing cow
    else if(type==NPC_BOUNCING_COW){
        wears_tophat=true;

        //Physics:

        max_speed=0.4;
        jump_max=12.0;

        //AI:

        can_be_duplicated=false;

        friendly=true;

        act_as_platform=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        jump=true;
        jump_only=true;
        jump_frequency=0;
        jump_percent=100;

        wander=true;
        wander_notice_walls=true;
        wander_weight=90;
    }
    //Bouncing ball
    else if(type==NPC_BOUNCING_BALL){
        wears_tophat=true;

        //Physics:

        max_speed=0.4;
        jump_max=12.0;
        invulnerable=true;

        //AI:

        can_be_duplicated=false;

        friendly=true;

        drops_items=false;

        act_as_platform=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        jump=true;
        jump_only=true;
        jump_frequency=0;
        jump_percent=100;

        wander=true;
        wander_notice_walls=true;
        wander_weight=90;
    }
    //Sea turtle
    else if(type==NPC_SEA_TURTLE){
        wears_tophat=true;
        tophat_adjust_x=40.0;
        tophat_adjust_y=0.0;

        //Physics:

        invulnerable=true;

        max_speed=0.3;
        max_swim_speed=1.25;
        max_swim_speed_at_surface=9.5;
        max_buoyancy=1.0;

        swimming_gear=true;

        //AI:

        can_be_duplicated=false;

        friendly=true;

        act_as_platform=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;
        wander_notice_edges=false;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_up_and_down=true;

        follow_player=true;
        follow_player_range_x=12;
        follow_player_range_y=12;

        //Rendering:

        frame_speed_swim=60;
    }
    //Starfish
    else if(type==NPC_STARFISH){
        //Physics:

        invulnerable=true;

        max_speed=0.3;
        max_swim_speed=0.5;
        max_buoyancy=1.0;

        swimming_gear=true;

        set_color();

        //AI:

        can_be_duplicated=false;

        friendly=true;

        deadly_to_touch=false;
        deadly_while_special_attacking=false;

        wander=true;
        wander_notice_edges=false;
        wander_notice_walls=true;
        wander_frequency_min=200;
        wander_frequency_max=600;
        wander_up_and_down=true;

        follow_player=true;
        follow_player_range_x=6;
        follow_player_range_y=6;
        follow_player_only_in_water=true;
    }
    //Boss Castle
    else if(type==NPC_BOSS_CASTLE){
        //Physics:

        invulnerable=true;

        health_max=8;
        health=health_max;

        max_speed=1.0;
        never_move=true;

        FLYING=true;

        //AI:

        can_be_duplicated=false;

        BOSS=true;
        MAIN_BOSS=true;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=false;
        wander_pause=false;
        wander_frequency_min=7200;
        wander_frequency_max=10000;

        if(make_traps){
            //Create the boss's traps.
            vector_traps.push_back(Trap(x-100.0,y,TRAP_BOSS_CASTLE,true,-1,LEFT,2.0,100.0));
            vector_traps.push_back(Trap(x+150.0,y,TRAP_BOSS_CASTLE,true,-1,RIGHT,2.5,150.0));
            vector_traps.push_back(Trap(x-200.0,y,TRAP_BOSS_CASTLE,true,-1,LEFT,3.0,200.0));
            vector_traps.push_back(Trap(x+250.0,y,TRAP_BOSS_CASTLE,true,-1,RIGHT,3.5,250.0));
        }
    }
    //Boss Haunted House
    else if(type==NPC_BOSS_HAUNTED_HOUSE){
        //Physics:

        invulnerable=true;

        health_max=8;
        health=health_max;

        max_speed=1.5;
        never_move=true;

        FLYING=true;

        undead=true;

        //AI:

        can_be_duplicated=false;

        BOSS=true;
        MAIN_BOSS=true;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=false;
        wander_pause=false;
        wander_frequency_min=7200;
        wander_frequency_max=10000;

        special_attack_type=SPECIAL_ATTACK_BOSS_HAUNTED_HOUSE;
        special_attack_on_sight=true;
        special_attack_range_x=-1;
        special_attack_range_y=-1;
        special_attack_frequency=35;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;
    }
    //Boss Volcano
    else if(type==NPC_BOSS_VOLCANO){
        //Physics:

        invulnerable=true;

        health_max=8;
        health=health_max;

        max_speed=3.5;
        never_move=true;

        jump_max=25.0;
        gravity_max=15.0;
        gravity=0.6;

        //AI:

        can_be_duplicated=false;

        BOSS=true;
        MAIN_BOSS=true;

        jump=true;
        jump_only=true;
        jump_frequency=60;
        jump_percent=100;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=false;
        wander_pause=false;
        wander_frequency_min=7200;
        wander_frequency_max=10000;

        special_attack_type=SPECIAL_ATTACK_BOSS_VOLCANO;
        special_attack_on_sight=true;
        special_attack_range_x=-1;
        special_attack_range_y=-1;
        special_attack_frequency=65;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;
    }
    //Boss Pyramid
    else if(type==NPC_BOSS_PYRAMID){
        //Physics:

        invulnerable=true;

        health_max=8;
        health=health_max;

        max_speed=1.5;
        never_move=true;

        FLYING=true;

        //AI:

        can_be_duplicated=false;

        BOSS=true;
        MAIN_BOSS=true;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=false;
        wander_pause=false;
        wander_frequency_min=7200;
        wander_frequency_max=10000;

        special_attack_type=SPECIAL_ATTACK_BOSS_PYRAMID;
        special_attack_on_sight=true;
        special_attack_range_x=-1;
        special_attack_range_y=-1;
        special_attack_frequency=45;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;

        if(make_traps){
            //Create the boss's traps.
            vector_traps.push_back(Trap(x-100.0,y,TRAP_BOSS_PYRAMID,true,-1,LEFT,2.0,100.0));
            vector_traps.push_back(Trap(x+150.0,y,TRAP_BOSS_PYRAMID,true,-1,RIGHT,2.5,150.0));
        }
    }
    //Boss Lighthouse
    else if(type==NPC_BOSS_LIGHTHOUSE){
        //Physics:

        invulnerable=true;

        health_max=8;
        health=health_max;

        max_speed=1.5;
        never_move=true;

        FLYING=true;

        //AI:

        can_be_duplicated=false;

        BOSS=true;
        MAIN_BOSS=true;

        wander=true;
        wander_notice_walls=true;
        wander_notice_edges=false;
        wander_pause=false;
        wander_frequency_min=400;
        wander_frequency_max=800;
        wander_up_and_down=true;

        special_attack_type=SPECIAL_ATTACK_BOSS_LIGHTHOUSE;
        special_attack_on_sight=true;
        special_attack_range_x=-1;
        special_attack_range_y=-1;
        special_attack_frequency=55;
        special_attack_ends_with_animation=true;

        frame_speed_special_attack=1;
    }

    double speed_buff=0.0;

    //If the npc is undead, it speeds up at night.
    if(undead && player.is_night()){
        speed_buff+=0.5;
    }
    //If the npc is undead, it speeds up even more at midnight.
    if(undead && player.is_midnight()){
        speed_buff+=0.5;
    }

    short difficulty=player.return_actual_difficulty();

    if(!friendly){
        if(difficulty==DIFFICULTY_EASY){
            health_max/=2.0;
            health=health_max;

            speed_buff-=0.25;
        }
        else if(difficulty>=DIFFICULTY_HARD){
            health_max+=(short)ceil((double)health_max*(double)(difficulty-DIFFICULTY_NORMAL)*0.1);
            health=health_max;

            speed_buff+=(double)(difficulty-DIFFICULTY_NORMAL)*0.05;
        }
    }

    if(speed_buff>0.0){
        max_speed+=max_speed*speed_buff;

        if(max_speed>10.0){
            max_speed=10.0;
        }
    }
    else if(speed_buff<0.0){
        max_speed-=max_speed*fabs(speed_buff);

        if(max_speed<0.25){
            max_speed=0.25;
        }
    }

    if(swimming_gear){
        //Start timing how long the actor has been in the water.
        timer_time_in_water.start();

        SWIMMING=true;
        CLIMBING=false;
        climb_jump_timer=climb_jump_delay;
        IN_AIR=false;
        air_velocity=0;
    }

    wander_frequency=random_range(wander_frequency_min,wander_frequency_max);

    //Set the wander counter to the wander frequency, so
    //the npc starts off by making an ai decision.
    wander_counter=wander_frequency;

    default_max_swim_speed=max_swim_speed;

    default_max_speed=max_speed;

    if(!custom_physics){
        acceleration=max_speed*2;
        deceleration=max_speed*2;
        air_drag=0.3;
        air_drag_divisor=64.0;
        friction=acceleration*2;
        air_accel=acceleration*2;
        air_decel=deceleration*2;
    }

    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        w/=2.0;
        h/=2.0;
    }*/
}

void Npc::set_color(){
    color=random_range(COLOR_YELLOW,COLOR_SPRING);
}

void Npc::handle_input(){
    //If the npc is set to jump and only jump while moving,
    //and if the npc is not currently in the air.
    if(jump && jump_only && !IN_AIR){
        //Clear any movement keys.
        ai_keystates[AIK_LEFT]=false;
        ai_keystates[AIK_UP]=false;
        ai_keystates[AIK_RIGHT]=false;
        ai_keystates[AIK_DOWN]=false;
    }
    //If the npc is currently heading upwards on a jump.
    if(jump_state){
        //Keep holding the jump key for maximum jump height.
        ai_keystates[AIK_JUMP]=true;
    }

    //************************//
    // Handle direction keys: //
    //************************//

    //If the left directional key is pressed.
    if(ai_keystates[AIK_LEFT]){
        //Set their move state to match the directional key.
        move_state=LEFT;

        //Set their facing variable to match the directional key.
        facing=LEFT;
    }

    //If the up directional key is pressed.
    if(ai_keystates[AIK_UP]){
        move_state=UP;
    }

    //If the npc is swimming and the jump key is pressed, mimic the up direction key's swimming function.
    if(SWIMMING && ai_keystates[AIK_JUMP]){
        move_state=UP;
    }

    //If the right directional key is pressed.
    if(ai_keystates[AIK_RIGHT]){
        //Set their move state to match the directional key.
        move_state=RIGHT;

        //Set their facing variable to match the directional key.
        facing=RIGHT;
    }

    //If the down directional key is pressed.
    if(ai_keystates[AIK_DOWN]){
        move_state=DOWN;
    }

    //*******************************************************//
    // Handle multiple direction keys being pressed at once: //
    //*******************************************************//

    //If swimming or flying, 8 directions of movement are possible.
    if(SWIMMING || FLYING){
        //Priorities:
        //Movement: Left, Up, Right, Down.

        if(ai_keystates[AIK_LEFT] && (ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP])){
            move_state=LEFT_UP;
        }
        if(ai_keystates[AIK_RIGHT] && (ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP])){
            move_state=RIGHT_UP;
        }
        if(ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=RIGHT_DOWN;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_DOWN]){
            move_state=LEFT_DOWN;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_RIGHT]){
            move_state=LEFT;
        }
        if((ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP]) && ai_keystates[AIK_DOWN]){
            move_state=UP;
        }
        if(ai_keystates[AIK_LEFT] && (ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP]) && ai_keystates[AIK_RIGHT]){
            move_state=LEFT_UP;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_DOWN] && ai_keystates[AIK_RIGHT]){
            move_state=LEFT_DOWN;
        }
        if(ai_keystates[AIK_LEFT] && (ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP]) && ai_keystates[AIK_DOWN]){
            move_state=LEFT_UP;
        }
        if((ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP]) && ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=RIGHT_UP;
        }
        if(ai_keystates[AIK_LEFT] && (ai_keystates[AIK_UP] || ai_keystates[AIK_JUMP]) && ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=LEFT_UP;
        }

        //If no directional keys are pressed, the player is not moving.
        if(!ai_keystates[AIK_LEFT] && (!ai_keystates[AIK_UP] && !ai_keystates[AIK_JUMP]) && !ai_keystates[AIK_RIGHT] && !ai_keystates[AIK_DOWN]){
            move_state=0;
        }
    }

    //If on land or in the air, 4 directions of movement are possible.
    else if(!SWIMMING && !FLYING){
        //Priorities:
        //Movement: Left, Up, Right, Down.

        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_UP]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_RIGHT] && ai_keystates[AIK_UP]){
            move_state=RIGHT;
            facing=RIGHT;
        }
        if(ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=RIGHT;
            facing=RIGHT;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_DOWN]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_RIGHT]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_UP] && ai_keystates[AIK_DOWN]){
            move_state=UP;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_UP] && ai_keystates[AIK_RIGHT]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_DOWN] && ai_keystates[AIK_RIGHT]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_UP] && ai_keystates[AIK_DOWN]){
            move_state=LEFT;
            facing=LEFT;
        }
        if(ai_keystates[AIK_UP] && ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=RIGHT;
            facing=RIGHT;
        }
        if(ai_keystates[AIK_LEFT] && ai_keystates[AIK_UP] && ai_keystates[AIK_RIGHT] && ai_keystates[AIK_DOWN]){
            move_state=LEFT;
            facing=LEFT;
        }

        //If no directional keys are pressed, the player is not moving.
        if(!ai_keystates[AIK_LEFT] && !ai_keystates[AIK_UP] && !ai_keystates[AIK_RIGHT] && !ai_keystates[AIK_DOWN]){
            frame=0;
            frame_counter=0;
            move_state=0;
        }
    }

    //********************************//
    // Handle dropping through cloud: //
    //********************************//

    if(ai_keystates[AIK_JUMP] && ai_keystates[AIK_DOWN] && !IN_AIR){
        if(on_cloud){
            on_cloud=false;
            //If the npc is on a moving platform that is moving down.
            if(moving_platform_y>0.0){
                air_velocity=(4.0/10.0+0.4)*gravity_max;
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }
                y+=moving_platform_y*5;
            }
            else{
                air_velocity=0;
                y+=10;
            }
            IN_AIR=true;
            frame_jump=0;
        }
    }

    //*****************//
    // Handle jumping: //
    //*****************//

    //If the player presses the jump key and all the conditions for a jump are met.
    if(ai_keystates[AIK_JUMP] && !IN_AIR && !jump_state && ((SWIMMING && SWIM_CAN_JUMP) || !SWIMMING)){
        jump_state=true;

        air_velocity=jump_max*-1;

        //The player has begun a jump, so set them to be in the air.
        IN_AIR=true;
        frame_jump=0;

        play_positional_sound(sound_system.npc_jump,x,y);
    }

    if(!ai_keystates[AIK_JUMP] && jump_state){
        jump_state=false;

        if(air_velocity<jump_min*-1){
            air_velocity=jump_min*-1;
        }
    }

    //This is a special jump thing only needed for NPC AI.
    if(jump_state && !IN_AIR){
        jump_state=false;
    }

    if(ai_keystates[AIK_USE]){
        for(int i=0;i<vector_triggers.size();i++){
            if(fabs(vector_triggers[i].x-x)<PROCESS_RANGE && fabs(vector_triggers[i].y-y)<PROCESS_RANGE){
                if(type!=NPC_REPLICATOR){
                    //If the trigger is active, its user type is npc, and it is manually activated.
                    if(vector_triggers[i].active && (vector_triggers[i].user_type==TRIGGER_USER_ALL) && !vector_triggers[i].trigger_method){
                        if(collision_check(x,y,w,h,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                            vector_triggers[i].use();
                            play_positional_sound(sound_system.trigger_click,vector_triggers[i].x,vector_triggers[i].y);
                            break;
                        }
                    }
                }
                //Special Replicator-only way to use triggers.
                else{
                    if(vector_triggers[i].active && !vector_triggers[i].trigger_method){
                        if(collision_check(x-TILE_SIZE*4,y-TILE_SIZE*4,w+TILE_SIZE*8,h+TILE_SIZE*8,vector_triggers[i].x,vector_triggers[i].y,vector_triggers[i].w,vector_triggers[i].h)){
                            vector_triggers[i].use();
                            play_positional_sound(sound_system.trigger_click,vector_triggers[i].x,vector_triggers[i].y);
                            break;
                        }
                    }
                }
            }
        }
    }

    after_input();

    ///******************///
    /// Debug/Test Code. ///
    ///******************///



    ///*************************///
    /// End of Debug/Test Code. ///
    ///*************************///
}

void Npc::move(){
    if(counter_targeted>0){
        counter_targeted--;
    }

    if(!never_move){
        bool events_handled=false;

        speedometer=run_speed+moving_platform_x;
        if(sucked_left && !sucked_right){
            speedometer-=SUCK_SPEED;
        }
        else if(!sucked_left && sucked_right){
            speedometer+=SUCK_SPEED;
        }
        speedometer=fabs(speedometer);

        if(sucked_left && !sucked_right){
            move_suck(LEFT);
            handle_events();
            events_handled=true;
        }
        else if(!sucked_left && sucked_right){
            move_suck(RIGHT);
            handle_events();
            events_handled=true;
        }

        if(exists){
            handle_counters();
        }

        if(max_swim_speed_at_surface>0.0 && SWIMMING && SWIM_CAN_JUMP){
            max_swim_speed=max_swim_speed_at_surface;
        }
        else{
            max_swim_speed=default_max_swim_speed;
        }

        if(max_speed_in_air>0.0 && IN_AIR && !SWIMMING){
            max_speed=max_speed_in_air;
        }
        else if(max_speed_during_special_attack>0.0 && special_attack_in_progress){
            max_speed=max_speed_during_special_attack;
        }
        else if(max_speed_during_alarm>0.0 && level.alarm){
            max_speed=max_speed_during_alarm;
        }
        else{
            max_speed=default_max_speed;
        }

        if(!custom_physics){
            acceleration=max_speed;
            deceleration=max_speed;
            friction=acceleration;
            air_accel=acceleration;
            air_decel=deceleration;
        }

        if(exists && !FLYING){
            //*************************//
            // Handle x-axis movement: //
            //*************************//

            ///////////////////////////////////////////////
            //Move the npc according to their move state://
            ///////////////////////////////////////////////

            double run_chunk;
            if(fabs(run_speed)<pixel_safety_x){
                run_chunk=fabs(run_speed);
            }
            else{
                run_chunk=pixel_safety_x;
            }

            for(double i=fabs(run_speed);i>0;){
                //If we have run_chunk or more pixels left to move,
                //we will move run_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than run_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<run_chunk){
                    run_chunk=i;
                    i=0;
                }

                //Move.
                if(run_speed<0){
                    run_chunk*=-1;
                }
                x+=run_chunk;
                if(run_speed<0){
                    run_chunk*=-1;
                }

                //If we still have pixels left to move.
                if(i!=0){
                    i-=run_chunk;
                }

                handle_events();
                events_handled=true;
            }

            if(!SWIMMING){
                //Handle acceleration.
                double accel_to_use=acceleration;
                if(IN_AIR){
                    accel_to_use=air_accel;
                }

                if(run_speed<max_speed && run_speed>max_speed*-1){
                    if(move_state==LEFT && run_speed<=0.0){
                        run_speed-=accel_to_use;
                    }
                    else if(move_state==RIGHT && run_speed>=0.0){
                        run_speed+=accel_to_use;
                    }
                    if(run_speed>max_speed){
                        run_speed=max_speed;
                    }
                    else if(run_speed<max_speed*-1){
                        run_speed=max_speed*-1;
                    }
                }
                //Handle deceleration.
                double decel_to_use=deceleration;
                if(IN_AIR){
                    decel_to_use=air_decel;
                }
                if(move_state==LEFT && run_speed>0.0){
                    if(run_speed-decel_to_use<0.0){
                        run_speed=0.0-decel_to_use;
                    }
                    else{
                        run_speed-=decel_to_use;
                    }
                }
                else if(move_state==RIGHT && run_speed<0.0){
                    if(run_speed+decel_to_use>0.0){
                        run_speed=decel_to_use;
                    }
                    else{
                        run_speed+=decel_to_use;
                    }
                }
                if(!IN_AIR){
                    //Handle friction.
                    if(move_state!=LEFT && move_state!=RIGHT && run_speed!=0.0){
                        if(run_speed<0.0){
                            run_speed+=friction;
                            if(run_speed>0.0){
                                run_speed=0.0;
                            }
                        }
                        else if(run_speed>0.0){
                            run_speed-=friction;
                            if(run_speed<0.0){
                                run_speed=0.0;
                            }
                        }
                    }
                    if(!MOVING_PLATFORM_X_THIS_FRAME && moving_platform_x!=0.0){
                        if(moving_platform_x<0.0){
                            moving_platform_x+=friction;
                            if(moving_platform_x>0.0){
                                moving_platform_x=0.0;
                            }
                        }
                        else if(moving_platform_x>0.0){
                            moving_platform_x-=friction;
                            if(moving_platform_x<0.0){
                                moving_platform_x=0.0;
                            }
                        }
                    }
                }
                else{
                    //Handle air drag.
                    if(air_velocity<0.0 && air_velocity>jump_min*-1 && air_drag!=0.0){
                        if(run_speed<0.0){
                            run_speed+=(fabs(run_speed)/air_drag)/air_drag_divisor;
                        }
                        else if(run_speed>0.0){
                            run_speed-=(fabs(run_speed)/air_drag)/air_drag_divisor;
                        }
                        if(moving_platform_x<0.0){
                            moving_platform_x+=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                        }
                        else if(moving_platform_x>0.0){
                            moving_platform_x-=(fabs(moving_platform_x)/air_drag)/air_drag_divisor;
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////
            //Move the npc according to their moving platform x modifier://
            ///////////////////////////////////////////////////////////////

            if(fabs(moving_platform_x)<pixel_safety_x){
                run_chunk=fabs(moving_platform_x);
            }
            else{
                run_chunk=pixel_safety_x;
            }

            for(double i=fabs(moving_platform_x);i>0;){
                //If we have run_chunk or more pixels left to move,
                //we will move run_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than run_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<run_chunk){
                    run_chunk=i;
                    i=0;
                }

                //Move.
                if(moving_platform_x<0.0){
                    run_chunk*=-1;
                }
                x+=run_chunk;
                if(moving_platform_x<0.0){
                    run_chunk*=-1;
                }

                //If we still have pixels left to move.
                if(i!=0){
                    i-=run_chunk;
                }

                handle_events();
                events_handled=true;
            }

            //*************************//
            // Handle y-axis movement: //
            //*************************//

            ///////////////////////////////////////////////////
            //Move the npc according to their swimming state://
            ///////////////////////////////////////////////////

            //If the npc is swimming.
            if(SWIMMING){
                double float_chunk;
                if(fabs(float_speed)<pixel_safety_y){
                    float_chunk=fabs(float_speed);
                }
                else{
                    float_chunk=pixel_safety_y;
                }

                for(double i=fabs(float_speed);i>0;){
                    //If we have float_chunk or more pixels left to move,
                    //we will move float_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than float_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<float_chunk){
                        float_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(float_speed<0){
                        float_chunk*=-1;
                    }

                    y+=float_chunk;

                    if(float_speed<0){
                        float_chunk*=-1;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=float_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }
            }

            /////////////////////////////////////////////////
            //Move the npc according to their air_velocity://
            /////////////////////////////////////////////////

            //If the npc is in the air.
            if(IN_AIR){
                double air_chunk;
                if(fabs(air_velocity)<pixel_safety_y){
                    air_chunk=fabs(air_velocity);
                }
                else{
                    air_chunk=pixel_safety_y;
                }

                //First, translate the npc based on his air velocity.
                for(double i=fabs(air_velocity);i>0;){
                    //If we have air_chunk or more pixels left to move,
                    //we will move air_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than air_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<air_chunk){
                        air_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(air_velocity<0){
                        y-=air_chunk;
                    }
                    else if(air_velocity>0){
                        y+=air_chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=air_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }

                //Then, we handle gravity, which will affect the next tick.

                //As long as air_velocity hasn't exceeded gravity_max, the maximum speed an object can fall, add gravity to air_velocity.
                if(air_velocity<gravity_max){
                    air_velocity+=gravity;
                }
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }
            }

            //*******************************//
            // Handle swimming acceleration: //
            //*******************************//

            //If the npc is swimming but isn't currently moving on the y-axis.
            if(SWIMMING && move_state!=UP && move_state!=LEFT_UP && move_state!=RIGHT_UP && move_state!=DOWN && move_state!=LEFT_DOWN && move_state!=RIGHT_DOWN){
                //Float upwards.
                if(float_speed>-max_buoyancy){
                    //If the time in water is long enough.
                    if(timer_time_in_water.get_ticks()>=500){
                        //Stop the timer so the npc's buoyancy acceleration will slow to the standard rate.
                        timer_time_in_water.stop();
                    }

                    //If the timer is not running, buoyancy acceleration is normal.
                    if(!timer_time_in_water.is_started()){
                        float_speed-=swim_acceleration/4;
                    }
                    //If the timer is running, buoyancy acceleration is increased.
                    else if(timer_time_in_water.is_started()){
                        float_speed-=swim_acceleration*4;
                    }

                    if(float_speed<-max_buoyancy){
                        float_speed=-max_buoyancy;
                    }
                    if(float_speed<-max_swim_speed){
                        float_speed=-max_swim_speed;
                    }
                }
            }
            //Slowly stop swimming on the x-axis.
            if(SWIMMING && move_state!=LEFT && move_state!=LEFT_DOWN && move_state!=LEFT_UP && move_state!=RIGHT && move_state!=RIGHT_DOWN && move_state!=RIGHT_UP){
                if(run_speed>0){
                    run_speed-=swim_friction;
                    if(run_speed<0){
                        run_speed=0;
                    }
                }
                else if(run_speed<0){
                    run_speed+=swim_friction;
                    if(run_speed>0){
                        run_speed=0;
                    }
                }
            }
            if(SWIMMING && moving_platform_x!=0.0){
                if(moving_platform_x<0.0){
                    moving_platform_x+=friction;
                    if(moving_platform_x>0.0){
                        moving_platform_x=0.0;
                    }
                }
                else if(moving_platform_x>0.0){
                    moving_platform_x-=friction;
                    if(moving_platform_x<0.0){
                        moving_platform_x=0.0;
                    }
                }
            }
            //If the npc is swimming and is moving.
            //Don't allow the npc to swim beyond the maximum swim speed.
            if(SWIMMING && move_state!=0){
                if(run_speed<max_swim_speed && run_speed>max_swim_speed*-1){
                    if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed<=0.0){
                        run_speed-=swim_acceleration;
                    }
                    else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed>=0.0){
                        run_speed+=swim_acceleration;
                    }
                    if(run_speed>max_swim_speed){
                        run_speed=max_swim_speed;
                    }
                    else if(run_speed<max_swim_speed*-1){
                        run_speed=max_swim_speed*-1;
                    }
                }
                if(float_speed<max_swim_speed && float_speed>max_swim_speed*-1){
                    if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed<=0.0){
                        float_speed-=swim_acceleration;
                    }
                    else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed>=0.0){
                        float_speed+=swim_acceleration;
                    }
                    if(float_speed>max_swim_speed){
                        float_speed=max_swim_speed;
                    }
                    else if(float_speed<max_swim_speed*-1){
                        float_speed=max_swim_speed*-1;
                    }
                }
                if((move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP) && run_speed>0.0){
                    if(run_speed-swim_deceleration<0.0){
                        run_speed=0.0-swim_deceleration;
                    }
                    else{
                        run_speed-=swim_deceleration;
                    }
                }
                else if((move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP) && run_speed<0.0){
                    if(run_speed+swim_deceleration>0.0){
                        run_speed=swim_deceleration;
                    }
                    else{
                        run_speed+=swim_deceleration;
                    }
                }
                if((move_state==UP || move_state==LEFT_UP || move_state==RIGHT_UP) && float_speed>0.0){
                    if(float_speed-swim_deceleration<0.0){
                        float_speed=0.0-swim_deceleration;
                    }
                    else{
                        float_speed-=swim_deceleration;
                    }
                }
                else if((move_state==DOWN || move_state==LEFT_DOWN || move_state==RIGHT_DOWN) && float_speed<0.0){
                    if(float_speed+swim_deceleration>0.0){
                        float_speed=swim_deceleration;
                    }
                    else{
                        float_speed+=swim_deceleration;
                    }
                }
            }
        }

        //****************//
        // Handle flying: //
        //****************//

        else if(exists && FLYING){

            //************************************//
            // Handle x-axis and y-axis movement: //
            //************************************//

            /////////////////////////////////////////////
            //Move the npc according to its move state://
            /////////////////////////////////////////////

            double run_chunk;
            if(fabs(max_speed)<pixel_safety_y){
                run_chunk=fabs(max_speed);
            }
            else{
                run_chunk=pixel_safety_y;
            }

            for(double i=fabs(max_speed);i>0;){
                //If we have run_chunk or more pixels left to move,
                //we will move run_chunk pixels, call handle_events(), and loop back up here.

                //Or, if we have less than run_chunk pixels left to move,
                //we will move whatever pixels we have left and call handle_events() once more.
                if(i<run_chunk){
                    run_chunk=i;
                    i=0;
                }

                //Move.
                if(move_state==LEFT){
                    x-=run_chunk;
                }
                else if(move_state==RIGHT){
                    x+=run_chunk;
                }
                else if(move_state==UP){
                    y-=run_chunk;
                }
                else if(move_state==DOWN){
                    y+=run_chunk;
                }
                else if(move_state==LEFT_UP){
                    x-=run_chunk;
                    y-=run_chunk;
                }
                else if(move_state==RIGHT_UP){
                    x+=run_chunk;
                    y-=run_chunk;
                }
                else if(move_state==RIGHT_DOWN){
                    x+=run_chunk;
                    y+=run_chunk;
                }
                else if(move_state==LEFT_DOWN){
                    x-=run_chunk;
                    y+=run_chunk;
                }

                //If we still have pixels left to move.
                if(i!=0){
                    i-=run_chunk;
                }

                handle_events();
                events_handled=true;
            }
        }

        //If the npc is dead.
        else if(!exists){
            /////////////////////////////////////////////////
            //Move the npc according to their air_velocity://
            /////////////////////////////////////////////////

            //If the npc is in the air.
            if(IN_AIR){
                double air_chunk;
                if(fabs(air_velocity)<pixel_safety_y){
                    air_chunk=fabs(air_velocity);
                }
                else{
                    air_chunk=pixel_safety_y;
                }

                //First, translate the npc based on his air velocity.
                for(double i=fabs(air_velocity);i>0;){
                    //If we have air_chunk or more pixels left to move,
                    //we will move air_chunk pixels, call handle_events(), and loop back up here.

                    //Or, if we have less than air_chunk pixels left to move,
                    //we will move whatever pixels we have left and call handle_events() once more.
                    if(i<air_chunk){
                        air_chunk=i;
                        i=0;
                    }

                    //Move.
                    if(air_velocity<0){
                        y-=air_chunk;
                    }
                    else if(air_velocity>0){
                        y+=air_chunk;
                    }

                    //If we still have pixels left to move.
                    if(i!=0){
                        i-=air_chunk;
                    }

                    handle_events();
                    events_handled=true;
                }

                //Then, we handle gravity, which will affect the next tick.

                //Handle gravity normally.
                //As long as air_velocity hasn't exceeded gravity_max, the maximum speed an object can fall, add gravity to air_velocity.
                if(air_velocity<gravity_max){
                    air_velocity+=gravity;
                }
                if(air_velocity>gravity_max){
                    air_velocity=gravity_max;
                }
            }
            else{
                handle_events();
                events_handled=true;
            }
        }

        if(!events_handled){
            handle_events();
        }

        sucked_left=false;
        sucked_right=false;
    }
}

void Npc::handle_tiles(int check_x_start,int check_x_end,int check_y_start,int check_y_end){
    for(int int_y=check_y_start;int_y<check_y_end;int_y++){
        for(int int_x=check_x_start;int_x<check_x_end;int_x++){
            //As long as the current tile is within the level's boundaries.
            if(int_x>=0 && int_x<=(level.level_x/TILE_SIZE)-1 && int_y>=0 && int_y<=(level.level_y/TILE_SIZE)-1){
                //If the npc is not climbing.
                if(!CLIMBING){

                    handle_collision_tiles(int_x,int_y,false);

                    //If the tile is a hazard tile.
                    if(level.tile_array[int_x][int_y].special==TILE_SPECIAL_HAZARD){
                        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y()+6,TILE_SIZE,TILE_SIZE-6)){
                            ///I need to decide what happens to NPCs that touch hazards.
                            handle_death();
                        }
                    }

                    //If the tile is a water tile and water tiles are deadly.
                    else if(player.deadly_water && SWIMMING && level.tile_array[int_x][int_y].special==TILE_SPECIAL_WATER){
                        if(collision_check(return_x(),return_y(),return_w(),return_h(),level.tile_array[int_x][int_y].return_x(),level.tile_array[int_x][int_y].return_y()+6,TILE_SIZE,TILE_SIZE-6)){
                            ///I need to decide what happens to NPCs that touch deadly water.
                            handle_death();
                        }
                    }
                }

                //If the npc is currently climbing.
                else{
                    handle_climbing(int_x,int_y);
                }
            }
        }
    }
}

void Npc::handle_events(bool being_pushed_up){
    prepare_for_events();

    //The current tile location for the actor.
    int actor_current_x=(int)((int)x/TILE_SIZE);
    int actor_current_y=(int)((int)y/TILE_SIZE);

    //Check all tiles in a square around the actor.
    int check_x_start=actor_current_x-4;
    int check_x_end=actor_current_x+4;
    int check_y_start=actor_current_y-4;
    int check_y_end=actor_current_y+4;

    if(!ethereal){
        //*********************************************//
        // Check for collisions with moving platforms: //
        //*********************************************//
        short solidity;
        if(ethereal){
            solidity=TILE_SOLIDITY_PASSABLE;
        }
        else{
            solidity=TILE_SOLIDITY_CLOUD;
        }
        handle_collision_moving_platforms(solidity,counts_as_trap);

        //*********************************//
        // Check for collisions with npcs: //
        //*********************************//
        for(int i=0;i<vector_npcs.size();i++){
            if(this!=&vector_npcs[i] && exists && vector_npcs[i].exists){
                if(fabs(vector_npcs[i].x-x)<PROCESS_RANGE && fabs(vector_npcs[i].y-y)<PROCESS_RANGE){
                    if(!CLIMBING && !vector_npcs[i].CLIMBING){
                        if(ethereal || vector_npcs[i].ethereal || ethereal_to_npcs || vector_npcs[i].ethereal_to_npcs){
                            solidity=TILE_SOLIDITY_PASSABLE;
                        }
                        else if(act_as_platform && vector_npcs[i].act_as_platform){
                            solidity=TILE_SOLIDITY_CLOUD;
                        }
                        else if(SWIMMING && vector_npcs[i].SWIMMING){
                            solidity=TILE_SOLIDITY_PASSABLE;
                        }
                        else{
                            solidity=TILE_SOLIDITY_SOLID;
                        }
                        handle_collision_solid(vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h,solidity,vector_npcs[i].type);

                        handle_npc_platform(i);

                        //If this npc is acting as a platform.
                        if(!vector_npcs[i].SWIMMING && act_as_platform && !ethereal && !vector_npcs[i].ethereal){
                            if(push_actor_up(&vector_npcs[i],vector_npcs[i].x,vector_npcs[i].w,being_pushed_up,type)){
                                vector_npcs[i].handle_events(true);
                            }
                        }
                    }

                    //If the npc touches any part of the npc, and the npc exists and is deadly to the touch.
                    /**if(vector_npcs[i].exists && vector_npcs[i].deadly_to_touch && collision_check(x,y,w,h,vector_npcs[i].x,vector_npcs[i].y,vector_npcs[i].w,vector_npcs[i].h)){
                        //The npc dies!
                        handle_death();
                    }*/
                }
            }
        }

        //**********************************//
        // Check for collisions with doors: //
        //**********************************//
        handle_collision_doors();

        //************************************//
        // Check for collisions with springs: //
        //************************************//
        if(ethereal){
            solidity=TILE_SOLIDITY_PASSABLE;
        }
        else{
            solidity=TILE_SOLIDITY_SOLID;
        }
        handle_collision_springs(solidity);

        //*************************************//
        // Check for collisions with boosters: //
        //*************************************//
        handle_collision_boosters();

        //**********************************//
        // Check for collisions with traps: //
        //**********************************//
        if(ethereal){
            solidity=TILE_SOLIDITY_PASSABLE;
        }
        else{
            solidity=TILE_SOLIDITY_SOLID;
        }
        collision_data_trap box_trap=handle_collision_traps(solidity);
        if(box_trap.w!=-1.0){
            ///I need to decide what happens to NPCs that touch traps.
            handle_death();
        }

        //**********************************//
        // Check for collisions with tiles: //
        //**********************************//
        handle_tiles(check_x_start,check_x_end,check_y_start,check_y_end);

        //If the npc is underwater.
        if(underwater){
            //If the npc is out of oxygen, they have drowned.
            if(oxygen<=0){
                handle_death();
            }
        }

        if(exists){
            if(fabs(player.x-x)<PROCESS_RANGE && fabs(player.y-y)<PROCESS_RANGE){
                if(!CLIMBING && !player.DYING && !player.bubble_mode && !player.CLIMBING){
                    short solidity;
                    if(!deadly_to_touch && !deadly_while_special_attacking && !ethereal){
                        solidity=TILE_SOLIDITY_CLOUD;
                    }
                    else{
                        solidity=TILE_SOLIDITY_PASSABLE;
                    }
                    //Check for a collision with the player.
                    handle_collision_solid(player.x,player.y,player.w,player.h,solidity,NPC_END);

                    //If this npc is acting as a platform.
                    if(!player.SWIMMING && act_as_platform && !ethereal && !player.cheat_noclip){
                        if(push_actor_up(&player,player.x,player.w,being_pushed_up,type)){
                            player.handle_events(true);
                        }
                    }
                }
            }

            for(int i=0;i<mp_players.size();i++){
                if(fabs(mp_players[i].x-x)<PROCESS_RANGE && fabs(mp_players[i].y-y)<PROCESS_RANGE){
                    if(!CLIMBING && !mp_players[i].DYING && !mp_players[i].bubble_mode && !mp_players[i].CLIMBING){
                        short solidity;
                        if(!deadly_to_touch && !deadly_while_special_attacking && !ethereal){
                            solidity=TILE_SOLIDITY_CLOUD;
                        }
                        else{
                            solidity=TILE_SOLIDITY_PASSABLE;
                        }
                        //Check for a collision with the player.
                        handle_collision_solid(mp_players[i].x,mp_players[i].y,mp_players[i].w,mp_players[i].h,solidity,NPC_END);

                        //If this npc is acting as a platform.
                        if(!mp_players[i].SWIMMING && act_as_platform && !ethereal){
                            if(push_actor_up(&mp_players[i],mp_players[i].x,mp_players[i].w,being_pushed_up,type)){
                                mp_players[i].handle_events(true);
                            }
                        }
                    }
                }
            }
        }
    }

    finish_events();
}

void Npc::standard_death(){
    if(special_attack_in_progress){
        ai_end_special_attack();
    }

    bool stay_ethereal=false;

    if(ethereal){
        //The current tile location for the actor.
        int actor_current_x=(int)((int)x/TILE_SIZE);
        int actor_current_y=(int)((int)y/TILE_SIZE);

        //Check all tiles in a square around the actor.
        int check_x_start=actor_current_x-4;
        int check_x_end=actor_current_x+4;
        int check_y_start=actor_current_y-4;
        int check_y_end=actor_current_y+4;

        stay_ethereal=touching_solid_tile(check_x_start,check_x_end,check_y_start,check_y_end);
    }

    exists=false;

    ethereal=stay_ethereal;

    IN_AIR=true;

    if(NPC_SPRITE_COUNTS[type].dead_height>h){
        y-=NPC_SPRITE_COUNTS[type].dead_height-h;
    }
    else{
        y+=NPC_SPRITE_COUNTS[type].dead_height-h;
    }
    w=NPC_SPRITE_COUNTS[type].dead_width;
    h=NPC_SPRITE_COUNTS[type].dead_height;

    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        w/=2.0;
        h/=2.0;
    }*/
}

void Npc::handle_death(bool override_invulnerability){
    if(BOSS){
        //We do nothing if override_invulnerability is true, because bosses should ONLY die if truly killed by the player.
        if(!override_invulnerability){
            if(exists && !invulnerable){
                health--;

                if(health<=0){
                    standard_death();

                    int number_of_items=100+(player.new_game_plus*100);
                    if(player.get_upgrade_state("candy_drop")){
                        number_of_items*=2;
                    }
                    if(number_of_items>2000){
                        number_of_items=2000;
                    }

                    int score_bonus=100+ceil(pow(player.new_game_plus*10,1.5));

                    for(int i=0;i<number_of_items;i++){
                        vector_items.push_back(Item(x+(w-16)/2.0,y+(h-16)/2.0,true,ITEM_CANDY,0,false,1,100,1,175,true,15,score_bonus));
                    }

                    new_shots.push_back(new_shot_data(x+(w-128)/2.0,y+(h-128)/2.0,0.0,SHOT_BOSS_DEFEATED,false));

                    never_move=true;

                    //Set any associated traps to fall out of the level.
                    for(int i=0;i<vector_traps.size();i++){
                        if(vector_traps[i].BOSS){
                            vector_traps[i].movement_type=1;
                            vector_traps[i].moving=true;
                            vector_traps[i].movement_circle=false;
                            vector_traps[i].move_state=DOWN;
                            vector_traps[i].move_speed=10.0;
                            vector_traps[i].dangerous=false;
                        }
                    }

                    //Set any associated shots to dissipate.
                    for(int i=0;i<vector_shots.size();i++){
                        if(vector_shots[i].BOSS){
                            vector_shots[i].dissipate();
                        }
                    }

                    player.boss_end();

                    if(MAIN_BOSS){
                        player.bosses[player.current_level]=BOSS_STATE_DEFEATED;
                    }

                    player.stat_bosses_defeated++;

                    player.gain_score(SCORE_BEAT_BOSS,x+w/2.0,y);

                    if(MAIN_BOSS){
                        if(player.return_main_bosses_defeated()==4){
                            profile.update_achievements(ACHIEVEMENT_DEFEAT_ALL_BOSSES_BUT_FINAL);

                            //Unlock the final boss.
                            player.bosses[14]=BOSS_STATE_SPAWNED;

                            //Play story message.
                            window_message[0].set_message(player.boss_messages[14].message_title,player.boss_messages[14].message);
                        }
                        else if(player.return_main_bosses_defeated()==5){
                            profile.update_achievements(ACHIEVEMENT_DEFEAT_ALL_BOSSES);
                        }
                    }

                    play_positional_sound(sound_system.boss_die,x,y);

                    profile.save_profile_global_data();
                }
                else{
                    if(boss_phase==0 && health<=health_max*0.5){
                        boss_phase++;

                        if(type==NPC_BOSS_CASTLE){
                            default_max_speed*=2.5;

                            for(int i=0;i<vector_traps.size();i++){
                                if(vector_traps[i].BOSS){
                                    vector_traps[i].move_speed+=0.75;
                                }
                            }
                        }
                        else if(type==NPC_BOSS_HAUNTED_HOUSE){
                            default_max_speed*=2.0;
                            special_attack_frequency=15;
                        }
                        else if(type==NPC_BOSS_VOLCANO){
                            default_max_speed*=1.15;
                            jump_frequency=10;
                            jump_max=15.0;
                            gravity_max=15.0;
                            gravity=0.6;
                            special_attack_frequency=15;
                        }
                        else if(type==NPC_BOSS_PYRAMID){
                            default_max_speed*=1.66;
                            special_attack_frequency=15;

                            for(int i=0;i<vector_traps.size();i++){
                                if(vector_traps[i].BOSS){
                                    vector_traps[i].move_speed+=2.0;
                                }
                            }
                        }
                        else if(type==NPC_BOSS_LIGHTHOUSE){
                            default_max_speed*=3.0;
                            special_attack_frequency=30;
                        }

                        //Begin playing faster boss music.
                        music.play_track(MUSIC_BOSS_FAST);
                    }

                    play_positional_sound(sound_system.boss_hurt,x,y);
                }
            }
        }
    }
    else{
        //As long as the npc is not invulnerable.
        if(exists && (override_invulnerability || !invulnerable)){
            standard_death();

            if(!counts_as_trap){
                if(drops_items){
                    int random_type=random_range(0,99);
                    if(random_type<1){
                        int random=random_range(1,3);

                        for(int i=0;i<random;i++){
                            vector_items.push_back(Item(x+w/2.0,y-16,true,ITEM_AMMO,0,false,20,40,20,80));
                        }
                    }
                    else{
                        int random=0;
                        if(player.get_upgrade_state("candy_drop")){
                            random=random_range(6,12);
                        }
                        else{
                            random=random_range(1,3);
                        }

                        for(int i=0;i<random;i++){
                            vector_items.push_back(Item(x+w/2.0,y-16,true,ITEM_CANDY,0,false,20,40,20,80));
                        }
                    }
                }

                play_positional_sound(sound_system.npc_die,x,y);
            }
        }

        //If the npc still exists and is a replicator, it becomes deadly, if it wasn't already.
        if(exists && type==NPC_REPLICATOR){
            ///deadly_to_touch=true;
            angry=true;
        }
    }
}

void Npc::animate(){
    //If the npc is alive.
    if(exists){
        //Increment the frame counter.
        frame_counter++;
        frame_counter_swim++;
        frame_counter_fly++;
        frame_counter_jump++;
        frame_counter_special_attack++;

        //Handle standard walking animation.
        if(frame_counter>=frame_speed_walk){
            frame_counter=0;

            //Now increment the frame.
            frame++;

            if(has_footsteps){
                //If the actor is walking, play an occasional footstep sound.
                if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !DYING && run_speed!=0.0 && frame%footstep_sound_frequency==0){
                    ///play_footstep_sound();
                }
            }

            if(frame>NPC_WALK_SPRITES-1){
                frame=0;
            }
        }

        //The swimming animation increments at different rates depending on whether or not the npc is moving.
        short swim_frame_limit=0;
        if(move_state==0){
            swim_frame_limit=frame_speed_swim;
        }
        else{
            swim_frame_limit=frame_speed_swim/5.0;
        }
        //Handle swimming animation.
        if(frame_counter_swim>=swim_frame_limit){
            frame_counter_swim=0;

            //Now increment the frame.
            frame_swim++;

            if(frame_swim>NPC_SWIM_SPRITES-1){
                frame_swim=0;
            }
        }

        //The flying animation increments at different rates depending on whether or not the npc is moving.
        short fly_frame_limit=0;
        if(move_state==0){
            fly_frame_limit=frame_speed_fly;
        }
        else{
            fly_frame_limit=frame_speed_fly/5.0;
        }
        //Handle flying animation.
        if(frame_counter_fly>=fly_frame_limit){
            frame_counter_fly=0;

            //Now increment the frame.
            frame_fly++;

            if(frame_fly>NPC_FLY_SPRITES-1){
                frame_fly=0;
            }
        }

        //Handle jumping animation.
        if(frame_counter_jump>=20){
            frame_counter_jump=0;

            //Now increment the frame.
            frame_jump++;

            if(frame_jump>NPC_JUMP_SPRITES-1){
                frame_jump=0;
            }
        }

        //Handle special attack animation.
        if(frame_counter_special_attack>=frame_speed_special_attack && special_attack_in_progress){
            frame_counter_special_attack=0;

            //Now increment the frame.
            frame_special_attack++;

            //If this frame is the special attack sound frame.
            if(frame_special_attack==special_attack_sound_frame){
                //Play special attack sound.
                if(special_attack_type==SPECIAL_ATTACK_COW_MOO){
                    short random=random_range(0,99);
                    if(random<49){
                        play_positional_sound(sound_system.npc_cow_moo,x,y);
                    }
                    else{
                        play_positional_sound(sound_system.npc_cow_moo2,x,y);
                    }
                }
                else if(special_attack_type==SPECIAL_ATTACK_ZOMBIE_COW_MOO){
                    play_positional_sound(sound_system.npc_zombie_cow_moo,x,y);
                }
                else if(special_attack_type==SPECIAL_ATTACK_MUMMY_COW_MOO){
                    play_positional_sound(sound_system.npc_mummy_cow_moo,x,y);
                }
            }

            //If the animation is on its last frame.
            if(special_attack_type==SPECIAL_ATTACK_MONKEY_THROW_FRUIT && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a banana shot.

                //Determine the angle to the player.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_FRUIT].width;
                    spawn_y=y;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_FRUIT].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FRUIT,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_LASER_SPINNER_FIRE_LASER && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a laser shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_LASER].width;
                    spawn_y=y;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_LASER].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_LASER,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_BALL_KNIGHT_THROW_BALL && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a ball shot.

                //Determine the angle to the target.
                double angle=90;
                double spawn_x;
                double spawn_y;

                spawn_x=x;
                spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BALL].height;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_BALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_ARCHER_FIRE_ARROW && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create an arrow shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_ARROW].width;
                    spawn_y=y;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_ARROW].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_ARROW,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_SNOWMAN_THROW_SNOWBALL && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a snowball shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].width;
                    spawn_y=y;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_SNOWBALL].width;
                    spawn_y=y;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_SNOWBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_DRAGON_WHELP_SHOOT_FIREBALL && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a fireball shot.

                //Determine the angle to the target.
                double angle=270;
                double spawn_x;
                double spawn_y;

                spawn_x=x;
                spawn_y=y+h;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FIREBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_LEFT && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a fireball shot.

                //Determine the angle to the target.
                double angle;
                double spawn_x;
                double spawn_y;

                angle=180;

                spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_FIREBALL].width;
                spawn_y=y;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FIREBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_RIGHT && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a fireball shot.

                //Determine the angle to the target.
                double angle;
                double spawn_x;
                double spawn_y;

                angle=0;

                spawn_x=x+w;
                spawn_y=y;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FIREBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_UP && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a fireball shot.

                //Determine the angle to the target.
                double angle;
                double spawn_x;
                double spawn_y;

                angle=90;

                spawn_x=x;
                spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_FIREBALL].height;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FIREBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_FIREBALL_SHOOTER_SHOOT_DOWN && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a fireball shot.

                //Determine the angle to the target.
                double angle;
                double spawn_x;
                double spawn_y;

                angle=270;

                spawn_x=x;
                spawn_y=y+h;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_FIREBALL,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_SCORPION_SHOOT_POISON && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a poison shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_POISON].width;
                    spawn_y=y;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_POISON].width;
                    spawn_y=y;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_POISON,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_MECHSUIT_MOUSE_FIRE_MISSILE && frame_special_attack==NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a homing missile shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].height;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].width;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].height;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x-SHOT_SPRITE_COUNTS[SHOT_HOMING_MISSILE].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_HOMING_MISSILE,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_BOSS_HAUNTED_HOUSE && frame_special_attack>=NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a boss shot.

                //Determine the angle to the player.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w/2.0;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].height;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].width;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].height;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_HAUNTED_HOUSE].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w/2.0;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_BOSS_HAUNTED_HOUSE,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_BOSS_VOLCANO && frame_special_attack>=NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a boss shot.

                //Determine the angle to the player.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w/2.0;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].width;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].width;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w/2.0;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_VOLCANO].height;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_BOSS_VOLCANO,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_BOSS_PYRAMID && frame_special_attack>=NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a boss shot.

                //Determine the angle to the target.
                double angle;
                double spawn_x;
                double spawn_y;

                angle=270.0;

                spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_PYRAMID].width;
                spawn_y=y+h;

                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_BOSS_PYRAMID,counts_as_trap));
            }
            else if(special_attack_type==SPECIAL_ATTACK_BOSS_LIGHTHOUSE && frame_special_attack>=NPC_SPECIAL_ATTACK_SPRITES-1){
                //Create a boss shot.

                //Determine the angle to the target.
                double angle=ai_get_target_direction(special_attack_target_x,special_attack_target_y,PLAYER_W,PLAYER_H);
                double spawn_x;
                double spawn_y;
                if(angle>=0 && angle<90){
                    spawn_x=x+w/2.0;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].height;
                }
                else if(angle>=90 && angle<180){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].width;
                    spawn_y=y-SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].height;
                }
                else if(angle>=180 && angle<270){
                    spawn_x=x+w/2.0-SHOT_SPRITE_COUNTS[SHOT_BOSS_LIGHTHOUSE].width;
                    spawn_y=y+h;
                }
                else if(angle>=270 && angle<=360){
                    spawn_x=x+w/2.0;
                    spawn_y=y+h;
                }
                vector_shots.push_back(Shot(spawn_x,spawn_y,angle,SHOT_BOSS_LIGHTHOUSE,counts_as_trap));
            }

            if(has_footsteps){
                //If the actor is walking, play an occasional footstep sound.
                if(!SWIMMING && !IN_AIR && !SHOOTING && !CLIMBING && !DYING && run_speed!=0.0 && frame_special_attack%footstep_special_attack_sound_frequency==0){
                    ///play_footstep_sound();
                }
            }

            if(frame_special_attack>NPC_SPECIAL_ATTACK_SPRITES-1){
                frame_special_attack=0;
                if(special_attack_ends_with_animation){
                    ai_end_special_attack();
                }
            }
        }
    }
    //If the npc is dead.
    else{
        //Increment the frame counter.
        frame_counter_death++;

        //Handle death animation.
        if(frame_counter_death>=frame_speed_death){
            frame_counter_death=0;

            //Now increment the frame.
            frame_death++;

            if(frame_death>NPC_DEAD_SPRITES-1){
                frame_death=0;
            }
        }
    }
}

void Npc::render(bool mirrored){
    double render_x=x;
    double render_y=y;
    if(mirrored && touching_mirror(x+MIRROR_DISTANCE_X,y+MIRROR_DISTANCE_Y,w,h)){
        render_x+=MIRROR_DISTANCE_X;
        render_y+=MIRROR_DISTANCE_Y;
    }

    double scale_x=1.0;
    double scale_y=1.0;
    /**if(!friendly && !counts_as_trap && player.get_upgrade_state("tiny_baddies")){
        scale_x=0.5;
        scale_y=0.5;
    }*/

    double opacity=1.0;
    short npc_fade_time=player.get_npc_fade();
    if(npc_fade_time>0){
        short fade_threshold=(short)ceil((double)npc_fade_time*0.1);
        if(counter_fade<=fade_threshold){
            opacity=(double)counter_fade/(double)fade_threshold;
        }
    }

    if(!mirrored || (mirrored && !undead)){
        //If the npc is alive.
        if(exists){
            //If the npc is in camera bounds, render the npc.
            if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
                //If the npc is on the ground.
                if(!SWIMMING && !IN_AIR && !special_attack_in_progress && !FLYING && NPC_WALK_SPRITES>0){
                    if((move_state==0 || move_state==UP || move_state==DOWN) && facing==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_left[0],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if((move_state==0 || move_state==UP || move_state==DOWN) && facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_right[0],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_left[frame],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_right[frame],1.0,scale_x,scale_y,0.0,color);
                    }
                }

                //If the npc is performing its special attack.
                else if(special_attack_in_progress && NPC_SPECIAL_ATTACK_SPRITES>0){
                    if(facing==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_special_attack_left[frame_special_attack],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_special_attack_right[frame_special_attack],1.0,scale_x,scale_y,0.0,color);
                    }
                }

                //If the npc is flying.
                else if(FLYING && NPC_FLY_SPRITES>0){
                    if(move_state==0 && facing==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_fly_left[frame_fly],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==0 && facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_fly_right[frame_fly],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP || ((move_state==UP || move_state==DOWN) && facing==LEFT)){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_fly_left[frame_fly],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP || ((move_state==UP || move_state==DOWN) && facing==RIGHT)){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_fly_right[frame_fly],1.0,scale_x,scale_y,0.0,color);
                    }
                }

                //If the npc is swimming.
                else if(SWIMMING && NPC_SWIM_SPRITES>0){
                    if(move_state==0 && facing==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_swim_left[frame_swim],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==0 && facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_swim_right[frame_swim],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==LEFT || move_state==LEFT_DOWN || move_state==LEFT_UP || ((move_state==UP || move_state==DOWN) && facing==LEFT)){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_swim_left[frame_swim],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(move_state==RIGHT || move_state==RIGHT_DOWN || move_state==RIGHT_UP || ((move_state==UP || move_state==DOWN) && facing==RIGHT)){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_swim_right[frame_swim],1.0,scale_x,scale_y,0.0,color);
                    }
                }

                //If the npc is in the air.
                else if(IN_AIR && NPC_JUMP_SPRITES>0){
                    if(facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_jump_right[frame_jump],1.0,scale_x,scale_y,0.0,color);
                    }
                    else if(facing==LEFT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_jump_left[frame_jump],1.0,scale_x,scale_y,0.0,color);
                    }
                }

                /**if(special_attack_in_progress){
                    render_rectangle((int)(home_x-player.camera_x),(int)(home_y-player.camera_y),w,h,0.5,0.0,1.0,0.0);
                    render_rectangle((int)(x-player.camera_x),(int)(y-player.camera_y),w,h,0.5,COLOR_BLUE);
                    render_rectangle((int)(special_attack_target_x-player.camera_x),(int)(special_attack_target_y-player.camera_y),w,h,0.5,0.0,0.0,1.0);
                }*/
            }
        }
        //If the npc is dead.
        else{
            //If the npc is in camera bounds, render the npc.
            if(x>=player.camera_x-w && x<=player.camera_x+player.camera_w && y>=player.camera_y-h && y<=player.camera_y+player.camera_h){
                if(NPC_DEAD_SPRITES>0){
                    if(facing==RIGHT){
                        render_sprite((int)((int)render_x-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_dead[frame_death],opacity,scale_x,scale_y,0.0,color);
                    }
                    else if(facing==LEFT){
                        render_sprite((int)((int)render_x-(NPC_SPRITE_COUNTS[type].dead_width-NPC_SPRITE_COUNTS[type].width)-(int)player.camera_x),(int)((int)render_y-(int)player.camera_y),*sprite_sheet_image_npc,&vector_sprites_dead[frame_death],opacity,scale_x,scale_y,0.0,color,true);
                    }
                }
            }
        }
    }

    if(wears_tophat && (exists || (!exists && tophat_when_dead)) && !counts_as_trap && player.get_upgrade_state("tophats")){
        SDL_Rect temp;
        temp.x=0;
        temp.y=0;
        temp.w=image.tophat.w;
        temp.h=image.tophat.h;

        double hat_scale_x=w/45.0;
        double hat_scale_y=h/45.0;

        if(w>h){
            hat_scale_x=hat_scale_y;
        }
        else if(h>w){
            hat_scale_y=hat_scale_x;
        }

        hat_scale_x*=scale_x;
        hat_scale_y*=scale_y;

        double hat_x=render_x-(image.tophat.w*hat_scale_x)*0.33+tophat_adjust_x;
        double hat_y=render_y-(image.tophat.h*hat_scale_y)/2.0+tophat_adjust_y;

        bool flip_x=false;
        if(facing==LEFT){
            hat_x=render_x+w-(image.tophat.w*hat_scale_x)*0.66-tophat_adjust_x;

            flip_x=true;
        }

        render_sprite((int)((int)hat_x-(int)player.camera_x),(int)((int)hat_y-(int)player.camera_y),image.tophat,&temp,opacity,hat_scale_x,hat_scale_y,0.0,COLOR_WHITE,flip_x);
    }

    ///render_rectangle((int)(x-player.camera_x),(int)(y-player.camera_y),w,h,0.5,COLOR_RED);
    ///render_rectangle((int)(return_x()-player.camera_x),(int)(return_y()-player.camera_y),return_w(),return_h(),0.5,COLOR_RED);
}
