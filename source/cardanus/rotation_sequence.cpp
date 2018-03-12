#include "rotation_sequence.h"

using namespace std;

/* ============================================================================= */
/* ================ Constructors, destructors, arithm. operators. ===============*/
/* ============================================================================= */

Rotation_Sequence::Rotation_Sequence() {}

Rotation_Sequence::Rotation_Sequence(const Rotation_Sequence& seq) {
    m_sequence = seq.m_sequence;
    m_seq_buffer = seq.m_seq_buffer;
}

Rotation_Sequence Rotation_Sequence::operator=(const Rotation_Sequence& seq) {
    m_sequence = seq.m_sequence;
    m_seq_buffer = seq.m_seq_buffer;
    return *this;
}

/* ============================================================================= */
/* ============================== Public methods =============================== */
/* ============================================================================= */

Rotation_Sequence Rotation_Sequence::get_reverse(Rotation_Sequence sequence) {
    Dir                 dir;
    Rotation_Sequence   seq;
    
    for (auto it = sequence.m_sequence.rbegin(); it != sequence.m_sequence.rend(); ++it) {
        seq.rotate(*it);
    }

    seq.reset();
    return seq;
}

Rotation_Sequence& Rotation_Sequence::cw(int n_rotates) {
    for (int i = 0; i < n_rotates; ++i) {
        m_sequence.push_back(CW);
    }
    reset();
    return *this;
}

Rotation_Sequence& Rotation_Sequence::ccw(int n_rotates) {
    for (int i = 0; i < n_rotates; ++i) {
        m_sequence.push_back(CCW);
    }
    reset();
    return *this;
}

Rotation_Sequence& Rotation_Sequence::rotate(Dir dir, int n_rotates) {
    for (int i = 0; i < n_rotates; ++i){
        m_sequence.push_back(dir);
    }
    reset();
    return *this;
}

void Rotation_Sequence::clear() {
    m_sequence.clear();
    reset();
}

void Rotation_Sequence::reset() {
    m_seq_buffer.clear();
    m_seq_buffer = m_sequence;
}

Rotation_Sequence::Dir Rotation_Sequence::next() {
    if (m_seq_buffer.empty()) {
        return STOP;
    }
    Dir dir = m_seq_buffer.front();
    m_seq_buffer.pop_front();
    return dir;
}
