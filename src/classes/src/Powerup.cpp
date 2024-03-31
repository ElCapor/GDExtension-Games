#include "Powerup.hpp"
#include "SignalSystem.h"
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>


Powerup::Powerup()
{
}

Powerup::~Powerup()
{
}

void Powerup::_bind_methods()
{
    
}

void Powerup::_process( double delta )
{
}

void Powerup::_ready()
{
    if (!godot::Engine::get_singleton()->is_editor_hint())
    {
        SignalConnector<Powerup> connector(this);
        connector.connect(get_node<godot::Timer>("Timer"), "timeout", &Powerup::OnTimerTimeOut);
    }
}

void Powerup::OnTimerTimeOut( const godot::Variant **inArgs, int inArgCount,
                              godot::Variant &returnValue, GDExtensionCallError &outError )
{
    UNUSED(inArgs);
    UNUSED(inArgCount);

    this->queue_free();
    
    outError.error = GDEXTENSION_CALL_OK;
}

void Powerup::TweenProps()
{
    get_node<godot::CollisionShape2D>("CollisionShape2D")->set_deferred("disabled", true);
    auto tw = create_tween()->set_parallel()->set_trans(godot::Tween::TRANS_QUAD);
    tw->tween_property(this, "scale", get_scale()*3, 0.3);
    tw->tween_property(this, "modulate:a", 0.0, 0.3);
    SignalConnector<Powerup> connector(this);
    connector.connect<godot::Tween>(tw, "finished", &Powerup::TweenEnd);
}

void Powerup::TweenEnd( const godot::Variant **inArguments, int inArgcount,
                        godot::Variant &outReturnValue, GDExtensionCallError &outCallError )
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    this->queue_free();

    outCallError.error = GDEXTENSION_CALL_OK;
}

void Powerup::Pickup()
{
   TweenProps();
}
