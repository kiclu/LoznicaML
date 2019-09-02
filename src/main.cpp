#include<iostream>
#include<fstream>
#include<string>
#include<string.h>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

void argument_parser(int, char**);
void show_help();

void matrix_test(){
    double data1[] = {
        4,  5,  2,  9,  4,
        4,  5,  2,  6,  5,
        5, 10,  8,  0,  1
    };

    double data2[] = {5, 3, 5, 10, 8};

    double data3[] = {1, 1, 1};

    MLL::Matrix m1(3, 5, data1);
    MLL::Matrix m2(5, 1, data2);
    MLL::Matrix m3(3, 1, data3);

    auto m4 = (m1*m2) + m3;

    m4.debug_out();
}

int main(int argc, char* argv[]){
    //argument_parser(argc, argv);


    Dataset data("resources/train-images.idx3-ubyte",
    "resources/train-labels.idx1-ubyte", 10000, 0);


    MLL::Network n1(std::vector<int>({784, 16, 16, 10}));
    n1.debug_out();

    /*
    for(int i=0;i<500;i+=100){
        double cost = 0;
        for(int j=0;j<100;j++){
            for(int p=0;p<28;p++){
                for(int q=0;q<28;q++){
                    if(data.get_at(i+j).pixel.at(p*28 + q) > 0.1){
                        std::cout << (char)219 << (char)219;
                    }
                    else{
                        std::cout << "  ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << data.get_at(i+j).label << std::endl << std::endl;

            n1.set_input(data.get_at(i+j).pixel, data.get_at(i+j).label);
            n1.calculate();
            cost += n1.get_cost();
            std::cout << "got_cost" << std::endl;



            n1.debug_out();

            int c = getchar();
            system("CLS");
        }
        //std::cout << cost << std::endl;
        //n1.backprop();
    }
    */

    /*
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
    */
    return 0;
}

void argument_parser(int argc, char* argv[]){
    for(int i=0;i<argc;i++){
        std::cout << argv[i] << std::endl;
    }
    std::cout << "--------------------" << std::endl;

    if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        show_help();


}

void show_help(){
    std::ifstream fin("help_page.txt");

    std::string help_text((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    std::cout << help_text << std::endl;
    exit(1);
}
