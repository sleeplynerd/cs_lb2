#include "cardanus_grid.h"

using namespace std;
/* ============================================================================= */
/* ==================== Constr., destr., arithmetic operators ================== */
/* ============================================================================= */
Cardanus_Grid::Cardanus_Grid() : SIZE(-1) {}

Cardanus_Grid::Cardanus_Grid(int size) : SIZE(size) {
    m_grid = new char*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        m_grid[i] = new char[SIZE];
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            set_at(i,j,MOCK);
        }
    }
}

Cardanus_Grid::Cardanus_Grid(const Cardanus_Grid& grid) : SIZE(grid.SIZE) {
    m_grid = new char*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        m_grid[i] = new char[SIZE];
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            grid.get_at(i,j);
        }
    }
}

Cardanus_Grid Cardanus_Grid::operator=(const Cardanus_Grid& grid) {
    if (SIZE == grid.SIZE) {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                grid.get_at(i,j);
            }
        }
    }
    return *this;
}

/* ============================================================================= */
/* ================================ Public methods ============================= */
/* ============================================================================= */

void Cardanus_Grid::set_at(int row, int col, char symbol) {
    if (row < SIZE && col < SIZE) {
        m_grid[row][col] = symbol;
    }
}

char Cardanus_Grid::get_at(int row, int col) const {
    if (row < SIZE && col < SIZE) {
        return m_grid[row][col];
    } else {
        return MOCK;
    }
}

int Cardanus_Grid::get_size() const {
    return SIZE;
}


string Cardanus_Grid::to_string(bool is_numbered) const {
    const char  NEWLINE = '\n';
    const char  SPACE   = '\t';
    string      text;

    /* Header */
    if (is_numbered) {
        text.append(2, SPACE);
        for (int i = 0; i < 0; ++i) {
            text.append(std::to_string(i));
            text.push_back(NEWLINE);
        }
    }

    /* Table */
    for (int i = 0; i < SIZE; ++i) {
        /* Row number */
        if (is_numbered) {  
            text.append(std::to_string(i));
            text.push_back(SPACE);
        }
        /* Rest of the row */
        for (int j = 0; j < SIZE; ++j) {
            text.push_back(get_at(i,j));
            text.push_back(SPACE);
        }
        text.push_back(NEWLINE);
    }

    return text;
}