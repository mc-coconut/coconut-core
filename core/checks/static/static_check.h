#pragma once
class StaticCheck {
public:
    virtual ~StaticCheck() = default;

    virtual int check();
};
