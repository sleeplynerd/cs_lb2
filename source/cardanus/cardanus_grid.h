#ifndef CARDANUS_GRID_H
#define CARDANUS_GRID_H

#include <string>
#include "cardanus_key.h"

class Cardanus_Grid {
private:
    const int           SIZE;
    char**              m_grid;

                        Cardanus_Grid   ();
public:
    static const char   MOCK            = '*';

    void                set_at          (int row, 
                                         int col, 
                                         char symbol);
    char                get_at          (int row, int col) const;
    int                 get_size        () const;
    //std::string         get_text        (const Cardanus_Key&) const;
    std::string         to_string       (bool is_numbered = false) const;
                        Cardanus_Grid   (int size);
                        Cardanus_Grid   (const Cardanus_Grid&);
    Cardanus_Grid       operator=       (const Cardanus_Grid&);
};

#endif /* CARDANUS_GRID_H */