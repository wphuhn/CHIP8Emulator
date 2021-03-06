#ifndef CHIP_8_REGISTER_HPP_
#define CHIP_8_REGISTER_HPP_

class Register {
public:
    Register();

    const int get(void);
    void set(const int);
private:
    int value_;
};

#endif //CHIP_8_REGISTER_HPP_
