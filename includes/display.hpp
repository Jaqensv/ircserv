#pragma once

#include <iostream>
#include <string>

struct Display
{
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
};

// Initialisation des membres statiques
const std::string Display::RESET = "\033[0m";
const std::string Display::RED = "\033[31m";
const std::string Display::GREEN = "\033[32m";
const std::string Display::YELLOW = "\033[33m";
const std::string Display::BLUE = "\033[34m";
