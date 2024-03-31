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

class PongGame : public godot::Control
{
    GDCLASS(PongGame, godot::Control)

public:

    PongGame();
    ~PongGame() override;

    static void _bind_methods();
    void _notification( int inWhat );

    void _ready() override;

    void SpawnBall();

    void UpdateScoreLabel();

    void _physics_process(double delta) override;

    void ProcessBallMovement(double delta);

    void ProcessAiMovement(double delta);

    void ProcessPlayerMovement(double delta);

// script variables
public:
    const float AI_SPEED = 280.0f;
    const float PLAYER_SPEED = 280.0f;
    float ball_speed = 300.0f;
    const float BALL_SPEED_GAME_START = 300.0f;
    const float BALL_SPEED_PLAYER_BOUNCE_INCREASE = 20.0f;

    godot::Vector2 ball_direction;
    godot::Marker2D* ball_spawn_pos;
    
    float ball_pos_player_score = 0;
    float ball_pos_ai_score = 0;

    godot::CharacterBody2D* body_ball = nullptr;
    godot::CharacterBody2D* body_player = nullptr;
    godot::CharacterBody2D* body_ai = nullptr;

    int score_player = 0;
    int score_ai = 0;
    godot::Label* score_label = nullptr;
};