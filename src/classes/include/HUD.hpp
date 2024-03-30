#ifndef HUD_HPP
#define HUD_HPP

#include "Include.hpp"


class HUD : public godot::CanvasLayer
{
    GDCLASS(HUD, godot::CanvasLayer);
public:
    HUD();
    ~HUD();

     /*Engine methods*/
    void _ready() override;
    static void _bind_methods();
    void ConnectSignals();
    void _notification( int inWhat );
    void _process(double delta) override;

    /*Behaviour*/
    void UpdateScore(int value);
    void UpdateTimer(int value);
    void ShowMessage(godot::String text);

    /*Signals*/
    void StartButtonPressed(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);
    void StartGameEvent(const godot::Variant** inArguments, int inArgcount, godot::Variant& outReturnValue, GDExtensionCallError& outCallError);

/*Members*/
public:
    bool is_ready = false;
/*Components*/
public:
    godot::Timer* timer;
};

#endif