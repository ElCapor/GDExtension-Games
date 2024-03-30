/*
USER INCLUDES
*/
#include "CoinDashGame.h"
#include "SignalSystem.h"
#include "Player.hpp"
#include "Coin.hpp"
#include "HUD.hpp"

/*
GODOT INCLUDES
*/
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_preloader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/tween.hpp>
#include <godot_cpp/classes/tweener.hpp>
#include <godot_cpp/classes/property_tweener.hpp>

/*Macros*/
#define dbg(...) godot::UtilityFunctions::print(__VA_ARGS__)




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
            this->ConnectSignals();
            this->player->hide();
            //this->NewGame();
        }
        else {
            godot::UtilityFunctions::print("Failed to get player...");
        }
    }
}

void CoinDashGame::_process(double delta)
{
    if (this->is_ready && !godot::Engine::get_singleton()->is_editor_hint())
    {
        if (is_playing && get_tree()->get_nodes_in_group("coins").size() == 0)
        {
            level += 1;
            time_left += 5;
            SpawnCoins();
        }
    }
}
void CoinDashGame::_bind_methods()
{
    // i didnt add any specific method , but we could add some , if we wanted to interact with the game through
    // the main menu for example
}

void CoinDashGame::ConnectSignals()
{
    SignalConnector<CoinDashGame> connector(this);
    connector.connect(get_node<Player>("Player"), "pickup", &CoinDashGame::OnPlayerPickup);
    connector.connect(get_node<Player>("Player"), "hurt", &CoinDashGame::OnPlayerHurt);
    connector.connect(get_node<godot::Timer>("GameTimer"), "timeout", &CoinDashGame::onGameTimerTimeout);
    connector.connect(get_node<HUD>("HUD"), "start_game", &CoinDashGame::OnStartGame);

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
    get_node<HUD>("HUD")->UpdateScore(score);
    get_node<HUD>("HUD")->UpdateTimer(time_left);

}

void CoinDashGame::GameOver()
{
    this->is_playing = false;
    this->game_timer->stop();
    get_tree()->call_group("coins", "queue_free");
    get_node<HUD>("HUD")->ShowMessage("Game Over");
    this->player->Die();

}

void CoinDashGame::onGameTimerTimeout(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError)
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    this->time_left -= 1;
    get_node<HUD>("HUD")->UpdateTimer(time_left);
    if (time_left < 0)
        this->GameOver();
        
    outCallError.error = GDEXTENSION_CALL_OK;
}

void CoinDashGame::OnPlayerHurt(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError)
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    GameOver();

    outCallError.error = GDEXTENSION_CALL_OK;
}

void CoinDashGame::OnPlayerPickup(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError)
{
    UNUSED(inArguments);
    UNUSED(inArgcount);
    
    score += 1;
    get_node<HUD>("HUD")->UpdateScore(score);

    outCallError.error = GDEXTENSION_CALL_OK;
}

void CoinDashGame::OnStartGame( const godot::Variant **inArguments, int inArgcount,
                                godot::Variant &outReturnValue, GDExtensionCallError &outCallError )
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    get_node<HUD>("HUD")->StartGameEvent(inArguments, inArgcount, outReturnValue, outCallError);
    this->NewGame();

    outCallError.error = GDEXTENSION_CALL_OK;
}


