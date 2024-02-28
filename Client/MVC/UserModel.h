#pragma once

#include "IModel.h"
#include <string>

class UserModel : public IModel
{
public:
    UserModel();
    ~UserModel();

    void SetData(int data) override;
    int GetData() const override;

    bool IsLoggedOn() const;
    void SetLoggedOn(bool loggedOn);

    void SetUsername(const std::string &username);
    std::string GetUsername() const;

    void SetPassword(const std::string &password);
    std::string GetPassword() const;

protected:
    int m_data;
    bool m_loggedOn;
    std::string m_username;
    std::string m_password;
};
