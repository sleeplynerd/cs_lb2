#include "decryptor.h"

using namespace std;

/* ============================================================================= */
/* =============================== Static members ============================== */
/* ============================================================================= */

const char* Decryptor::MOCK = "Err.";

/* ============================================================================= */
/* ========================== Constr., destr., arithm. ops. ==================== */
/* ============================================================================= */

Decryptor::Decryptor() : mp_grid(nullptr),
                         mp_key(nullptr) {}

Decryptor::Decryptor(const Decryptor& dec) {
    mp_grid = dec.mp_grid;
    mp_key = dec.mp_key;
}

Decryptor::~Decryptor() {
    unset_grid();
    unset_key();
}

Decryptor Decryptor::operator=(const Decryptor& dec) {
    unset_grid();
    unset_key();
    if (dec.mp_grid != nullptr) {
        mp_grid = new Cardanus_Grid(*(dec.mp_grid));
    }
    if (dec.mp_key != nullptr) {
        mp_key = new Cardanus_Key(*(dec.mp_key));
    }
    return *this;
}

/* ============================================================================= */
/* ================================ Private methods ============================ */
/* ============================================================================= */

void Decryptor::decr_word(const Cardanus_Grid& grid,
                          Cardanus_Key key,
                          string wordtail,
                          list<Cardanus_Key>& keylist) const 
{
    int     last_row = 0;
    int     last_col = 0;
    char    next_letter;

    if (wordtail.empty()) {
        keylist.push_back(key);
        return;
    } else {
        next_letter = wordtail.front();
        wordtail.erase(wordtail.begin());
    }

    for (int i = key.get_size() - 1; i > -1; --i) {
        for (int j = key.get_size() - 1; j > -1; --j) {
            if (key.is_opened(i,j)) {
                last_row = i;
                last_col = j+1;
                i = -1;
                break;
            }
        }
    }

    for (int row = last_row; row < key.get_size(); ++row) {
        for (int col = (row == last_row ? last_col : 0); col < key.get_size(); ++col) {
            if (grid.get_at(row,col) == next_letter) {
                Cardanus_Key newkey(key);
                newkey.open_at(row,col);
                decr_word(grid, newkey, wordtail, keylist);
            }
        }
    }
}

string Decryptor::decr(Cardanus_Grid grid,
                       Cardanus_Key key,
                       Rotation_Sequence sequence) const
{
    string                  text;
    Rotation_Sequence::Dir    dir;

    if (grid.get_size() != key.get_size()) {
        return MOCK;
    }

    while ((dir = sequence.next()) != Rotation_Sequence::STOP) {
        for (int row = 0; row < grid.get_size(); ++row) {
            for (int col = 0; col < grid.get_size(); ++col) {
                if (key.is_opened(row, col)) {
                    text.push_back(grid.get_at(row, col));
                }
            }
        }
        switch (dir) {
        case Rotation_Sequence::CW:
            key.rotate_cw();
            break;
        case Rotation_Sequence::CCW:
            key.rotate_ccw();
            break;
        }
    }

    return text;
}

/* ============================================================================= */
/* ================================ Public methods ============================= */
/* ============================================================================= */

list<Cardanus_Key> Decryptor::decrypt_by_word(const string& word) const {
    list<Cardanus_Key> listkey;

    if (mp_grid == nullptr) {
        return listkey;
    }

    decr_word(*mp_grid, Cardanus_Key(mp_grid->get_size()), word, listkey);
    return listkey;
}

list<Cardanus_Key> Decryptor::decrypt_by_word(const string& word, const Cardanus_Grid& grid) const {
    list<Cardanus_Key> listkey;
    decr_word(grid, Cardanus_Key(grid.get_size()), word, listkey);
    return listkey;
}

string Decryptor::decrypt(const Cipher_Set& ciphset) const {
    return decr(ciphset.grid, ciphset.key, ciphset.sequence);
}

string Decryptor::decrypt(const Cardanus_Grid& grid, 
                          const Cardanus_Key& key,
                          const Rotation_Sequence& seq) const 
{
    return decr(grid, key, seq);
}

string Decryptor::decrypt(const Cardanus_Key& key, const Rotation_Sequence& sequence) const {
    if (mp_grid != nullptr) {
        return decr(*mp_grid, key, sequence);
    }
    return MOCK;
}

string Decryptor::decrypt(const Rotation_Sequence& seq) const {
    if (mp_grid != nullptr && mp_key != nullptr) {
        return decr(*mp_grid, *mp_key, seq);
    }
    return MOCK;
}

void Decryptor::set_grid(const Cardanus_Grid& grid) {
    unset_grid();
    if (mp_key != nullptr) {
        if (mp_key->get_size() != grid.get_size()) {
            return;
        }
    }
    mp_grid = new Cardanus_Grid(grid);
}

void Decryptor::set_key(const Cardanus_Key& key) {
    unset_key();
    if (mp_grid != nullptr) {
        if (mp_grid->get_size() != key.get_size()) {
            return;
        }
    }
    mp_key = new Cardanus_Key(key);
}

void Decryptor::unset_grid() {
    if (mp_grid != nullptr) {
        delete mp_grid;
        mp_grid = nullptr;
    }
}

void Decryptor::unset_key() {
    if (mp_key != nullptr) {
        delete mp_key;
        mp_key = nullptr;
    }
}

