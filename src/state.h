#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <exception>
#include <unordered_map>
#include <vector>
#include "level.h"

class State {
public:
    Level& level;
    int turns = 0;
    std::vector<int> currencyAmounts;
    std::vector<int> pastActionIds;

    State(Level& level): level(level) {
        currencyAmounts = std::vector<int>(level.currencyCount);
        for (Init& init: level.inits) {
            int currency_id = level.currencyNameToId[init.name];
            currencyAmounts[currency_id] = init.amount;
        }
    }

    void calculateNextTurn(int action_id) {
        Action& action = level.actions[action_id];
        for (Requirement& requirement: action.requirements) {
            if (!isRequirementFulfilled(requirement)) {
                throw std::invalid_argument("Action's requirement not fulfilled");
            }
        }
        pastActionIds.push_back(action_id);
        turns++;

        // Calculate action effects
        tempCurrencyAmounts = currencyAmounts;
        for (Effect& effect: action.effects) {
            int affected_currency_id = level.currencyNameToId[effect.name];
            tempCurrencyAmounts[affected_currency_id] += effect.increment;
        }

        // Calculate currency effects
        currencyAmounts = tempCurrencyAmounts;
        for (int currency_id = 0; currency_id < level.currencyCount; currency_id++) {
            Currency& currency = level.currencies[currency_id];
            int currency_amount = tempCurrencyAmounts[currency_id];
            for (Effect& effect: currency.effects) {
                int affected_currency_id = level.currencyNameToId[effect.name];
                currencyAmounts[affected_currency_id] += effect.increment * currency_amount;
            }
        }
    }

    void calculatePreviousTurn() {
        if (turns == 0) {
            throw std::logic_error("Turns must be > 0 to calculate previous turn");
        }

        // Calculate currency effects
        tempCurrencyAmounts = currencyAmounts;
        for (int currency_id = 0; currency_id < level.currencyCount; currency_id++) {
            Currency& currency = level.currencies[currency_id];
            int currency_amount = currencyAmounts[currency_id];
            for (Effect& effect: currency.effects) {
                int affected_currency_id = level.currencyNameToId[effect.name];
                tempCurrencyAmounts[affected_currency_id] -= effect.increment * currency_amount;
            }
        }

        // Calculate action effects
        currencyAmounts = tempCurrencyAmounts;
        int action_id = pastActionIds[turns - 1];
        pastActionIds.pop_back();
        Action& action = level.actions[action_id];
        for (Effect& effect: action.effects) {
            int affected_currency_id = level.currencyNameToId[effect.name];
            currencyAmounts[affected_currency_id] -= effect.increment;
        }

        turns--;
    }

    void checkGoals() {
        for (Goal& goal: level.goals) {}
    }

    void checkGoal(Goal& goal) {
        for (Requirement& requirement: goal.requirements) {
            isRequirementFulfilled(requirement);
        }
        for (Goal& sub_goal: goal.subGoals) {
            checkGoal(sub_goal);
        }
    }

    bool isRequirementFulfilled(Requirement& requirement) {
        bool result;
        int currency_id = level.currencyNameToId[requirement.name];
        if (requirement.type == "greater-equal") {
            result = currencyAmounts[currency_id] >= requirement.amount;
        } else if (requirement.type == "greater") {
            result = currencyAmounts[currency_id] > requirement.amount;
        } else if (requirement.type == "less-equal") {
            result = currencyAmounts[currency_id] <= requirement.amount;
        } else if (requirement.type == "less") {
            result = currencyAmounts[currency_id] < requirement.amount;
        } else if (requirement.type == "equal") {
            result = currencyAmounts[currency_id] == requirement.amount;
        } else if (requirement.type == "not-equal") {
            result = currencyAmounts[currency_id] != requirement.amount;
        }

        return result;
    }

private:
    std::vector<int> tempCurrencyAmounts;
};

// int main() {
//     Level level = Level::fromFile("levels/tutorials/lv7.json");
//     State state(level);
//     for (int amount: state.currencyAmounts) {
//         std::cout << amount << " ";
//     }
//     std::cout << std::endl;

//     std::vector<int> action_ids = {1, 0, 0};
//     for (int action_id: action_ids) {
//         state.calculateNextTurn(action_id);
//         for (int amount: state.currencyAmounts) {
//             std::cout << amount << " ";
//         }
//         std::cout << std::endl;
//     }

//     for (int action_id: action_ids) {
//         state.calculatePreviousTurn();
//         for (int amount: state.currencyAmounts) {
//             std::cout << amount << " ";
//         }
//         std::cout << std::endl;
//     }
// }

#endif