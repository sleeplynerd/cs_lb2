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

string Decryptor::decrypt(const Cipher_Set& ciphset) {
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

