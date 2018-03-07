#ifndef CIPHER_SET_H
#define CIPHER_SET_H

#include "rotation_sequence.h"
#include "cardanus_grid.h"
#include "cardanus_key.h"

struct Cipher_Set {
    Cardanus_Grid       grid;
    Cardanus_Key        key;
    Rotation_Sequence   sequence;
                        Cipher_Set(const Cardanus_Grid& cardanus_grid,
                                   const Cardanus_Key& cardanus_key,
                                   const Rotation_Sequence& rotation_sequence)
                                   : grid(cardanus_grid), 
                                   key(cardanus_key),
                                   sequence(rotation_sequence) {}
};

#endif /* Include guard CIPHER_SET_H */