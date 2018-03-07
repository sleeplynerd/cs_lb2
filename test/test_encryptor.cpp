#include "gtest/gtest.h"
#include <iostream>
#include "encryptor.h"
#include "decryptor.h"
#include <list>

using namespace std;

static const string REFERENCE("Ayethefootmenarefranticintheirindignition");

// TEST(Encryptor, generate_key) {
//     const int       KEY_SIZE                        = 7;
//     const int       N_OPENED_PER_SIDE               = KEY_SIZE * KEY_SIZE / 4;
//     int             n_opened                        = 0;
//     Encryptor       encryptor;
//     Cardanus_Key    key(encryptor.generate_key(7));
    
//     for (int i = 0; i < KEY_SIZE; ++i) {
//         for (int j = 0; j < KEY_SIZE; ++j) {
//             if (key.is_opened(i,j)) {
//                 n_opened++;
//             }
//         }
//     }

//     EXPECT_EQ(N_OPENED_PER_SIDE, n_opened);
// }

TEST(Encryptor, encrypt) {
    Encryptor   encryptor;
    Decryptor   decryptor;
    Cipher_Set  set(encryptor.encrypt(REFERENCE));
    string      decrypted = decryptor.decrypt(set);

    while (decrypted.back() == Cardanus_Grid::MOCK) {
        decrypted.pop_back();
    }

    cout << endl << set.grid.to_string() << endl;
    cout << set.key.to_string() << endl;

    //EXPECT_TRUE(set.key.is_valid());
    EXPECT_EQ(REFERENCE, decrypted);

    cout << endl << decryptor.decrypt(set) << endl;
}