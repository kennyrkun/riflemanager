#ifndef PASSWORD_HPP
#define PASSWORD_HPP

#include "PicoSHA256.hpp"
#include "SettingsParser.hpp"

#include <string>
#include <ctime>

namespace password
{
    std::string generateHashedPassword(const std::string& username, const std::string& password)
    {
        std::time_t now = time(0);

        std::stringstream ss;
        ss << now;

        std::string hash_hex_str = picosha2::hash256_hex_string(password + ss.str());

        SettingsParser parser("./resources/admin/hashedpasswords");
        parser.set(username, hash_hex_str);
        parser.loadFromFile("./salts");
        parser.set(username, ss.str());

        return hash_hex_str;
    }

    std::string getSavedPasswordHash(const std::string& username)
    {
        SettingsParser parser("./resources/admin/hashedpasswords");

        std::string savedPasswordHash;
        parser.get(username, savedPasswordHash);

        return savedPasswordHash;
    }

    bool validatePassword(const std::string& username, const std::string& password)
    {
        std::string salt = "unable to find salt";
        SettingsParser parser("./resources/admin/salts");
        parser.get(username, salt);

        std::string providedPasswordHash = picosha2::hash256_hex_string(password + salt);
        std::string savedPasswordHash = getSavedPasswordHash(username);

        return providedPasswordHash == savedPasswordHash;
    }
}

#endif // !PASSWORD_HPP