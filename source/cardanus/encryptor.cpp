#include "encryptor.h"

using namespace std;

/* ============================================================================ */
/*                               Private methods                                */
/* ============================================================================ */

int Encryptor::compute_grid_size(const string& text) const {
    int text_len = 0;
    float size;

    for (auto it = text.begin(); it != text.end(); ++it) {
        if (*it != SPACE) {
            text_len++;
        }
    }

    size = sqrt(text_len);
    if (size - (float)((int)size) == 0.0) {
        return (int)size;
    } else {
        return ((int)size + 1);
    }
}

Cardanus_Key Encryptor::generate_key(int key_size) const {
    const int       MIN_ROT = 1;
    const int       MAX_ROT = 4;
    int             row;
    int             col;
    int             col_max;
    int             rotation;
    Cardanus_Key    key(key_size);
    Generator       generator;

    for (row = 0; row < (key.get_size()/2); ++row) {
        col_max = (key.get_size() - row - 1);
        for (col = row; col < col_max; ++col) {
            Distribution rndrot(MIN_ROT, MAX_ROT);

            rotation = rndrot(generator);
            key.rotate(Rotation_Sequence().cw(rotation));
            key.open_at(row, col);
            key.rotate_origin();
        }
    }

    return key;
}

/* ============================================================================ */
/*                                Public methods                                */
/* ============================================================================ */

Cipher_Set Encryptor::encrypt(const string& text, Rotation_Sequence seq) const {
    int                     grid_size = compute_grid_size(text);
    Cardanus_Grid           grid(grid_size);
    Cardanus_Key            key(generate_key(grid_size));
    Rotation_Sequence::Dir  dir;
    string::const_iterator  it = text.cbegin();

    while (((dir = seq.next()) != Rotation_Sequence::STOP) && (it != text.cend())) {
        
        for (int row = 0; row < grid_size; ++row) {
            for (int col = 0; col < grid_size; ++col) {
                if (key.is_opened(row, col)) {
                    while (1) {
                        if (it == text.cend()) {
                            key.rotate_origin();
                            seq.reset();
                            return Cipher_Set(grid, key, seq);
                        } else if (*it != SPACE) {
                            break;
                        }
                        ++it;
                    }
                    grid.set_at(row, col, *it);
                    ++it;
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

    key.rotate_origin();
    seq.reset();
    return Cipher_Set(grid, key, seq);
}

