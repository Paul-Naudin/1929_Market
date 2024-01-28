//
// Created by b4chh on 28/01/24.
//

#ifndef INC_1929_MARKET_ERROR_HPP
#define INC_1929_MARKET_ERROR_HPP
#include <string>

class Error : public std::exception {
public:
    explicit Error(const std::string  &message, const std::string &type = "Unknown");

    [[nodiscard]] std::string const &getType() const;
    [[nodiscard]] const char *what() const noexcept override;
    ~Error() override = default;
protected:
    std::string message;
    std::string type;
};


class ConnectionError : public Error {
public:
    explicit ConnectionError(std::string const &message, std::string const &type = "Unknown");
    ~ConnectionError() override = default;
};


#endif //INC_1929_MARKET_ERROR_HPP
