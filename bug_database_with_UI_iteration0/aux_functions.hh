/*
BASICS

Auxiliary functions
*/

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <streambuf>
#include <string>
#include <vector>


#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


using std::cin;
using std::cout;
using std::ofstream;
using std::string;
using std::vector;





// True if a file is a directory
// Based on: http://forum.codecall.net/topic/68935-how-to-test-if-file-or-directory/
bool is_dir(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

// True if a file exists and is a file
// Based on: http://forum.codecall.net/topic/68935-how-to-test-if-file-or-directory/
bool is_file(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}


// base_path (string): Base path where the list starts, last character should be '/'
// final_string (string): Final string after which all output is appended

string read_directory(const string& base_path, string &final_string)
{
    DIR* dirp = opendir(base_path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {

        // Prints the complete name of the directory
        string file_name =  dp->d_name;

        string complete_path = base_path;
        complete_path.append(file_name);

        // Ignores current and previous directories
        if ((file_name == ".") || (file_name == "..")){
            continue;
        }

        // If the file is a directory, it is recursive
        string possible_directory_path = complete_path;
        possible_directory_path.append("/");

        // Adds to the string
        final_string.append(complete_path);
        final_string.append("\n");

        if (is_dir(possible_directory_path.c_str())){
            read_directory(possible_directory_path, final_string);
        }
    }
    closedir(dirp);
    return final_string;
}



// Same as above, but without checking subdirectories
string read_dir_no_subs(const string& base_path, string &final_string, bool show_hidden_files)
{
    DIR* dirp = opendir(base_path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {

        // Prints the complete name of the directory
        string file_name =  dp->d_name;

        string complete_path = base_path;
        complete_path.append(file_name);

        // Ignores current and previous directories
        if ((file_name == ".") || (file_name == "..")){
            continue;
        }

        if ( (! show_hidden_files) && (file_name.at(0) == '.')) {
            continue;
        }


        if (is_dir(complete_path.c_str())){
            final_string.append(complete_path);
            final_string.append("/\n");
        } else {
            // Adds to the string
            final_string.append(complete_path);
            final_string.append("\n");
        }

    }
    closedir(dirp);
    return final_string;
}



// Gets directories from another directory without checking subdirectories
string read_dirs_from_dir_no_subs(const string& base_path, string &final_string)
{
    DIR* dirp = opendir(base_path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {

        // Prints the complete name of the directory
        string file_name =  dp->d_name;

        string complete_path = base_path;
        complete_path.append(file_name);

        // Ignores current and previous directories
        if ((file_name == ".") || (file_name == "..")){
            continue;
        }

        if (is_dir(complete_path.c_str())){
            final_string.append(complete_path);
            final_string.append("/\n");
        } 
    }
    closedir(dirp);
    return final_string;
}



// Given a string and a token, it returns a vector of strings split by said token
// Obtained from: https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring
vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


// Replaces a substring within another
// Based on: https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
void ReplaceStringInPlace(string& subject, string& search,
                          string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}



// Reads a file into a string
string file_to_string(string filepath) {

    std::ifstream t(filepath.c_str());
    string output_str;

    t.seekg(0, std::ios::end);   
    output_str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    output_str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());

    // Removes tabs
    string tabs = "\t";
    string four_spaces = "    ";
    ReplaceStringInPlace(output_str, tabs, four_spaces);

    return output_str;
}



// Given a string, it prints it inside a box within terminal
// One space between borders
// The title is colored

void print_string_in_box(string title, string title_color, string text, int box_width) {

    string nocolor = "\033[0m"; 

    // Prints top
    cout << string(box_width, '-') << "\n";

    // Prints title
    cout << "| " << title_color << title << nocolor << string(box_width - title.length() - 2-2, ' ') << " |\n";
    cout << string(box_width, '-') << "\n";

    // Prints main text
    cout << text << "\n";
}



// Returns a string of 2 strings as a row format
// Both strings must be below (halfwdith + 2)
// Table width must be odd
string table_row(string s1, string s2, int table_width){

    int halfwidth = table_width/2 - 1;

    string output_row = "| ";
    output_row.append(s1);
    output_row.append(string(halfwidth - 1 - s1.length(), ' '));
    output_row.append("| ");
    output_row.append(s2);
    output_row.append(string(halfwidth - 1 - s2.length(), ' '));
    output_row.append("|");
    return output_row;
}



// Given a string obtained from reading a file, it divides the string into lines
// A line can have a maximum length, if it is over that, it is split into ones with smaller length
// The end result is a vector of strings
vector <string> file_into_lines(string filecontents, int chars_per_line) {

    vector <string> lines;
    vector <string> base_lines = split(filecontents, "\n");

    for (int item=0; item < base_lines.size(); item++) {

        if (base_lines[item].length() < chars_per_line) {
            lines.push_back(base_lines[item]);
        } else {

            // Divides the string into substrings, with as much as the maximum number of characters per line
            int full_length_lines = base_lines[item].length() / chars_per_line;

            for (int subline_n=0; subline_n < full_length_lines; subline_n++){
                lines.push_back(base_lines[item].substr(subline_n*chars_per_line, (subline_n+1)*chars_per_line ));
            }

            // Adds the last part of the line
            if ((base_lines[item].length() % chars_per_line) != 0) {
                lines.push_back(base_lines[item].substr(full_length_lines*chars_per_line, base_lines[item].length()));
            }
        }
    }

    return lines;
}







// Prints two files side by side
// Assumes 201 char width
void print_table_2str(string title_left, string title_right, string title_left_color, string title_right_color, string text_left, string text_right) {

    string nocolor = "\033[0m"; 

    // Prints top
    cout << string(201, '-') << "\n";
    cout << "| " << title_left_color << title_left << nocolor << string(100 - 2 - title_left.length(), ' ') << "| " <<
            title_right_color << title_right << nocolor << string(99 - 1 - title_right.length(), ' ')  << "|\n";


    cout << string(201, '-') << "\n";


    vector <string> formatted_text_left = file_into_lines(text_left, 97);
    vector <string> formatted_text_right = file_into_lines(text_right, 97);


    // Ensures that both vectors have the same length
    if (formatted_text_left.size() > formatted_text_right.size()) {
        for (int hh=0; hh < (formatted_text_left.size()-formatted_text_right.size()); hh++) {
            formatted_text_right.push_back("");
        }
    } else if (formatted_text_left.size() < formatted_text_right.size()) {
        for (int hh=0; hh < (formatted_text_right.size()-formatted_text_left.size()); hh++) {
            formatted_text_left.push_back("");
        }
    }

    // Prints the columns one by one
    for (int idid=0; idid < formatted_text_right.size(); idid++) {
        cout << table_row(formatted_text_left[idid], formatted_text_right[idid], 201) << "\n";
    }




    // Last line
    cout << string(201, '-') << "\n";

}




