#include "cardanus_key.h"
#include "gtest/gtest.h"
#include "rotation_sequence.h"
#include <string>
#include <iostream>


using namespace std;

TEST(Cardanus_Key, CONSTRUCTOR_INT) {
    Cardanus_Key key(12);
}

TEST(Cardanus_Key, COPY_CONSTRUCTOR) {
    Cardanus_Key key(12);
    Cardanus_Key copycat(key);
    EXPECT_EQ(copycat.get_size(), 12);
}

TEST(Cardanus_Key, OPERATOR_ASSIGN) {
    Cardanus_Key key_size10(10);
    Cardanus_Key key1_size4(4);
    Cardanus_Key key2_size4(4);

    EXPECT_NO_THROW({
        key1_size4 = key2_size4;
    });
    EXPECT_ANY_THROW({
        key_size10 = key1_size4;
    });

    key2_size4.open_at(2,2);
    key1_size4 = key2_size4;
    EXPECT_TRUE(key1_size4.is_opened(2,2));
}

TEST(Cardanus_Key, OPERATOR_XOR) {
    Cardanus_Key key1(4);
    Cardanus_Key key2(4);
    Cardanus_Key key3(10);

    key1.open_at(0,0);
    EXPECT_ANY_THROW({
        if ((key3 ^ key1).is_opened(1,1));
    });
    EXPECT_TRUE((key1 ^ key2).is_opened(0,0));
    EXPECT_FALSE((key1 ^ key2).is_opened(1,0));
}

TEST(Cardanus_Key, OPERATOR_OR) {
    Cardanus_Key key1(4);
    Cardanus_Key key2(4);
    Cardanus_Key key3(10);

    key1.open_at(1,1);
    EXPECT_ANY_THROW({
        if ((key3 | key1).is_opened(1,1));
    });
    EXPECT_TRUE((key1 | key2).is_opened(1,1));
    EXPECT_FALSE((key1 | key2).is_opened(0,0));
}

TEST(Cardanus_Key, OPERATOR_EQUAL) {
    Cardanus_Key key1(4);
    Cardanus_Key key2(4);
    Cardanus_Key key3(10);

    EXPECT_TRUE(key1 == key2);
    EXPECT_FALSE(key1 == key3);
}

TEST(Cardanus_Key, rotate) {
    Cardanus_Key key(4);
    
    key.open_at(0,0);
    key.rotate(Rotation_Sequence().cw(2));
    EXPECT_FALSE(key.is_opened(0,0));
    EXPECT_TRUE(key.is_opened(3,3));
}

TEST(Cardanus_Key, rotate_ccw) {
    Cardanus_Key key(4);
    
    key.open_at(0,0);
    key.rotate_ccw();
    key.rotate_ccw();
    EXPECT_FALSE(key.is_opened(0,0));
    EXPECT_TRUE(key.is_opened(3,3));
}

TEST(Cardanus_Key, rotate_cw) {
    Cardanus_Key key_rot(3);
    Cardanus_Key key_reference(3);

    key_rot.open_at(0,0);
    key_rot.open_at(0,1);
    key_rot.open_at(0,2);
    key_rot.open_at(1,2);
    key_rot.open_at(2,2);

    key_reference.open_at(0,2);
    key_reference.open_at(1,2);
    key_reference.open_at(2,2);
    key_reference.open_at(2,0);
    key_reference.open_at(2,1);

    key_rot.rotate_cw();
    EXPECT_TRUE(key_reference == key_rot);
}

TEST(Cardanus_Key, rotate_origin) {
    Cardanus_Key key(4);

    key.open_at(0,0);
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_cw();
    key.rotate_ccw();
    key.rotate_origin();

    EXPECT_TRUE(key.is_opened(0,0));
}

TEST(Cardanus_Key, open_at) {
    Cardanus_Key key(5);
    key.open_at(3,2);
}

TEST(Cardanus_Key, close_at) {
    Cardanus_Key key(4);
    key.open_at(2,1);
    EXPECT_TRUE(key.is_opened(2,1));
    key.close_at(2,1);
    EXPECT_FALSE(key.is_opened(2,1));
}

TEST(Cardanus_Key, close_all) {
    const int       SIZE = 8;
    Cardanus_Key    key(SIZE);

    for (int i = 1; i < SIZE; i+=2) {
        for (int j = 0; j < SIZE; ++j) {
            key.open_at(i,j);
        }
    }

    key.close_all();
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            EXPECT_FALSE(key.is_opened(i,j));
        }
    }
}

TEST(Cardanus_Key, get_size) {
    Cardanus_Key key1(1);
    Cardanus_Key key2(234);

    EXPECT_EQ(key1.get_size(), 1);
    EXPECT_EQ(key2.get_size(), 234);
}

TEST(Cardanus_Key, is_opened) {
    Cardanus_Key key(3);

    key.open_at(1,1);
    key.open_at(2,2);
    EXPECT_TRUE(key.is_opened(1,1));
    EXPECT_TRUE(key.is_opened(2,2));
    EXPECT_FALSE(key.is_opened(0,0));
}

TEST(Cardanus_Key, is_valid) {
    Cardanus_Key invalid3(3);
    Cardanus_Key invalid4(4);
    Cardanus_Key valid3(3);

    invalid3.open_at(0,1);
    invalid3.open_at(1,2);

    invalid4.open_at(0,0);
    invalid4.open_at(3,0);
    invalid4.open_at(3,3);

    valid3.open_at(0,0);
    valid3.open_at(1,2);

    EXPECT_FALSE(invalid3.is_valid());
    EXPECT_FALSE(invalid4.is_valid());
    EXPECT_TRUE(valid3.is_valid());
}

TEST(Cardanus_Key, to_string) {
    Cardanus_Key key(3);

    key.open_at(0,0);
    key.open_at(0,2);
    key.open_at(1,0);
    key.open_at(1,1);
    key.open_at(1,2);

    cout << endl << key.to_string(true) << endl;
}