#include "UserModel.h"

UserModel::UserModel()
    : m_loggedOn(false)
{
}

UserModel::~UserModel()
{
}

void UserModel::SetData(int data)
{
    m_data = data;
}

int UserModel::GetData() const
{
    return m_data;
}

bool UserModel::IsLoggedOn() const
{
    return m_loggedOn;
}

void UserModel::SetLoggedOn(bool loggedOn)
{
    this->m_loggedOn = m_loggedOn;
}

std::string UserModel::GetUsername() const
{
    return m_username;
}

void UserModel::SetUsername(const std::string &username)
{
    this->m_username = username;
}

std::string UserModel::GetPassword() const
{
    return m_password;
}

void UserModel::SetPassword(const std::string &password)
{
    this->m_password = password;
}
