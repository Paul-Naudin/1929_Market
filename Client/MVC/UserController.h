#pragma once

#include "IController.h"
#include "UserModel.h"
#include "UserView.h"

class UserController : public IController
{
public:
    UserController(UserModel &model, UserView &view);
    ~UserController();

    void logon() const;

protected:
    UserModel &m_model;
    UserView &m_view;
};
