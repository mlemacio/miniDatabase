#ifndef CLI_COMMAND_LINE_INTERFACE_H
#define CLI_COMMAND_LINE_INTERFACE_H

#include <iostream>
#include <string_view>
#include <optional>
#include <memory>

#include "actions/actions.h"

class commandLineInterface_t
{
public:
    commandLineInterface_t(){};

    auto run() -> void
    {
        clearTerminal();
        printStartMessage();

        while (true)
        {
            auto responseNum = askForNumberedResponse("What action would you like to take:", possibleActions);
            auto &actionToTake = possibleActions[responseNum];

            submitTask(actionToTake->taskType());
        }
    }

private:
    // Prevent copying
    commandLineInterface_t(const commandLineInterface_t &) = delete;
    commandLineInterface_t &operator=(const commandLineInterface_t &) = delete;

    // Prevent moving
    commandLineInterface_t(commandLineInterface_t &&) = delete;
    commandLineInterface_t &operator=(commandLineInterface_t &&) = delete;

    auto submitTask(task_e t) -> void
    {
        switch (t)
        {
        case task_e::SHUT_DOWN:
            std::terminate();
        case task_e::MAKE_TABLE:
            return;
        case task_e::SHOW_TABLE:
            return;
        case task_e::LIST_TABLES:
            return;
        }
    }

    auto clearTerminal() -> void
    {
        system("clear");
    }

    auto printStartMessage() -> void
    {
        std::cout << "Hi! Welcome to a basic database table demo! \n"
                  << '\n';
    }

    auto askForNumberedResponse(const std::string_view &question, const possibleActions_t &options) -> size_t
    {
        while (true)
        {
            // Ask the question
            std::cout << question << '\n';

            // Print out all the options (Note: We 1-index)
            for (auto i = 0; i < options.size(); ++i)
            {
                auto &currOption = options[i];
                std::cout << (i + 1) << ": " << currOption->userOption() << '\n';
            }

            std::cout << '\n';

            auto maybeValidNumber = tryValidNumberFromUser(options.size());
            if (maybeValidNumber.has_value() == false)
            {
                continue;
            }

            clearTerminal();
            std::cout << "You chose: " << options[maybeValidNumber.value()]->userOption() << "\n\n";
            return maybeValidNumber.value();
        }

        __builtin_unreachable();
    }

    auto tryValidNumberFromUser(size_t numOptions) -> std::optional<size_t>
    {
        std::cout << "My response: ";

        // Get the numbered response
        auto maybeNumber = getNumberFromUser();
        if (maybeNumber.has_value() == false)
        {
            clearTerminal();
            std::cout << "Please type in a valid number\n"
                      << '\n';
            return std::nullopt;
        }

        // Remove the 1-index
        auto num = maybeNumber.value() - 1;
        if (num >= numOptions)
        {
            clearTerminal();
            std::cout << "That number is not a current option. Please try again\n"
                      << '\n';
            return std::nullopt;
        }

        return num;
    }

    auto getNumberFromUser() -> std::optional<size_t>
    {
        std::string input;
        std::cin >> input;

        try
        {
            return std::stoull(input);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
};

#endif // CLI_COMMAND_LINE_INTERFACE_H