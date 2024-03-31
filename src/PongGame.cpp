#include "PongGame.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

// even godot disable this warning in their engine
// about losing data when converting double to real_t
#pragma warning( disable : 4244)

PongGame::PongGame()
{
    // prevent rendering from happening in editor
    if (!godot::Engine::get_singleton()->is_editor_hint())
        godot::UtilityFunctions::print("Started PongGame in Editor // reloaded");
    else
        godot::UtilityFunctions::print("Started PongGame");
}

PongGame::~PongGame()
{
    godot::UtilityFunctions::print("Closed PongGame");
}

void PongGame::_bind_methods()
{
    // i didnt add any specific method , but we could add some , if we wanted to interact with the game through
    // the main menu for example
}

void PongGame::_notification( int inWhat )
{
    //godot::UtilityFunctions::print( "Notification: ", godot::String::num( inWhat ) );
}

void PongGame::_ready()
{
    if (!godot::Engine::get_singleton()->is_editor_hint()) {

        godot::UtilityFunctions::print("I'm ready guys !");
        ball_spawn_pos = get_node<godot::Marker2D>("Ball_Spawn_Pos");
        body_ball = get_node<godot::CharacterBody2D>("Ball");
        body_player = get_node<godot::CharacterBody2D>("Paddle_Player");
        body_ai = get_node<godot::CharacterBody2D>("Paddle_AI");

        score_label = get_node<godot::Label>("Score_Label");

        ball_pos_ai_score = get_node<godot::Marker2D>("AI_Win_Position")->get_global_position().x;
        ball_pos_player_score = get_node<godot::Marker2D>("Player_Win_Position")->get_global_position().x;

        SpawnBall();
    }
}

void PongGame::SpawnBall()
{
    ball_speed = BALL_SPEED_GAME_START;
    body_ball->set_global_position(ball_spawn_pos->get_global_position());
    float ball_horizontal_direction = 1;
    if (godot::UtilityFunctions::randf() >= 0.5)
        ball_horizontal_direction = -1;
    
    ball_direction = godot::Vector2(ball_horizontal_direction, godot::UtilityFunctions::randf_range(-1, 1)).normalized();
}

void PongGame::UpdateScoreLabel()
{
    score_label->set_text(godot::UtilityFunctions::str(score_player, " | ", score_ai));
}

void PongGame::_physics_process(double delta)
{
    if (!godot::Engine::get_singleton()->is_editor_hint()) {
        ProcessBallMovement(delta);
        ProcessAiMovement(delta);
        ProcessPlayerMovement(delta);
    }
}

void PongGame::ProcessBallMovement(double delta)
{
    godot::Ref<godot::KinematicCollision2D> collision_info = body_ball->move_and_collide(ball_direction * ball_speed * delta);
    if (!collision_info.is_null() && collision_info->get_normal() != godot::Vector2())
    {
        if (collision_info->get_normal().x == 0)
            ball_direction.y = collision_info->get_normal().y;
        else if (collision_info->get_normal().y == 0)
        {
            if (collision_info->get_collider() == body_ai || collision_info->get_collider() == body_player)
            {
                ball_direction.x = collision_info->get_normal().x;
                ball_direction.y = godot::Vector2(collision_info->get_collider()->get("global_position")).direction_to(body_ball->get_global_position()).y;
                ball_direction = ball_direction.normalized();
                ball_speed += BALL_SPEED_PLAYER_BOUNCE_INCREASE;
            }
            else {
                ball_direction.x = collision_info->get_normal().x;
            }
        }else {
            ball_direction = collision_info->get_normal();
        }
    }
    if (body_ball->get_global_position().x < ball_pos_ai_score)
    {
        score_ai += 1;
        UpdateScoreLabel();
        SpawnBall();
    }
    else if (body_ball->get_global_position().x > ball_pos_player_score)
    {
        score_player += 1;
        UpdateScoreLabel();
        SpawnBall();
    }
    
}

void PongGame::ProcessAiMovement(double delta)
{
    godot::Vector2 ai_to_ball_dir = body_ai->get_global_position().direction_to(body_ball->get_global_position());
    ai_to_ball_dir.x = 0;
    if (ai_to_ball_dir.y > 0.4 || ai_to_ball_dir.y < 0.4)
		if (ai_to_ball_dir.y > 0)
			ai_to_ball_dir.y = 1;
		else
			ai_to_ball_dir.y = -1;
    body_ai->move_and_collide(ai_to_ball_dir * AI_SPEED * delta);
}

void PongGame::ProcessPlayerMovement(double delta)
{
    if (godot::Input::get_singleton()->is_action_pressed("ui_down"))
    {
        body_player->move_and_collide(godot::Vector2(0,1) * PLAYER_SPEED * delta);
    } else if (godot::Input::get_singleton()->is_action_pressed("ui_up"))
    {
        body_player->move_and_collide(godot::Vector2(0,-1) * PLAYER_SPEED * delta);
    }
    
}