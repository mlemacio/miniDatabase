from typing import List, Tuple

actions: List[Tuple[str, str]] = [
    ("shut_down", "Shut down the system gracefully"),
    ("make_table", "Make a table"),
    ("show_table", "Show an existing table"),
    ("list_tables", "List all existing tables")
]

def to_camel_case(snake_str):
    components = snake_str.split('_')
    # We capitalize the first letter of each component except the first one
    # with the 'title' method and join them together.
    return components[0] + ''.join(x.title() for x in components[1:])

def generate_database_actions(actions: List[Tuple[str, str]]):
    database_actions = []
    for action in actions:
        assert(len(action) == 2)
        database_actions.append((to_camel_case(action[0]), action[0], action[1]))

    return database_actions

PWD = "cli/actions"

base_class_template = '''#ifndef CLI_DATABASE_ACTION_H
#define CLI_DATABASE_ACTION_H

#include <string>

enum class task_e
{{
{}
}};

class databaseAction_t
{{
public:
    virtual auto userOption() const -> std::string = 0;
    virtual auto taskType() const -> task_e = 0;
    virtual ~databaseAction_t(){{}};
}};

#endif // CLI_DATABASE_ACTION_H'''

derived_class_template = '''#ifndef CLI_ACTIONS_{action_sc_upper}_H
#define CLI_ACTIONS_{action_sc_upper}_H

#include "databaseAction.h"

class {action_cc}_t : public databaseAction_t
{{
public:
    std::string userOption() const override
    {{
        return "{user_option}";
    }}

    task_e taskType() const override 
    {{
        return task_e::{action_sc_upper};
    }}

    ~{action_cc}_t() {{}}
}};

#endif // CLI_ACTIONS_{action_sc_upper}_H'''

actions_list_template = '''#ifndef CLI_ACTIONS_ACTIONS_H
#define CLI_ACTIONS_ACTIONS_H

#include "shutdown.h"
#include "makeTable.h"

// We have to use a weird hack here BECAUSE this is a static const object
using possibleActions_t = std::vector<std::unique_ptr<databaseAction_t>>;
static const possibleActions_t possibleActions = []
{
    possibleActions_t ret;
    ret.emplace_back(std::make_unique<shutdown_t>());
    ret.emplace_back(std::make_unique<makeTable_t>());

    return ret;
}();

#endif // CLI_ACTIONS_SHUTDOWN_H'''

action_list_template = '''#ifndef CLI_ACTIONS_ACTIONS_H
#define CLI_ACTIONS_ACTIONS_H

{include_list}

// We have to use a weird hack here BECAUSE this is a static const object
using possibleActions_t = std::vector<std::unique_ptr<databaseAction_t>>;
static const possibleActions_t possibleActions = []
{{
    possibleActions_t ret;

{action_list}

    return ret;
}}();

#endif // CLI_ACTIONS_ACTIONS_H'''
