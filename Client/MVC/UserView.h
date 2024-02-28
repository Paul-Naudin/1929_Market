#pragma once

#include "IView.h"
#include "UserModel.h"

class UserView : public IView
{
public:
    UserView(UserModel &model);
    ~UserView();

protected:
    UserModel &m_model;
};