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
    AreaEnterSignal* area_signal;

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

class SignalCallable : public godot::CallableCustom
{
    virtual uint32_t hash() const
    {
        return godot::VariantHasher::hash(this->get_as_text());
    }

    virtual godot::String get_as_text() const
    {
        return "<SignalCallable>";
    }

    static bool compare_equal_func( const CallableCustom *inA, const CallableCustom *inB )
    {
        return inA == inB;
    }

    virtual CompareEqualFunc get_compare_equal_func() const
    {
        return &SignalCallable::compare_equal_func;
    }

    static bool compare_less_func( const CallableCustom *inA, const CallableCustom *inB )
    {
        return reinterpret_cast<const void *>( inA ) < reinterpret_cast<const void *>( inB );
    }

    virtual CompareLessFunc get_compare_less_func() const
    {
        return &SignalCallable::compare_less_func;
    }

    bool is_valid() const
    {
        return true;
    }

    virtual godot::ObjectID get_object() const
    {
        return godot::ObjectID();
    }
};

#define SIGNAL_CALLABLE_NAME(signal_name) \
    virtual godot::String get_as_text() const \
    { \
        return "<"#signal_name">"; \
    } \
// base class to create private c++ callables that dont expose to gd
class AreaEnterSignal : public SignalCallable
{
public:
    SIGNAL_CALLABLE_NAME(AreaEnterSignal);

    virtual void call( const godot::Variant **inArguments, int inArgcount,
                        godot::Variant &outReturnValue,
                        GDExtensionCallError &outCallError ) const
    {
        /*
        godot::UtilityFunctions::print("Number of arguments : ", inArgcount);
        for (int i=0; i < inArgcount; i++)
        {
            godot::UtilityFunctions::print("Argument ", i, " : ", inArguments[i]->get_type_name(inArguments[i]->get_type()));
        }*/
        godot::Object* obj = inArguments[0]->operator godot::Object *();
        if (obj->is_class("Coin"))
        {
            Coin* target = godot::Object::cast_to<Coin>(obj);
            CoinDashGame* game = godot::Object::cast_to<CoinDashGame>(godot::Engine::get_singleton()->get_singleton("CoinDashGame"));
            
            if (target->is_in_group("coins"))
            {
                godot::UtilityFunctions::print("Hit a coin !");
                target->Pickup();
                game->player->emit_signal("pickup");
            } else if (target->is_in_group("obstacles"))
            {
                godot::UtilityFunctions::print("Hit an obstacle !");
                game->player->emit_signal("hurt");
            }
        }
        outCallError.error = GDEXTENSION_CALL_OK;
    }
};

class StartGameSignal : public SignalCallable
{
SIGNAL_CALLABLE_NAME(StartGameSignal);
public:
    virtual void call( const godot::Variant **inArguments, int inArgcount,
                        godot::Variant &outReturnValue,
                        GDExtensionCallError &outCallError ) const
    {

    }
};

class HUDOnTimerTimeOutSignal : public SignalCallable
{
    SIGNAL_CALLABLE_NAME(HUDOnTimerTimeOutSignal);
    public:
    virtual void call( const godot::Variant **inArguments, int inArgcount,
                        godot::Variant &outReturnValue,
                        GDExtensionCallError &outCallError ) const
    {
        // hide messages
        godot::Object::cast_to<HUD>(godot::Engine::get_singleton()->get_singleton("HUD"))->get_node<godot::Label>("HUD/Message")->hide();
    }
};