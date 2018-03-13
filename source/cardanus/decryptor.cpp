#include "decryptor.h"

using namespace std;

/* ============================================================================= */
/* =============================== Static members ============================== */
/* ==========================================.=================================== */

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

/* ============================================================================ */
/*                               Private methods                                */
/* ============================================================================ */

list<Cardanus_Key> Decryptor::filter_keys(Cardanus_Grid grid, 
                                          const list<Cardanus_Key>& keys,
                                          const string& word,
                                          Rotation_Sequence::Dir dir) const 
{
    typedef Rotation_Sequence Seq;
    string              buf;
    list<Cardanus_Key>  result;

    for (list<Cardanus_Key>::const_iterator it = keys.cbegin(); it != keys.cend(); ++it) {
        buf = decrypt(grid, *it, Rotation_Sequence().rotate((dir == Seq::STOP ? Seq::CW : dir), (dir == Seq::STOP ? 1 : 4)));
        buf.append(buf);
        if (buf.find(word) != string::npos) {
            result.push_back(*it);
        }
    }

    return result;
}

bool Decryptor::is_safe_opening(Cardanus_Key current_key,
                                Rotation_Sequence::Dir dir,
                                int rots,
                                int chk_row,
                                int chk_col) const 
{
    Cardanus_Key            check(current_key.get_size());
    Rotation_Sequence       seq(Rotation_Sequence::get_reverse(Rotation_Sequence().rotate(dir,rots))); 
    
    seq.reset();
    while ((dir = seq.next()) != Rotation_Sequence::STOP) {
        check.open_at(chk_row, chk_col);
        current_key.rotate(Rotation_Sequence().rotate(dir,1));
        check.rotate(Rotation_Sequence().rotate(dir,1));

        for (int row = 0; row < current_key.get_size(); ++row) {
            for (int col = 0; col < current_key.get_size(); ++col) {
                if (check.is_opened(row, col)) {
                    row = current_key.get_size();
                    break;
                } else if (current_key.is_opened(row, col)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Decryptor::pick_key_by_word(std::string word,
                                 Cardanus_Grid grid,
                                 Cardanus_Key key,
                                 Rotation_Sequence::Dir dir,
                                 std::list<Cardanus_Key>& listkey,
                                 int last_row,
                                 int last_col,
                                 int pos,
                                 int rotations_made) const
{
    static const int            WORD_ROTS   = (word.size() / pow(key.get_size(), 2) / 4 + 2);
    static const int            N_ROTS      = (4 > WORD_ROTS ? WORD_ROTS : 4);
    static const Cardanus_Key   original(key);
    char                        next_letter;

    if (grid.get_size() != key.get_size()) {
        return;
    }

    if (pos == word.size()) {
        listkey.push_back(key);
        return;
    } else {
        next_letter = word[pos];
    }

    for (int rot = rotations_made; rot < (dir != Rotation_Sequence::STOP ? N_ROTS : 1); rot++) {
        for (int row = (rot == rotations_made ? last_row : 0); row < grid.get_size(); row++) {
            for (int col = (row == last_row && rot == rotations_made ? last_col : 0); col < grid.get_size(); ++col) {
                if (key.is_opened(row, col)) {
                    if (grid.get_at(row,col) == next_letter) {
                        pick_key_by_word(word, grid, key, dir, listkey, row, col+1, pos+1, rot);
                    } else {
                        //pick_key_by_word(word, grid, original, dir, listkey, row, col+1, 0, rot);
                        return;
                    }
                } else if (grid.get_at(row, col) == next_letter) {
                    Cardanus_Key newkey(key);
                    newkey.open_at(row,col);
                    if (newkey.is_valid() /* && (is_safe_opening(key, dir, rot, row, col) || rot == 0) */) {
                        pick_key_by_word(word, grid, newkey, dir, listkey, row, col+1, pos+1, rot);
                    }
                }
            }
        }
        /* Prevent non-needed offsets to a next side */
        if (pos == 0) {
            return;
        }
        key.rotate(Rotation_Sequence().rotate(dir));
    }
}

string Decryptor::decr(Cardanus_Grid grid,
                       Cardanus_Key key,
                       Rotation_Sequence sequence) const
{
    string                  text;
    Rotation_Sequence::Dir  dir;

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
        key.rotate(Rotation_Sequence().rotate(dir));
    }

    return text;
}

/* ============================================================================= */
/* ================================ Public methods ============================= */
/* ============================================================================= */

list<Cardanus_Key> Decryptor::pick_key_by_word(const string& word,
                                               const Rotation_Sequence::Dir dir) const
{
    if (mp_grid != nullptr) {
        return pick_key_by_word(word, *mp_grid, dir);
    } else {
        return list<Cardanus_Key>();
    }
}

list<Cardanus_Key> Decryptor::pick_key_by_word(const string& word,
                                               const Cardanus_Grid& grid,
                                               const Rotation_Sequence::Dir dir) const
{
    const int           N_ROTATES = 4;
    list<Cardanus_Key>  list_key;

    pick_key_by_word(word, grid, Cardanus_Key(grid.get_size()), dir, list_key);
    list_key = filter_keys(grid, list_key, word, dir);
    for (auto it = list_key.begin(); it != list_key.end(); ++it) {
        it->rotate_origin();
    }
    list_key.unique(); 
    return list_key;
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

