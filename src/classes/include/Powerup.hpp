#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Include.hpp"
#include "Coin.hpp"

class Pickup : public Coin
{
    GDCLASS(Pickup, Coin);
public:
    /*Constructors*/
    Pickup();
    ~Pickup() override;

    /*Engine methods*/
    static void _bind_methods();
    void _process(double delta) override;
    void _ready() override;
};

#endif