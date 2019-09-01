#include<iostream>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

int main(){
    /*
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
    */

    Dataset data("resources/train-images.idx3-ubyte", "resources/train-labels.idx1-ubyte", 500, 0);

    /*
    int tmp;
    std::cin >> tmp;
    */

    for(int tmp=0;tmp<500;tmp++){

        for(int i=0;i<28;i++){
            for(int j=0;j<28;j++){
                if(data.get_at(tmp).pixel.at(i*28 + j) > 50){
                    std::cout << (char)219 << (char)219;
                }
                else{
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << data.get_at(tmp).label << std::endl << std::endl;

    }

    return 0;
}

// ispisi(pozovi("cpp.exe", 2, "slika.jpg", "???"));
