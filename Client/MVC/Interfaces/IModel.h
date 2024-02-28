#pragma once

class IModel
{
public:
    virtual ~IModel() = default;
    virtual void SetData(int data) = 0;
    virtual int GetData() const = 0;
};
