#ifndef ROTATION_SEQUENCE_H
#define ROTATION_SEQUENCE_H

#include <list>

class Rotation_Sequence {
public:
    enum Dir {CW, CCW, STOP};
private:
    std::list<Dir>          m_sequence;
    std::list<Dir>  m_seq_buffer;
public:
    Rotation_Sequence&      cw                  (int n_rotates = 1);
    Rotation_Sequence&      ccw                 (int n_rotates = 1);
    void                    clear               ();
    void                    reset               ();
    Dir                     next                ();
							Rotation_Sequence   ();
                            Rotation_Sequence   (const Rotation_Sequence&);
    Rotation_Sequence         operator=         (const Rotation_Sequence&);
};

#endif /* ROTATION_SEQUENCE_H */