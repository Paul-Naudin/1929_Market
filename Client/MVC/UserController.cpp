#include "UserController.h"
#include "FIXProtocol.hpp"

UserController::UserController(UserModel &model, UserView &view)
    : m_model(model), m_view(view)
{
}

UserController::~UserController()
{
}

void UserController::logon() const
{
    if (m_model.GetUsername().length() <= 0 || m_model.GetPassword().length() <= 0) {
        std::cout << "NO USERNAME OR NO PASSWORD" << std::endl;
        return;
    }

    FIXProtocol::Logon logon("CLIENT", "42", 0, "0", 30, m_model.GetUsername(), m_model.GetPassword());

    std::cout << logon.serialize() << std::endl;

}
