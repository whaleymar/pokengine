#include "loadpokemon.h"
#include <fstream>
#include <iostream>

#define FILE_POKEMON "data/pokemon.json"

std::string getJsonEntry(std::string name) {
    // Pokemon pokemon;
    std::fstream pokedex(FILE_POKEMON);
    if (!pokedex.is_open()) {
        std::cerr << "Could not open file: " << FILE_POKEMON << std::endl;
    }
    std::string line;
    while (std::getline(pokedex, line)) {
    }
    return "";
}
