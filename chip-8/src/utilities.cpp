#include "utilities.hpp"

OPCODE_TYPE gen_XYNN_opcode(const int A, const int X, const int NN) {
    int A_ = 0xF & A;
    int X_ = 0xF & X;
    int NN_ = 0xFF & NN;
    return (A_ << 12) + (X_ << 8) + NN_;
}
