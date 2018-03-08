#ifndef CARDANUS_KEY_H
#define CARDANUS_KEY_H

#include <string>
#include "rotation_sequence.h"
#include "errors.hpp"

class Cardanus_Key {
private:
    const int           SIZE;
    bool**              m_grid;
    int                 m_origin_offset;

    void                swap                (int row_1, 
                                             int col_1,
                                             int row_2,
                                             int col_2);
    void                transpose           ();
    void                reflect_horizontal  ();
                        Cardanus_Key        ();
public:
    static const char   OPENED              = 'O';
    static const char   CLOSED              = '-';
    void                rotate              (Rotation_Sequence);
    void                rotate_cw           ();
    void                rotate_ccw          ();
    void                rotate_origin       ();
    void                open_at             (int row, int col);
    void                close_at            (int row, int col);
    void                close_all           ();
    int                 get_size            () const;
    bool                is_opened           (int row, int col) const;
    bool                is_valid            () const;
    std::string         to_string           (bool is_numbered = false) const;
                        Cardanus_Key        (int size);
                        Cardanus_Key        (const Cardanus_Key&);
    Cardanus_Key        operator=           (const Cardanus_Key&);
    friend Cardanus_Key operator^           (const Cardanus_Key&, const Cardanus_Key&);
    friend Cardanus_Key operator|           (const Cardanus_Key&, const Cardanus_Key&);
    friend bool         operator==          (const Cardanus_Key&, const Cardanus_Key&);
    friend bool         operator!=          (const Cardanus_Key&, const Cardanus_Key&);
};

/* ============================================================================ */
/*                                   Friends                                    */
/* ============================================================================ */

Cardanus_Key    operator^   (const Cardanus_Key&, const Cardanus_Key&) throw (Matrix_Dimension_Err);
Cardanus_Key    operator|   (const Cardanus_Key&, const Cardanus_Key&) throw (Matrix_Dimension_Err);
bool            operator==  (const Cardanus_Key&, const Cardanus_Key&);
bool            operator!=  (const Cardanus_Key&, const Cardanus_Key&);

#endif /* CARDANUS_KEY_H */