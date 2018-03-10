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
const char* Interface::SHOW_SCAN    = "show_scan";
const char* Interface::DECRYPT_WORD = "decrypt_word";

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

bool Interface::is_numeric(const string& word) const {
    string::const_iterator it = word.cbegin();
    while (it != word.cend() && isdigit(*it)) {
        ++it;
    }
    return (!word.empty() && it == word.cend());
}

string Interface::get_decrypted_text(Cardanus_Grid grid, Cardanus_Key key) const {
    string      decrypted   = Decryptor().decrypt(grid, key, Rotation_Sequence().cw(4));
    int         decr_size   = decrypted.size();
    const char  SPACE       = ' ';
    
    for (int i = 1; i < N_ROTATES; ++i) {
        decrypted.insert(decr_size / 4 * i + i - 1, 1, SPACE);
    }

    return decrypted;
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
    if (!(mp_key->is_valid())) {
        mp_key->close_at(row,col);
        return "Err. Cannot open. Opening of the given position invalidates the key.";
    }
    return "Done!";
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
    return "Done";
}

string Interface::close_all() {
    if (mp_key == nullptr) {
        return "Err. The grid was not set";
    }

    mp_key->close_all();
    return "Done!";
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

string Interface::show_help() const {
    const string help("- open_at        {row}       {col}\n"
                      "- close_at       {row}       {col}\n"
                      "- load           {file_grid} [file_key]\n"
                      "- save           {file_grid} [file_key]\n"
                      "- rotate         {< | >}\n"
                      "- decrypt_word   {known_word}\n"
                      "- {0-9*} {0-9*} [{0-9*} {0-9*}]*\n"
                      "- close_all\n"
                      "- decrypt\n"
                      "- help\n"
                      "- show_scan\n"
                      "- CTRL+C FOR EXIT\n");
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

string Interface::decrypt() const {
    const char  DELIMETER = ' ';
    const char  NEWLINE = '\n';
    const int   ROW_WIDTH = 1;
    const int   TABLES_GAP = 2;
    Decryptor   decryptor;
    bool        a_countdown[N_ROTATES*2] = {1,1,1,0,0,0,0,1};
    bool        f_v_from_null;
    bool        f_h_from_null;
    int         addon;
    int         target;
    int         base;
    int         decryption_size;
    string      process;
    string      decryption;

    if (mp_grid == nullptr || mp_key == nullptr) {
        return "Err. Grid must be set";
    }

    for (int rot = 0; rot < N_ROTATES; ++rot) {
        /* Determine a base */
        f_v_from_null = a_countdown[2*rot];
        f_h_from_null = a_countdown[2*rot+1];

        /* === Header === */
        process.append(ROW_WIDTH * 2, DELIMETER);
        for (int i = 0; i < mp_grid->get_size(); ++i) {
            process.append(to_string(i));
            process.append((2 * ROW_WIDTH - to_string(i).size()), DELIMETER);
        }
        process.append((TABLES_GAP+2) * ROW_WIDTH, DELIMETER);
        if (f_h_from_null) {
            base = 0;
            target = mp_grid->get_size();
            addon = 1;
        } else {
            base = mp_grid->get_size()-1;
            target = -1;
            addon = -1;
        }
        for (int i = base; i != target; i += addon) {
            process.append(to_string(i));
            process.append((2 * ROW_WIDTH - to_string(i).size()), DELIMETER);
        }
        process.push_back(NEWLINE);

        /* === Body === */
        if (f_v_from_null) {
            base = 0;
            target = mp_grid->get_size();
            addon = 1;
        } else {
            base = mp_grid->get_size() - 1;
            target = 0;
            addon = -1;
        }
        for (int i = 0; i < mp_grid->get_size(); ++i) {
            /* Cardanus grid */
            process.append(to_string(i));
            process.append((2 * ROW_WIDTH - to_string(i).size()), DELIMETER);
            for (int j = 0; j < mp_grid->get_size(); ++j) {
                process.append(1, mp_grid->get_at(i,j));
                process.append((2 * ROW_WIDTH - 1), DELIMETER);
            }
            /* Cardanus key */
            process.append((TABLES_GAP * ROW_WIDTH), DELIMETER);
            process.append(to_string(base));
            process.append((2 * ROW_WIDTH - to_string(base).size()), DELIMETER);
            for (int j = 0; j < mp_grid->get_size(); ++j) {
                if (mp_key->is_opened(i,j)) {
                    process.append(1, Cardanus_Key::OPENED);
                } else {
                    process.append(1, Cardanus_Key::CLOSED);
                }
                process.append((2 * ROW_WIDTH - 1), DELIMETER);
            }
            base += addon;
            process.push_back(NEWLINE);
        }
        process.push_back(NEWLINE);
        mp_key->rotate_cw();
    }
    process.push_back(NEWLINE);

    /* Decryption */
    decryption = get_decrypted_text(*mp_grid, *mp_key);
    

    process.append(decryption);
    process.push_back(NEWLINE);

    return process;
}

string Interface::decrypt_by_word(Command_List& cmdlist) {
    string              word;
    string              result;
    Decryptor           decryptor;
    list<Cardanus_Key>  list_crdkey;

    if (mp_grid == nullptr) {
        return "Err. Grid must be set";
    }

    cmdlist.pop_front();
    word = cmdlist.front();

    list_crdkey = decryptor.decrypt_by_word(word, *mp_grid);

    for (auto it = list_crdkey.begin(); it != list_crdkey.end(); ++it) {
        if (it->is_valid()) {
            result.append(it->to_string(true));
            result.append(get_decrypted_text(*mp_grid, *it));
            result.push_back('\n');
        }
    }

    return result;
}

string Interface::contextual_openclose(Command_List& cmdlist) {
    Command_List::const_iterator    it;
    int                             row;
    int                             col;
    int                             counter = 0;
    
    for (it = cmdlist.cbegin(); it != cmdlist.cend(); ++it) {
        if (!is_numeric(*it)) {
            return "All the input parameters must be pairs of unsigned integers";
        }
        counter++;
    }

    if (counter % 2 != 0) {
        return "All the input parameters must be pairs of unsigned integers";
    }

    if (mp_grid == nullptr) {
        return "Grid must be set";
    }

    it = cmdlist.cbegin();
    while (it != cmdlist.cend()) {
        row = stoi(*it);
        it++;
        col = stoi(*it);
        it++;
        if (mp_key->is_opened(row, col)) {
            mp_key->close_at(row,col);
        } else {
            mp_key->open_at(row,col);
        }
    }

    return "Done!";
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

    if (!infile.is_open()) {
        return "Unable to open file";
    }

    while (getline(infile, buf)) {
        if (buf.empty()) {
            continue;
        }
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

    if (!infile.is_open()) {
        return "Unable to open file";
    }

    while (getline(infile,buf)) {
        if (buf.empty()) {
            continue;
        }
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
            }
            col++;
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
        } else if (command == DECRYPT_WORD) {
            return decrypt_by_word(cmdlist);
        } else if (is_numeric(command)) {
            return contextual_openclose(cmdlist);
        }
    } catch (...) {
        return "An error occured";
    }
    return "Undefined command";
}