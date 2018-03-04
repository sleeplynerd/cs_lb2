#ifndef ERRORS_H
#define ERRORS_H

class Basic_Err {
    virtual const char* what() const = 0;
};

class Matrix_Dimension_Err : public Basic_Err {
    inline virtual const char* what() const {
        return "Matrix dimensions must be agreed";
    }
};

#endif /* ERRORS_H */