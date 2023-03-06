from typing import List, Tuple
import os

from constants import PWD, base_class_template, derived_class_template, action_list_template

def clear_actions_directory():
    os.system("rm -rf {}".format(PWD))
    os.system("mkdir {}".format(PWD))

def write_file(file_name: str, file_content):
    os.system("touch {}".format(file_name))
    with open(file_name, "w") as file:
        file.write(file_content)

def generate_base_action_header(database_actions: List[Tuple[str, str, str]]):
    # Do a little validation
    for action in database_actions:
        assert(len(action) == 3)

    enumListString = ""
    for action in database_actions:
        enumListString += "    {},\n".format(action[1].upper())
    enumListString = enumListString[:-1]

    file_name = "{}/databaseAction.h".format(PWD)
    file_content = base_class_template.format(enumListString)

    write_file(file_name, file_content)


def generate_derived_action_header(action_info: Tuple[str, str]):
    action_cc = action_info[0]
    action_sc_upper = action_info[1].upper()
    user_option = action_info[2]

    file_content = derived_class_template.format(action_cc=action_cc,
                                                 user_option=user_option,
                                                 action_sc_upper=action_sc_upper)

    file_name = "{}/{}.h".format(PWD, action_cc)
    write_file(file_name, file_content)

def generate_derived_action_headers(database_actions: List[Tuple[str, str, str]]):
    for action in database_actions:
        generate_derived_action_header(action)

def generate_action_list(database_actions: List[Tuple[str, str, str]]):
    include_list = ""
    action_list = ""

    for action_info in database_actions:
        include_list += "#include \"{}.h\"\n".format(action_info[0])
        action_list += "    ret.emplace_back(std::make_unique<{}_t>());\n".format(
            action_info[0])

    include_list = include_list[:-1]
    action_list = action_list[:-1]

    file_content = action_list_template.format(
        include_list=include_list, action_list=action_list)

    file_name = "{}/actions.h".format(PWD)
    write_file(file_name, file_content)
