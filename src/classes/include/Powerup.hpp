#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Include.hpp"
#include "Coin.hpp"

class Powerup : public godot::Area2D
{
    GDCLASS(Powerup, godot::Area2D);
public:
    /*Constructors*/
    Powerup();
    ~Powerup() override;

    /*Engine methods*/
    static void _bind_methods();
    void _process(double delta) override;
    void _ready() override;

    void Pickup();
    void TweenProps();
    void TweenEnd(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    
    godot::Vector2 screensize;

    /*Signals*/
    void OnTimerTimeOut(const godot::Variant** inArgs, int inArgCount, godot::Variant& returnValue, GDExtensionCallError& outError);
};

#endif