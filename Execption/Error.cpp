//
// Created by b4chh on 28/01/24.
//

#include "Error.hpp"

Error::Error(const std::string  &message, const std::string &whoami) {
    this->message = message;
    this->type = whoami;
}

const char *Error::what() const noexcept {
    return this->message.c_str();
}

std::string const &Error::getType() const {
    return this->type;
}
