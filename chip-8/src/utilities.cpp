#include "utilities.hpp"

OPCODE_TYPE gen_XYNN_opcode(const int A, const int X, const int NN) {
    int A_ = 0xF & A;
    int X_ = 0xF & X;
    int NN_ = 0xFF & NN;
    return (A_ << 12) + (X_ << 8) + NN_;
}

OPCODE_TYPE gen_WXYZ_opcode(const int W, const int X, const int Y, const int Z) {
    int W_ = 0xF & W;
    int X_ = 0xF & X;
    int Y_ = 0xF & Y;
    int Z_ = 0xF & Z;
    return (W_ << 12) + (X_ << 8) + (Y_ << 4) + Z_;
}
