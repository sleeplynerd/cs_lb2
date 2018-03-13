#include "cardanus_key.h" 

using namespace std;

/* ============================================================================= */
/* ==================== Constr., destr., arithmetic methods ==================== */
/* ============================================================================= */

Cardanus_Key::Cardanus_Key() : SIZE(-1) {}

Cardanus_Key::Cardanus_Key(int size) : SIZE(size) {
    m_origin_offset = 0;
    m_grid = new bool*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        m_grid[i] = new bool[SIZE];
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            close_at(i,j);
        }
    }
}

Cardanus_Key::Cardanus_Key(const Cardanus_Key& key) : SIZE(key.SIZE) {
    m_origin_offset = key.m_origin_offset;
    m_grid = new bool*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        m_grid[i] = new bool[SIZE];
    }
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            m_grid[i][j] = key.m_grid[i][j];
        }
    }
}

Cardanus_Key Cardanus_Key::operator=(const Cardanus_Key& key) {
    if (get_size() == key.get_size()) {
        m_origin_offset = key.m_origin_offset;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                m_grid[i][j] = key.is_opened(i,j);
            }
        }
    } else {
        throw Matrix_Dimension_Err();
    }
    return *this;
}

/* ============================================================================= */
/* ============================== Private methods ============================== */
/* ============================================================================= */

void Cardanus_Key::swap(int row_1, int col_1, int row_2, int col_2) {
    bool buffer = m_grid[row_1][col_1];

    m_grid[row_1][col_1] = m_grid[row_2][col_2];
    m_grid[row_2][col_2] = buffer;
}

void Cardanus_Key::transpose() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < i; ++j) {
            swap(i, j, j, i);
        }
    }
}

void Cardanus_Key::reflect_horizontal() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < (SIZE/2); ++j) {
            swap(i, j, i, (SIZE - j -1));
        }
    }
}
/* ============================================================================= */
/* =============================== Public methods ============================== */
/* ============================================================================= */

void Cardanus_Key::rotate(Rotation_Sequence sequence) {
    Rotation_Sequence::Dir dir;

    while ((dir = sequence.next()) != Rotation_Sequence::STOP) {
        switch (dir) {
        case Rotation_Sequence::CCW:
            rotate_ccw();
            break;
        case Rotation_Sequence::CW:
            rotate_cw();
            break;
        }
    }
}

void Cardanus_Key::rotate_cw() {
    m_origin_offset++;
    transpose();
    reflect_horizontal();
}

void Cardanus_Key::rotate_ccw() {
    m_origin_offset--;
    reflect_horizontal();
    transpose();
}

void Cardanus_Key::rotate_origin() {
    m_origin_offset = m_origin_offset % 4;
    while (m_origin_offset != 0) {
        if (m_origin_offset > 0) {
            rotate_ccw();
        } else {
            rotate_cw();
        }
    }
}

void Cardanus_Key::open_at(int row, int col) {
    if (row < SIZE && col < SIZE) {
        m_grid[row][col] = true;
    }
}

void Cardanus_Key::close_at(int row, int col) {
    if (row < SIZE && col < SIZE) {
        m_grid[row][col] = false;
    }
}

void Cardanus_Key::close_all() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            close_at(i,j);
        }
    }
}

int Cardanus_Key::get_size() const {
    return SIZE;
}

bool Cardanus_Key::is_opened(int row, int col) const {
    if (row < SIZE && col < SIZE) {
        return m_grid[row][col];
    }
}

bool Cardanus_Key::is_valid() const {
    Cardanus_Key    reference(*this);
    Cardanus_Key    vellum_or(SIZE);
    Cardanus_Key    vellum_xor(SIZE);
    const int       N_ROTATES = 4;

    vellum_xor.close_all();
    vellum_or.close_all();
    for (int i = 0; i < N_ROTATES; ++i) {
        vellum_xor = vellum_xor ^ reference;
        vellum_or = vellum_or | reference;
        reference.rotate_cw();
        if (vellum_or != vellum_xor) {
            return false;
        }
    }

    return true;
}

string Cardanus_Key::to_string(bool is_numbered) const {
    const char  NEWLINE = '\n';
    const char  SPACE   = ' ';
    string      text;

    /* Header */
    if (is_numbered) {
        text.append(2, SPACE);
        for (int i = 0; i < SIZE; ++i) {
            text.append(std::to_string(i));
            text.push_back(SPACE);
        }
    }
    text.push_back(NEWLINE);

    /* Table */
    for (int i = 0; i < SIZE; ++i) {
        /* Row number */
        if (is_numbered) {
            text.append(std::to_string(i));
            text.push_back(SPACE);
        }
        /* Rest of the row */
        for (int j = 0; j < SIZE; ++j) {
            if (is_opened(i,j)) {
                text.push_back(OPENED);
            } else {
                text.push_back(CLOSED);
            }
            text.push_back(SPACE);
        }
        text.push_back(NEWLINE);
    }

    return text;
}

/* ============================================================================= */
/* =================================== Friends ================================= */
/* ============================================================================= */

Cardanus_Key operator^(const Cardanus_Key& lhs, const Cardanus_Key& rhs) throw (Matrix_Dimension_Err) {
    Cardanus_Key key_res(lhs.get_size());

    if (lhs.get_size() != rhs.get_size()) {
        throw Matrix_Dimension_Err();
    }

    for (int i = 0; i < lhs.get_size(); ++i) {
        for (int j = 0; j < lhs.get_size(); ++j) {
            key_res.m_grid[i][j] = lhs.m_grid[i][j] ^ rhs.m_grid[i][j];
        }
    }

    return key_res;
}

Cardanus_Key operator|(const Cardanus_Key& lhs, const Cardanus_Key& rhs) throw (Matrix_Dimension_Err) {
    Cardanus_Key key_res(lhs.get_size());

    if (lhs.get_size() != rhs.get_size()) {
        throw Matrix_Dimension_Err();
    }

    for (int i = 0; i < lhs.get_size(); ++i) {
        for (int j = 0; j < rhs.get_size(); ++j) {
            key_res.m_grid[i][j] = lhs.m_grid[i][j] | rhs.m_grid[i][j];
        }
    }

    return key_res;
}

Cardanus_Key operator&(const Cardanus_Key& lhs, const Cardanus_Key& rhs) throw (Matrix_Dimension_Err) {
    Cardanus_Key key_res(lhs.get_size());

    if (lhs.get_size() != rhs.get_size()) {
        throw Matrix_Dimension_Err();
    }

    for (int i = 0; i < lhs.get_size(); ++i) {
        for (int j = 0; j < rhs.get_size(); ++j) {
            key_res.m_grid[i][j] = lhs.m_grid[i][j] & rhs.m_grid[i][j];
        }
    }

    return key_res;
}

bool operator==(const Cardanus_Key& lhs, const Cardanus_Key& rhs) {
    if (lhs.get_size() != rhs.get_size()) {
        return false;
    }

    for (int i = 0; i < lhs.get_size(); ++i) {
        for (int j = 0; j < lhs.get_size(); ++j) {
            if (lhs.is_opened(i,j) != rhs.is_opened(i,j)) {
                return false;
            }
        }
    }

    return true;
}

bool operator!=(const Cardanus_Key& lhs, const Cardanus_Key& rhs) {
    return !(lhs == rhs);
}