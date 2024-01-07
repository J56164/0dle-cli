#ifndef LEVEL_H_
#define LEVEL_H_

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

struct Effect {
    std::string name;
    int increment;
};

void from_json(const nlohmann::json& j, Effect& effect) {
    j.at("name").get_to(effect.name);
    j.at("increment").get_to(effect.increment);
}

struct Currency {
    std::string name;
    std::vector<Effect> effects;
};

void from_json(const nlohmann::json& j, Currency& currency) {
    j.at("name").get_to(currency.name);
    j.at("effects").get_to(currency.effects);
}

struct Init {
    std::string name;
    int amount;
};

void from_json(const nlohmann::json& j, Init& init) {
    j.at("name").get_to(init.name);
    j.at("amount").get_to(init.amount);
}

struct Requirement {
    std::string name;
    std::string type;
    int amount;
};

void from_json(const nlohmann::json& j, Requirement& requirement) {
    j.at("name").get_to(requirement.name);
    j.at("type").get_to(requirement.type);
    j.at("amount").get_to(requirement.amount);
}

struct Action {
    std::string name;
    std::vector<Effect> effects;
    std::vector<Requirement> requirements;
};

void from_json(const nlohmann::json& j, Action& action) {
    j.at("name").get_to(action.name);
    j.at("effects").get_to(action.effects);
    j.at("requirements").get_to(action.requirements);
}

struct Goal {
    std::string name;
    std::vector<Requirement> requirements;
    std::vector<Goal> subGoals;
};

void from_json(const nlohmann::json& j, Goal& goal) {
    j.at("name").get_to(goal.name);
    j.at("requirements").get_to(goal.requirements);
    j.at("subGoals").get_to(goal.subGoals);
}

struct Level {
    std::string name;
    std::string description;
    std::vector<Currency> currencies;
    std::vector<Init> inits;
    std::vector<Action> actions;
    std::vector<Goal> goals;
    int currencyCount;
    int actionCount;
    std::unordered_map<std::string, int> currencyNameToId;

    static Level fromFile(std::string& filename);
};

void from_json(const nlohmann::json& j, Level& level) {
    j.at("name").get_to(level.name);
    j.at("description").get_to(level.description);
    j.at("currencies").get_to(level.currencies);
    j.at("inits").get_to(level.inits);
    j.at("actions").get_to(level.actions);
    j.at("goals").get_to(level.goals);
    level.currencyCount = level.currencies.size();
    level.actionCount = level.actions.size();
    for (int id = 0; id < level.currencyCount; id++) {
        Currency& currency = level.currencies[id];
        level.currencyNameToId[currency.name] = id;
    }
}

Level Level::fromFile(std::string& filename) {
    std::ifstream file(filename);
    nlohmann::json j = nlohmann::json::parse(file);
    return j.get<Level>();
}

// int main() {
//     Level level = Level::fromFile("levels/tutorials/tutorial1.json");
// }

#endif