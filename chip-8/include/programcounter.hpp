#ifndef CHIP_8_PROGRAMCOUNTER_HPP_
#define CHIP_8_PROGRAMCOUNTER_HPP_
class ProgramCounter{
public:
    ProgramCounter();
    int get() const;
    void set(int);
private:
    int value_;
};
#endif //CHIP_8_PROGRAMCOUNTER_HPP_
