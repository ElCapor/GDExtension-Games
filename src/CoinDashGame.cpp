#include "CoinDashGame.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>

// even godot disable this warning in their engine
// about losing data when converting double to real_t
#pragma warning( disable : 4244)

/* Player */

Player::Player()
{
    velocity = godot::Vector2();
    screensize = godot::Vector2(480, 720);
    area_signal = memnew( SignalCallable );
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
   ADD_SIGNAL(godot::MethodInfo("pickup"));
   ADD_SIGNAL(godot::MethodInfo("hurt"));
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

void Player::Start()
{
    set_process(true);
    set_position(screensize/2);
    this->sprite->set_animation("idle");
}

void Player::Die()
{
    this->sprite->set_animation("hurt");
    set_process(false);
}

void Player::_ready()
{
    this->connect( "area_entered", area_signal );
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

/* Coins */
Coin::Coin()
{

}

Coin::~Coin()
{

}

void Coin::Pickup()
{
    queue_free();
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

/* The Game*/
CoinDashGame::CoinDashGame()
{
    godot::Engine::get_singleton()->register_singleton("CoinDashGame", this);
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

void CoinDashGame::_ready()
{
    if (!godot::Engine::get_singleton()->is_editor_hint())
    {
        this->screensize = get_viewport()->get_visible_rect().size;
        this->player = get_node<Player>("Player");
        this->game_timer = get_node<godot::Timer>("GameTimer");
        this->coin_scene = godot::ResourceLoader::get_singleton()->load("res://coin.tscn");
        if (this->player != nullptr && this->game_timer != nullptr && this->coin_scene.is_valid())
        {
            this->is_ready = true;
            this->player->hide();
            this->NewGame();
        }
        else {
            godot::UtilityFunctions::print("Failed to get player...");
        }
    }
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

void CoinDashGame::SpawnCoins()
{
    for (int i = 0 ; i < this->level +4 ; i++)
    {
        Coin* coin = Object::cast_to<Coin>(coin_scene->instantiate());
        this->add_child(coin);
        coin->screensize = screensize;
        coin->set_position(
            godot::Vector2(
                godot::UtilityFunctions::randi_range(0, screensize.x),
                godot::UtilityFunctions::randi_range(0, screensize.y)
            )
        );
    }
}
void CoinDashGame::NewGame()
{
    this->is_playing = true;
    this->level = 1;
    this->score = 0;
    this->time_left = this->play_time;
    this->player->Start();
    this->player->show();
    this->game_timer->start();
    this->SpawnCoins();
}
