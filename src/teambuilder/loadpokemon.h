#pragma once

#include <memory>
#include <string>

class Pokemon;

std::unique_ptr<Pokemon*> loadPokemon(std::string name);
std::string getJsonEntry(std::string name);
