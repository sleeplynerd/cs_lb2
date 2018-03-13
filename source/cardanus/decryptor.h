#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <string>
#include <list>
#include <cmath>
#include "cardanus_grid.h"
#include "cardanus_key.h"
#include "rotation_sequence.h"
#include "cipher_set.hpp"

class Decryptor {
private:
    static const char*      MOCK;
    Cardanus_Grid*          mp_grid;
    Cardanus_Key*           mp_key;
    
    std::list<Cardanus_Key> filter_keys     (Cardanus_Grid grid,
                                             const std::list<Cardanus_Key>& keys,
                                             const std::string& word,
                                             Rotation_Sequence::Dir dir) const;
    bool                    is_safe_opening (Cardanus_Key current_key, 
                                             Rotation_Sequence::Dir current_dir,
                                             int rotations_made,
                                             int row,
                                             int col) const;
    void                    pick_key_by_word(std::string word,
                                             Cardanus_Grid grid,
                                             Cardanus_Key key,
                                             Rotation_Sequence::Dir dir,
                                             std::list<Cardanus_Key>& listkey,
                                             int last_row = 0,
                                             int last_col = 0,
                                             int pos = 0,
                                             int rotations_made = 0) const;
    std::string             decr            (Cardanus_Grid,
                                             Cardanus_Key,
                                             Rotation_Sequence) const;
public:     
    std::list<Cardanus_Key> pick_key_by_word(const std::string& word,
                                             const Rotation_Sequence::Dir dir = Rotation_Sequence::STOP) const;
    std::list<Cardanus_Key> pick_key_by_word(const std::string& word,
                                             const Cardanus_Grid& grid,
                                             const Rotation_Sequence::Dir dir = Rotation_Sequence::STOP) const;
    std::string             decrypt         (const Cipher_Set&) const;
    std::string             decrypt         (const Cardanus_Grid&,
                                             const Cardanus_Key&,
                                             const Rotation_Sequence&) const;
    std::string             decrypt         (const Cardanus_Key&, const Rotation_Sequence&) const;
    std::string             decrypt         (const Rotation_Sequence&) const;
    void                    set_grid        (const Cardanus_Grid& grid);
    void                    set_key         (const Cardanus_Key& key);
    void                    unset_grid      ();
    void                    unset_key       ();
                            Decryptor       ();
                            Decryptor       (const Decryptor& decryptor);
    virtual                 ~Decryptor      ();    
	Decryptor               operator=       (const Decryptor& decryptor);
};

#endif /* DECRYPTOR_H */