#pragma once
#include "GameClient/Keyboard.h"

class AndroidKeyboard : public Keyboard
{
public:
    AndroidKeyboard() {}
    virtual ~AndroidKeyboard() {}

    virtual void init( void ) {}
    virtual void update( void ) {}
    virtual void reset( void ) {}
};
