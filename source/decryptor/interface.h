#ifndef INTERFACE_H
#define INTERFACE_H

#include <list>
#include <string>
#include <fstream>
#include "cardanus.h"
#include <exception>

class Interface {
private:
    typedef std::list<std::string> Command_List;

    static const int        N_ROTATES           = 4;
    static const char*      FULL_HELP;
    static const char*      OPEN_AT;
    static const char*      CLOSE_AT;
    static const char*      CLOSE_ALL;
    static const char*      ROTATE;
    static const char*      DECRYPT;
    static const char*      HELP;
    static const char*      LOAD;
    static const char*      SAVE;
    static const char*      SHOW_SCAN;
    static const char*      DECRYPT_WORD;
    static const char*      DECRYPT_SAFE;
    Cardanus_Grid*          mp_grid;
    Cardanus_Key*           mp_key;

    Command_List            split_args          (const std::string& cmdline) const;
    bool                    is_numeric          (const std::string&) const;
    bool                    is_safe_key         (const std::list<std::string>& words_to_save,
                                                 const std::string& word_to_find,
                                                 const Cardanus_Key& key) const;
    std::list<Cardanus_Key> get_keys_rotations  (const std::list<Cardanus_Key>&) const;
    std::string             get_path_to_save    (Command_List&) const;
    std::string             get_decrypted_text  (Cardanus_Grid, Cardanus_Key) const;
    std::string             open_at             (Command_List&);
    std::string             close_at            (Command_List&);
    std::string             close_all           ();
    std::string             rotate              (Command_List&);
    std::string             show_help           () const;
    std::string             load                (Command_List&);
    std::string             save                (Command_List&);
    std::string             decrypt             () const;
    std::string             decrypt_by_word     (Command_List&);
    std::string             decrypt_safe        (Command_List&);
    std::string             contextual_openclose(Command_List&);
    std::string             load_grid_from_file (const std::string& filepath);
    std::string             load_key_from_file  (const std::string& filepath);
    std::string             save_grid_to_file   (const std::string& filepath);
    std::string             save_key_to_file    (const std::string& filepath);
    std::string             save_to_file        (const std::string& filepath, const std::string& saving_string) const;
public:
    std::string             exec                (const std::string& cmdline);
                            Interface           ();
                            ~Interface          ();
};

#endif /* Include guard INTERFACE_H */