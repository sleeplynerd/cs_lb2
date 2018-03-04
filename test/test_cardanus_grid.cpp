#include "cardanus_grid.h"
#include "gtest/gtest.h"
#include <string>
#include <iostream>

using namespace std;

TEST(Cardanus_Grid, set_at) { 
    Cardanus_Grid grid(10);
    grid.set_at(2,9,'c');
}

TEST(Cardanus_Grid, get_at) {
    Cardanus_Grid grid(10);
    grid.set_at(2,9,'c');
    EXPECT_EQ('c', grid.get_at(2,9));
}

TEST(Cardanus_Grid, get_size) {
    Cardanus_Grid grid(10);
    EXPECT_EQ(10, grid.get_size());
}

TEST(Cardanus_Grid, to_string) {
    string          text("abcdefghi");
    Cardanus_Grid   grid(3);

    grid.set_at(0,0,'a');
    grid.set_at(0,1,'b');
    grid.set_at(0,2,'c');
    grid.set_at(1,0,'d');
    grid.set_at(1,1,'e');
    grid.set_at(1,2,'f');
    grid.set_at(2,0,'g');
    grid.set_at(2,1,'h');
    grid.set_at(2,2,'i');

    cout << grid.to_string(true) << endl;
}