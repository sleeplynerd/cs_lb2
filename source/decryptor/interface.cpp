#include "interface.h"

using namespace std;

/* ============================================================================ */
/*                                Static members                                */
/* ============================================================================ */

const char* Interface::OPEN_AT      = "open_at";
const char* Interface::CLOSE_AT     = "close_at";
const char* Interface::CLOSE_ALL    = "close_all";
const char* Interface::ROTATE       = "rotate";
const char* Interface::DECRYPT      = "decrypt";
const char* Interface::HELP         = "help";
const char* Interface::LOAD         = "load";
const char* Interface::SAVE         = "save";

/* ============================================================================ */
/*                          Constructors, destructors                           */
/* ============================================================================ */

Interface::Interface() {
    mp_grid = nullptr;
    mp_key = nullptr;
}

Interface::~Interface() {
    if (mp_grid != nullptr) {
        delete mp_grid;
    }
    if (mp_key != nullptr) {
        delete mp_key;
    }
}

/* ============================================================================ */
/*                               Private methods                                */
/* ============================================================================ */

Interface::Command_List Interface::split_args(const string& cmdline) const {
    Command_List    cmdlist;
    string          buf;
    const char      DELIMETER = ' ';

    for (auto it = cmdline.cbegin(); it != cmdline.cend(); ++it) {
        switch (*it) {
        case DELIMETER:
            cmdlist.push_back(buf);
            buf.clear();
            break;
        default:
            buf.push_back(*it);
            break;
        }
    }

    if (!buf.empty()) {
        cmdlist.push_back(buf);
    }

    return cmdlist;
}

string Interface::open_at(Command_List& cmdlist) {
    string  arg_row;
    string  arg_col;
    int     row;
    int     col;

    if (mp_key == nullptr) {
        return "Err. The grid was not set";
    } else if (cmdlist.size() != 3) {
        return "Err. Invalid argument set.";
    }

    cmdlist.pop_front();
    arg_row = cmdlist.front();
    cmdlist.pop_front();
    arg_col =  cmdlist.front();

    try {
        row = stoi(arg_row);
        col = stoi(arg_col);
    } catch (const invalid_argument& ia) {
        return "Err. Probably command arguments are not numeric.";
    }

    mp_key->open_at(row, col);
    return mp_key->to_string();
}

string Interface::close_at(Command_List& cmdlist) {
    string  arg_row;
    string  arg_col;
    int     row;
    int     col;

    if (mp_key == nullptr) {
        return "Err. The grid was not set";
    } else if (cmdlist.size() != 3) {
        return "Err. Invalid argument set.";
    }

    cmdlist.pop_front();
    arg_row = cmdlist.front();
    cmdlist.pop_front();
    arg_col =  cmdlist.front();

    try {
        row = stoi(arg_row);
        col = stoi(arg_col);
    } catch (const invalid_argument& ia) {
        return "Err. Probably command arguments are not numeric.";
    }

    mp_key->close_at(row, col);
    return mp_key->to_string();
}

string Interface::close_all() {
    if (mp_key == nullptr) {
        return "Err. The grid was not set";
    }

    mp_key->close_all();
    return mp_key->to_string();
}

string Interface::rotate(Command_List& cmdlist) {
    if (mp_key == nullptr) {
        return "Err. The grid was not set";
    } else if (cmdlist.size() != 2) {
        return "Err. Invalid argument set";
    }

    cmdlist.pop_front();

    if (cmdlist.front() == ">") {
        mp_key->rotate_cw();
    } else if (cmdlist.front() == "<") {
        mp_key->rotate_ccw();
    } else {
        return "Err. Wrong direction";
    }

    return mp_key->to_string();
}

string Interface::decrypt() const {
    Decryptor   decryptor;
    string      result;

    result.append(mp_grid->to_string(true));
    result.append("\n");
    result.append(mp_key->to_string(true));
    result.append("\n");
    result.append("\n");
    result.append(decryptor.decrypt(*mp_grid, *mp_key, Rotation_Sequence().cw(4)));
    result.append("\n");
    result.append("\n");
    
    return result;
}

string Interface::show_help() const {
    const string help("- open_at \t {row}, \t {col}\n"
                      "- close_at \t {row} \t {col}\n"
                      "- close_all\n"
                      "- rotate {< | >}\n"
                      "- decrypt\n"
                      "- help\n"
                      "- load {file_grid}, [file_key]\n"
                      "- save {file_grid}, [file_key]\n");
    return help;
}

string Interface::load(Command_List& cmdlist) {
    string second;
    string third;
    string result;

    if (cmdlist.size() == 3) {
        cmdlist.pop_front();
        second = cmdlist.front();
        cmdlist.pop_front();
        third = cmdlist.front();
        try {
            result.append(load_grid_from_file(second));
            result.append("\n");
            result.append(load_key_from_file(third));
        } catch (...) {
            return "Loading failed";
        }
    } else if (cmdlist.size() == 2) {
        cmdlist.pop_front();
        second = cmdlist.front();
        try {
            result.append(load_grid_from_file(second));
        } catch (...) {
            return "Loading failed";
        }
    } else {
        return "Invalid argument set";
    }
    return result;
}

string Interface::save(Command_List& cmdlist) {
    string second;
    string third;
    string result;
    if (cmdlist.size() == 3) {
        cmdlist.pop_front();
        second = cmdlist.front();
        cmdlist.pop_front();
        third = cmdlist.front();
        try {
            result.append(save_grid_to_file(second));
            result.append("\n");
            result.append(save_key_to_file(third));
        } catch (...) {
            return "Loading failed";
        }
    } else if (cmdlist.size() == 2) {
        cmdlist.pop_front();
        second = cmdlist.front();
        try {
            result.append(save_grid_to_file(second));
        } catch (...) {
            return "Loading failed";
        }
    } else {
        return "Invalid argument set";
    }
    return result;
}

/* ============================================================================ */
/*                                Public methods                                */
/* ============================================================================ */

string Interface::load_grid_from_file(const string& path) {
    const char          SPACE = ' ';
    ifstream            infile(path);
    string              buf;
    string::iterator    it;
    int                 row = 0;
    int                 col = 0;

    if (mp_grid != nullptr) {
        delete mp_grid;
        delete mp_key;
        mp_grid = nullptr;
        mp_key = nullptr;
    }

    while (getline(infile, buf)) {
        /* Remove spaces */
        it = buf.begin();
        while (it != buf.end()) {
            if (*it == SPACE) {
                buf.erase(it);
                it = buf.begin();
            } else {
                ++it;
            }
        }
        /* Create grid and key if needed */
        if (mp_grid == nullptr) {
            mp_grid = new Cardanus_Grid(buf.size());
            mp_key = new Cardanus_Key(buf.size());
        }
        /* Fill grid */
        for (it = buf.begin(); it != buf.end(); ++it) {
            mp_grid->set_at(row, col, *it);
            ++col;
        }
        col = 0;
        row++;
    }

    return "Done!";
}

string Interface::load_key_from_file(const string& path) {
    const char          SPACE = ' ';
    ifstream            infile(path);
    string              buf;
    string::iterator    it;
    int                 row = 0;
    int                 col = 0;

    if (mp_grid == nullptr) {
        return "Err. The grid must be loaded first";
    }
    if (mp_key != nullptr) {
        delete mp_key;
        mp_key = nullptr;
    }

    while (getline(infile,buf)) {
        /* Remove spaces */
        it = buf.begin();
        while (it != buf.end()) {
            if (*it == SPACE) {
                buf.erase(it);
                it = buf.begin();
            } else {
                ++it;
            }
        }
        /* Create key */
        if (mp_key == nullptr) {
            if (mp_grid->get_size() != buf.size()) {
                return "Err. Grid and key dimensions must be agreed";
            }
            mp_key = new Cardanus_Key(buf.size());
        }
        /* Fill the key */
        for (it = buf.begin(); it != buf.end(); ++it) {
            if (*it == Cardanus_Key::OPENED) { 
                mp_key->open_at(row,col);
                col++;
            }
        }
        col = 0;
        row++;
    }

    return "Done!";
}

string Interface::save_grid_to_file(const string& path) {
    ofstream outfile(path);

    if (mp_grid == nullptr) {
        return "There is no grid in memory";
    } else {
        outfile << mp_grid->to_string(false);
    }

    return "Done!";
}

string Interface::save_key_to_file(const string& path) {
    ofstream outfile(path);

    if (mp_key == nullptr) {
        return "There is no key in memory";
    } else {
        outfile << mp_key->to_string(false);
    }

    return "Done!";
}

string Interface::exec(const string& cmdline) {
    Command_List    cmdlist = split_args(cmdline);
    string          command = cmdlist.front();
    try {
        if (command == OPEN_AT) {
            return open_at(cmdlist);
        } else if (command == CLOSE_AT) {
            return close_at(cmdlist);
        } else if (command == ROTATE) {
            return rotate(cmdlist);
        } else if (command == CLOSE_ALL) {
            return close_all();
        } else if (command == DECRYPT) {
            return decrypt();
        } else if (command == HELP) {
            return show_help();
        } else if (command == SAVE) {
            return save(cmdlist);
        } else if (command == LOAD) {
            return load(cmdlist);
        }
    } catch (...) {
        return "An error occured";
    }
    return "Undefined command";
}