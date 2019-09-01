#include<iostream>
#include<fstream>
#include<string>
#include<string.h>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

void argument_parser(int, char**);
void show_help();

int main(int argc, char* argv[]){
    //argument_parser(argc, argv);

    Dataset data("resources/train-images.idx3-ubyte", "resources/train-labels.idx1-ubyte", 500, 0);

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
