#include "HUD.hpp"
#include "SignalSystem.h"
#include <godot_cpp/classes/button.hpp>

HUD::HUD()
{

}

HUD::~HUD()
{
}

void HUD::_ready()
{
    if (!godot::Engine::get_singleton()->is_editor_hint())
    {
        this->timer = this->get_node<godot::Timer>(("Timer"));

        if (timer != nullptr)
        {
            this->ConnectSignals();
            this->is_ready = true;
        }
        else {
            godot::UtilityFunctions::print("Failed to get timer from HUD");
        }
    }
}

void HUD::_bind_methods()
{
    ADD_SIGNAL(godot::MethodInfo("start_game"));
}

void HUD::ConnectSignals()
{
    SignalConnector<HUD> connector(this);
    connector.connect(get_node<godot::Button>("StartButton"), "pressed", &HUD::StartButtonPressed);
}

void HUD::_notification( int inWhat )
{
}

void HUD::_process( double delta )
{
}

void HUD::UpdateScore(int value)
{
    godot::Label* txt = get_node<godot::Label>("MarginContainer/Score");
    txt->set_text(godot::UtilityFunctions::str(value));
}

void HUD::UpdateTimer(int value)
{
    godot::Label* txt = get_node<godot::Label>("MarginContainer/Time");
    txt->set_text(godot::UtilityFunctions::str(value));
}

void HUD::ShowMessage( godot::String text )
{
    godot::Label* txt = get_node<godot::Label>("Message");
    txt->set_text(text);
    txt->show();
    timer->start();
}

void HUD::StartButtonPressed( const godot::Variant **inArguments, int inArgcount,
                        godot::Variant &outReturnValue, GDExtensionCallError &outCallError )
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    this->emit_signal("start_game");

    outCallError.error = GDEXTENSION_CALL_OK;
}

void HUD::StartGameEvent( const godot::Variant **inArguments, int inArgcount,
                          godot::Variant &outReturnValue, GDExtensionCallError &outCallError )
{
    UNUSED(inArguments);
    UNUSED(inArgcount);

    get_node<godot::Label>("Message")->hide();
    get_node<godot::Button>("StartButton")->hide();

    outCallError.error = GDEXTENSION_CALL_OK;
}
