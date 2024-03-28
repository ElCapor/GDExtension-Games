#include "CoinDashGame.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

// even godot disable this warning in their engine
// about losing data when converting double to real_t
#pragma warning( disable : 4244)


Player::Player()
{
    velocity = godot::Vector2();
    screensize = godot::Vector2(480, 720);
}

Player::~Player()
{

}

void Player::set_speed(float speed)
{
    this->player_speed = speed;
}

float Player::get_speed()
{
    return this->player_speed;
}

void Player::_bind_methods()
{
    /*
    godot::ClassDB::bind_method(godot::D_METHOD("set_speed", "speed"), &Player::set_speed);
    godot::ClassDB::bind_method(godot::D_METHOD("get_speed"), &Player::get_speed);

    godot::ClassDB::add_property(get_class_static(), godot::PropertyInfo(godot::Variant::FLOAT, "speed"), "set_string", "get_string");
    */
}

void Player::ProcessMovement(double delta)
{
    this->velocity = godot::Input::get_singleton()->get_vector("ui_left", "ui_right", "ui_up", "ui_down");
    this->set_position(get_position() + this->velocity * this->player_speed * delta);

    this->set_position(
        godot::Vector2(
            godot::UtilityFunctions::clamp(this->get_position().x, 0, screensize.x),
            godot::UtilityFunctions::clamp(this->get_position().y, 0, screensize.y)
        )
    );
}

void Player::ProcessAnimations(double delta)
{
    if (this->velocity.length() > 0)
        this->sprite->set_animation("run");
    else
        this->sprite->set_animation("idle");

    if (this->velocity.x != 0)
        this->sprite->set_flip_h(velocity.x < 0);
    
};

void Player::_ready()
{
    if (!godot::Engine::get_singleton()->is_editor_hint())
    {
        this->sprite = get_node<godot::AnimatedSprite2D>("AnimatedSprite2D");

        if (this->sprite != nullptr)
        {
            godot::UtilityFunctions::print("Got sprite 2D !");
            this->is_ready = true;
        }
        else {
            godot::UtilityFunctions::print("Could not find sprite , aborting...");
        }
    }
}
void Player::_process(double delta)
{
    if (!godot::Engine::get_singleton()->is_editor_hint() && is_ready) {
        ProcessMovement(delta);
        ProcessAnimations(delta);
    }
}
CoinDashGame::CoinDashGame()
{
    // prevent rendering from happening in editor
    if (!godot::Engine::get_singleton()->is_editor_hint())
        godot::UtilityFunctions::print("Started Coin Dash in Editor // reloaded");
    else
        godot::UtilityFunctions::print("Started Coin Dash");
}

CoinDashGame::~CoinDashGame()
{
    godot::UtilityFunctions::print("Closed Coin Dash");
}

void CoinDashGame::_bind_methods()
{
    // i didnt add any specific method , but we could add some , if we wanted to interact with the game through
    // the main menu for example
}

void CoinDashGame::_notification( int inWhat )
{
    //godot::UtilityFunctions::print( "Notification: ", godot::String::num( inWhat ) );
}
