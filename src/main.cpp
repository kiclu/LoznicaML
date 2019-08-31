#include<iostream>

#include<MLL/mll.hpp>

int main(){
    double data1[] = {
        4, 5, 2, 9, 4,
        4, 5, 2, 6, 5,
        5, 10, 8, 0, 1
    };

    double data2[] = {
        5,
        3,
        5,
        10,
        8
    };

    MLL::Matrix m1(3, 5, data1);
    MLL::Matrix m2(5, 1, data2);

    auto m3 = m1 * m2;

    std::cout << m3[1][0] << std::endl;

    return 0;
}
