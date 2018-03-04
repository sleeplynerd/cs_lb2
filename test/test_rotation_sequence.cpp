#include "rotation_sequence.h"
#include "gtest/gtest.h"

using namespace std;

TEST(Rotation_Sequence, cw__ccw__next) {
    Rotation_Sequence seq;
    seq.cw(2).ccw(3);
    EXPECT_EQ(seq.next(), Rotation_Sequence::CW);
    EXPECT_EQ(seq.next(), Rotation_Sequence::CW);
    EXPECT_EQ(seq.next(), Rotation_Sequence::CCW);
    EXPECT_EQ(seq.next(), Rotation_Sequence::CCW);
    EXPECT_EQ(seq.next(), Rotation_Sequence::CCW);
    EXPECT_EQ(seq.next(), Rotation_Sequence::STOP);
}

TEST(Rotation_Sequence, clear) {
    Rotation_Sequence seq;
    seq.cw(2).ccw(3);
    seq.clear();
    EXPECT_EQ(seq.next(), Rotation_Sequence::STOP);
}

TEST(Rotation_Sequence, reset) {
    Rotation_Sequence seq;
    seq.cw().ccw();
    EXPECT_EQ(seq.next(), Rotation_Sequence::CW);
    seq.reset();
    EXPECT_EQ(seq.next(), Rotation_Sequence::CW);
}

TEST(Rotation_Sequence, OPERATOR_ASSIGN) {
    Rotation_Sequence seq1;
    Rotation_Sequence seq2;
    seq1.cw().ccw();
    seq1.next();
    seq2 = seq1;
    EXPECT_EQ(seq2.next(), Rotation_Sequence::CCW);
    seq2.reset();
    EXPECT_EQ(seq2.next(), Rotation_Sequence::CW);
}