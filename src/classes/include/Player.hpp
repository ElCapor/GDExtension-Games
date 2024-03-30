#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Include.hpp"

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

#endif