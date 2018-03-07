#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <string>
#include "cardanus_grid.h"
#include "cardanus_key.h"
#include "rotation_sequence.h"
#include "cipher_set.hpp"

class Decryptor {
private:
    static const char*      MOCK;
    Cardanus_Grid*          mp_grid;
    Cardanus_Key*           mp_key;
    
    std::string             decr        (Cardanus_Grid,
                                         Cardanus_Key,
                                         Rotation_Sequence) const;
public:
    std::string             decrypt     (const Cipher_Set&);
    std::string             decrypt     (const Cardanus_Grid&,
                                         const Cardanus_Key&,
                                         const Rotation_Sequence&) const;
    std::string             decrypt     (const Cardanus_Key&, const Rotation_Sequence&) const;
    std::string             decrypt     (const Rotation_Sequence&) const;
    void                    set_grid    (const Cardanus_Grid& grid);
    void                    set_key     (const Cardanus_Key& key);
    void                    unset_grid  ();
    void                    unset_key   ();
                            Decryptor   ();
                            Decryptor   (const Decryptor& decryptor);
    virtual                 ~Decryptor  ();    
	Decryptor               operator=   (const Decryptor& decryptor);
};

#endif /* DECRYPTOR_H */