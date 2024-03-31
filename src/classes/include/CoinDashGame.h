#ifndef COINDASHGAME_H
#define COINDASHGAME_H
#include "Include.hpp"
#include "Player.hpp"

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
    void ConnectSignals();
    void _notification( int inWhat );
    void _process(double delta) override;

    /*Behaviour*/
    void SpawnCoins(); // spawn coins
    void NewGame(); // start a new game
    void GameOver(); // the game is finished

    /*Signals*/
    void onGameTimerTimeout(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    void onPowerupTimerTimeout(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    void OnPlayerHurt(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    void OnPlayerPickup(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    void OnStartGame(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);


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
    godot::Ref<godot::PackedScene> powerup_scene;
    godot::Timer* game_timer;
    Player* player;
};

#endif