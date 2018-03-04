#include "decryptor.h"
#include "gtest/gtest.h"

using namespace std;

/** Grid:
 *  w e h v
 *  t a e b
 *  u p e e
 *  ! n s e
 * ----------
 * wehavebeensetup -> we have been setup
 * 
 * Key:
 *  O O O x
 *  x O x x
 *  x x x x
 *  x x x x
*/
static Cardanus_Grid        grid(4);
static Cardanus_Key         key(4);
static Rotation_Sequence    sequence;
static string               text_reference;

static void init() {
    text_reference = "wehavebeensetup!";

    grid.set_at(0,0,'w');
    grid.set_at(0,1,'e');
    grid.set_at(0,2,'h');
    grid.set_at(0,3,'v');
    grid.set_at(1,0,'t');
    grid.set_at(1,1,'a');
    grid.set_at(1,2,'e');
    grid.set_at(1,3,'b');
    grid.set_at(2,0,'u');
    grid.set_at(2,1,'p');
    grid.set_at(2,2,'e');
    grid.set_at(2,3,'e');
    grid.set_at(3,0,'!');
    grid.set_at(3,1,'n');
    grid.set_at(3,2,'s');
    grid.set_at(3,3,'e');

    key.open_at(0,0);
    key.open_at(0,1);
    key.open_at(0,2);
    key.open_at(1,1);

    sequence.clear();
    sequence.cw(4);
}

TEST(Decryptor, decrypt_GRID_KEY_SEQUENCE) {
    Decryptor decryptor;

    init();
    EXPECT_EQ(text_reference, decryptor.decrypt(grid, key, sequence));
}

TEST(Decryptor, decrypt_KEY_SEQUENCE) {
    Decryptor decryptor;

    init();
    decryptor.set_grid(grid);
    EXPECT_EQ(text_reference, decryptor.decrypt(key, sequence));
}

TEST(Decryptor, decrypt_SEQUENCE) {
    Decryptor decryptor;

    init();
    decryptor.set_grid(grid);
    decryptor.set_key(key);
    EXPECT_EQ(text_reference, decryptor.decrypt(sequence));
}

