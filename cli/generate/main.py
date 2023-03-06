from constants import actions, generate_database_actions
import generate_files as gf

if __name__ == "__main__":
    gf.clear_actions_directory()

    # Generate a little extra stuff to help us out
    database_actions = generate_database_actions(actions)

    gf.generate_base_action_header(database_actions)
    gf.generate_derived_action_headers(database_actions)
    gf.generate_action_list(database_actions)