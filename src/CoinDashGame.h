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

class Player : public godot::Area2D
{
    GDCLASS(Player, godot::Area2D)

public:

    Player();
    ~Player() override;

    static void _bind_methods();

    void set_speed(float speed);
    float get_speed();

    void _process(double delta) override;
    void _ready() override;

    void ProcessMovement(double delta);

    void ProcessAnimations(double delta);

public:
    bool is_ready = false;
    float player_speed  = 35.0f;
    godot::Vector2 velocity;
    godot::Vector2 screensize;

// game components    
public:
    godot::AnimatedSprite2D* sprite;
};

class CoinDashGame : public godot::Control
{
    GDCLASS(CoinDashGame, godot::Control)

public:

    CoinDashGame();
    ~CoinDashGame() override;

    static void _bind_methods();
    void _notification( int inWhat );
};