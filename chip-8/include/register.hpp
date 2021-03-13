#ifndef CHIP_8_REGISTER_HPP_
#define CHIP_8_REGISTER_HPP_

class Register {
public:
    Register();

    int get() const;
    void set(int);
private:
    int value_{};
};

#endif //CHIP_8_REGISTER_HPP_
