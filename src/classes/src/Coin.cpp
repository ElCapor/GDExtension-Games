#include "Coin.hpp"
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>
#include "SignalSystem.h"


/* Coins */
Coin::Coin()
{

}

Coin::~Coin()
{

}

void Coin::TweenProps()
{
    get_node<godot::CollisionShape2D>("CollisionShape2D")->set_deferred("disabled", true);
    auto tw = create_tween()->set_parallel()->set_trans(godot::Tween::TRANS_QUAD);
    tw->tween_property(this, "scale", get_scale()*3, 0.3);
    tw->tween_property(this, "modulate:a", 0.0, 0.3);
    SignalConnector<Coin> connector(this);
    connector.connect<godot::Tween>(tw, "finished", &Coin::TweenEnd);
}

void Coin::Pickup()
{
   TweenProps();
}
void Coin::_bind_methods()
{
}

void Coin::_process( double delta )
{
}

void Coin::_ready()
{
}

void Coin::TweenEnd( const godot::Variant **inArguments, int inArgcount,
                     godot::Variant &outReturnValue, GDExtensionCallError &outCallError )
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    this->queue_free();

    outCallError.error = GDEXTENSION_CALL_OK;
}
