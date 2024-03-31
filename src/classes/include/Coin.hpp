#ifndef COIN_HPP
#define COIN_HPP

#include "Include.hpp"

class Coin : public godot::Area2D
{
    GDCLASS(Coin, godot::Area2D)

public:
    /*Constructor & Destructor*/
    Coin();
    ~Coin() override;

    /*Getters & Setters*/
    
    /*Coin Behaviour*/
    void TweenProps(); // tween properties
    void Pickup();

    /*Default class functions*/
    static void _bind_methods();
    void _process(double delta) override;
    void _ready() override;
    
    /*Signals*/
    void TweenEnd(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);

    

/*Members & Variables*/
public:
    bool is_ready = false;
    godot::Vector2 screensize = godot::Vector2();

/*Node Components*/   
public:
    godot::AnimatedSprite2D* sprite;
};

#endif