#include "engine/battle.h"
#include "engine/log.h"
#include "engine/pokemon.h"
#include "teambuilder/movebuilder.h"
#include "teambuilder/pokemonbuilder.h"

int main(int argc, char* argv[]) {
    engine::BattleAnnouncer::log("Running...");

    builder::PokemonSpeciesBuilder speciesBuilder;
    speciesBuilder.setName("Mew");
    speciesBuilder.setDexNo(151);
    speciesBuilder.setHp(100);
    speciesBuilder.setAttack(100);
    speciesBuilder.setDefense(100);
    speciesBuilder.setSpAttack(100);
    speciesBuilder.setSpDefense(100);
    speciesBuilder.setSpeed(100);
    speciesBuilder.setType(engine::Type::PSYCHIC);
    engine::PokemonSpecies mewSpecies = speciesBuilder.build();

    speciesBuilder.setName("MewTwo");
    speciesBuilder.setSpeed(150);
    engine::PokemonSpecies mewtwoSpecies = speciesBuilder.build();

    speciesBuilder.setName("Golem");
    speciesBuilder.setType(engine::Type::ROCK, engine::Type::GROUND);
    speciesBuilder.setSpeed(100);
    speciesBuilder.setAttack(150);
    engine::PokemonSpecies golemSpecies = speciesBuilder.build();

    engine::BattleAnnouncer::log("Built pokemon species");

    builder::PokemonBuilder pokemonBuilder;
    pokemonBuilder.setSpecies(&mewSpecies);
    engine::Move tackle = builder::getBasicMove();
    pokemonBuilder.setMove1(tackle);
    engine::Pokemon mew1 = pokemonBuilder.build();

    pokemonBuilder.setSpecies(&mewtwoSpecies);
    engine::Pokemon mew2 = pokemonBuilder.build();

    pokemonBuilder.setSpecies(&golemSpecies);
    engine::Pokemon golem = pokemonBuilder.build();

    engine::Team teamLeft = engine::Team();
    engine::Team teamRight = engine::Team();

    teamLeft.addPokemon(&mew1);
    teamLeft.addPokemon(&mew2);
    teamRight.addPokemon(&golem);

    engine::Battle battle = engine::Battle(&teamLeft, &teamRight);
    battle.start();

    return 0;
}
