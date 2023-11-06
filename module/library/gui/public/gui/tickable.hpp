#pragma once

class Tickable
{
public:
    virtual void handleTick(float delta) = 0;
};