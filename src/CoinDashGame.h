#pragma once

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/global_constants.hpp"
#include "godot_cpp/classes/tile_map.hpp"
#include "godot_cpp/classes/tile_set.hpp"
#include "godot_cpp/classes/viewport.hpp"
#include "godot_cpp/core/binder_common.hpp"
#include "godot_cpp/variant/variant.hpp"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/marker2d.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/canvas_layer.hpp>

#define UNUSED( expr ) (void)( expr )

class AreaEnterSignal;

class Player : public godot::Area2D
{
    GDCLASS(Player, godot::Area2D)

public:
    /*Constructor & Destructor*/
    Player();
    ~Player() override;

    /*Getters & Setters*/
    void set_speed(float speed);
    float get_speed();

    /*Player Behaviour*/
    void Start();
    void Die();

    /*Default class functions*/
    static void _bind_methods();
    void _process(double delta) override;
    void _ready() override;
    
    /*Signals*/
    void area_entered_callback(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    /*Movement*/
    void ProcessMovement(double delta);
    void ProcessAnimations(double delta);

/*Members & Variables*/
public:
    bool is_ready = false;
    float player_speed  = 150.0f;
    godot::Vector2 velocity;
    godot::Vector2 screensize;

/*Node Components*/   
public:
    godot::AnimatedSprite2D* sprite;
};

class Coin : public godot::Area2D
{
    GDCLASS(Coin, godot::Area2D)

public:
    /*Constructor & Destructor*/
    Coin();
    ~Coin() override;

    /*Getters & Setters*/
    
    /*Coin Behaviour*/
    void Pickup();

    /*Default class functions*/
    static void _bind_methods();
    void _process(double delta) override;
    void _ready() override;
    
    /*Signals*/
    AreaEnterSignal* area_signal;

    

/*Members & Variables*/
public:
    bool is_ready = false;
    godot::Vector2 screensize = godot::Vector2();

/*Node Components*/   
public:
    godot::AnimatedSprite2D* sprite;
};
class CoinDashGame : public godot::Node
{
    GDCLASS(CoinDashGame, godot::Node)

/*Methods*/
public:
    /*Constructors*/
    CoinDashGame();
    ~CoinDashGame() override;

    /*Engine methods*/
    void _ready() override;
    static void _bind_methods();
    void _notification( int inWhat );
    void _process(double delta) override;

    /*Behaviour*/
    void SpawnCoins(); // spawn coins
    void NewGame(); // start a new game

/*Variables*/
public:
    bool is_ready = false;

    int play_time = 30;
    int level = 1;
    int score = 0;
    int time_left = 0;
    godot::Vector2 screensize = godot::Vector2();
    bool is_playing = false;
/*Components*/
public:
    godot::Ref<godot::PackedScene> coin_scene;
    godot::Timer* game_timer;
    Player* player;
};

class HUD : public godot::CanvasLayer
{
    GDCLASS(HUD, godot::CanvasLayer);
public:
    HUD();
    ~HUD();

     /*Engine methods*/
    void _ready() override;
    static void _bind_methods();
    void _notification( int inWhat );
    void _process(double delta) override;

    /*Behaviour*/
    void UpdateScore(int value);
    void UpdateTimer(int value);
    void ShowMessage(godot::String text);


/*Members*/
public:
    bool is_ready = false;
/*Components*/
public:
    godot::Timer* timer;
};



