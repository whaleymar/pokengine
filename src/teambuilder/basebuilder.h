#pragma once

template <typename T>
class Builder {
public:
    Builder() = default;
    ~Builder() = default;

    virtual T build() = 0;
    virtual void reset() = 0;
};
