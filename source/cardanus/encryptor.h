#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include <cmath>
#include "cipher_set.hpp"
#include <string>
#include <random>

typedef std::default_random_engine          Generator;
typedef std::uniform_int_distribution<int>  Distribution;

class Encryptor {
private:
    static const char   SPACE               = ' ';
   // mutable Generator   m_generator;

    Cardanus_Key        generate_key        (int key_size) const;
    int                 compute_grid_size   (const std::string&) const;
public:
    Cipher_Set          encrypt             (const std::string& text, 
                                             Rotation_Sequence seq = Rotation_Sequence().cw(4)) const;
};

#endif /* Include guard ENCRYPTOR_H */