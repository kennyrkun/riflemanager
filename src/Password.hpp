#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include <string>

namespace password
{
    std::string generateHashedPassword(const std::string& username, const std::string& password);

    std::string getSavedPasswordHash(const std::string& username);

    bool validatePassword(const std::string& username, const std::string& password);
}

#endif // !PASSWORD_HPP