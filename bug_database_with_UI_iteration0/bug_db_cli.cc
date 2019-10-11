/*
BASICS

Reads the bug database into and shows the user a CLI so that they can see bug information
*/


#include "aux_functions.hh"



int main(){



    // Terminal Colors, helpful for printing
    string REDRED = "\033[0;31m";
    string GREENGREEN = "\033[0;32m";
    string YELLOWYELLOW = "\033[1;33m";
    string BLUEBLUE = "\033[1;34m";
    string PURPLEPURPLE = "\033[1;35m";
    string NCNC = "\033[0m"; // No color

    // Gets the list of directories
    string bc_path = "Bug_Collection/";
    string empty_string = "";
    string newline_str = "\n";
    string bug_directories = read_dirs_from_dir_no_subs(bc_path, empty_string);
    // Removes the last character
    bug_directories.pop_back();

    vector <string> main_bug_categories = split(bug_directories, newline_str);


    // Prompts the user to select the appropriate option
    std::printf("Select the parallel implementation:\n");
    string only_show_program;

    for (int qq=0; qq < main_bug_categories.size(); qq++){

        only_show_program = main_bug_categories[qq];
        only_show_program.erase(0, bc_path.length());
        only_show_program.pop_back();

        std::printf("%d. %s\n", qq+1, only_show_program.c_str());
    }


    // User value must be accepted
    int user_program_selection;
    cout << "\nSelect program: ";
    cin >> user_program_selection;
    user_program_selection = user_program_selection-1;



    if ((user_program_selection >= main_bug_categories.size()) || (user_program_selection < 0)){
        cout << REDRED.c_str() << "Program is not accepted\n" << NCNC.c_str() << "\n";
        return 0;
    }



    // Shows the user the list of bugs
    string user_dir_program = main_bug_categories[user_program_selection];
    string empty2 = "";
    string bug_list = read_dir_no_subs(user_dir_program, empty2, false);
    bug_list.pop_back();

    vector <string> user_bug_vec = split(bug_list, newline_str);

    std::printf("Select the bug:\n");
    string only_show_bug;

    for (int qq=0; qq < user_bug_vec.size(); qq++){

        only_show_bug = user_bug_vec[qq];
        only_show_bug.erase(0, main_bug_categories[user_program_selection].length());
        only_show_bug.pop_back();
        std::printf("%d. %s\n", qq+1, only_show_bug.c_str());
    }


    // User value must be accepted
    int bug_number_selection;
    cout << "\nSelect bug: ";
    cin >> bug_number_selection;
    bug_number_selection = bug_number_selection-1;

    if ((bug_number_selection >= user_bug_vec.size()) || (bug_number_selection < 0)){
        cout << REDRED.c_str() << "Bug is not accepted\n" << NCNC.c_str() << "\n";
        return 0;
    }


    string bug_dir = user_bug_vec[bug_number_selection];

    string explanation_file = bug_dir;
    string buggy_file = "";
    string fixed_file = "";
    explanation_file.append("Explanation.txt");

    // Ensures the existance of all necessary files

    if (! is_file(explanation_file.c_str())) {
        cout << REDRED.c_str() << "Error in reading bug, explanation file is missing" << NCNC.c_str() << "\n\n";
    }


    // Buggy file is always named buggy_version.*
    string empty3 = "";
    string all_files_within_bug = read_dir_no_subs(bug_dir, empty3, false);
    all_files_within_bug.pop_back();
    vector <string> bug_files = split(all_files_within_bug, newline_str);

    string only_show_rel_path;

    for (int nvnv=0; nvnv < bug_files.size(); nvnv++){

        // Checks only the last chars
        only_show_rel_path = bug_files[nvnv];
        only_show_rel_path.erase(0, bug_dir.length());


        if (only_show_rel_path.find("buggy_version.") != string::npos ) {
            buggy_file = bug_files[nvnv]; 
            continue;
        }

        if (only_show_rel_path.find("fixed_version.") != string::npos ) {
            fixed_file = bug_files[nvnv]; 
            continue;
        }
    }

    if ((buggy_file == "") || (fixed_file == "")) {
        cout << REDRED << "Error in bug, buggy and/or fixed file do not exit" << NCNC << "\n";
    }

    string explanation = file_to_string(explanation_file);
    string buggy_code = file_to_string(buggy_file);
    string fixed_code = file_to_string(fixed_file);


    string explanation_title = "Explanation";
    string buggy_title = "Buggy version";
    string fixed_title = "Fixed version";

    // Erroneous and fixed versions
    //print_string_in_box(fixed_title, BLUEBLUE, fixed_code, 160);


    cout << "\n\n\n";
    print_table_2str(buggy_title, fixed_title, REDRED, BLUEBLUE, buggy_code, fixed_code);

    cout << "\n\n";

    // Prints the explanation below
    print_string_in_box(explanation_title, GREENGREEN, explanation, 160);

    return 0;
}

