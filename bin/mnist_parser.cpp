#include<mnist_parser/mnist_parser.hpp>

Dataset::Dataset(const char* t_imgfile, const char* t_lblfile, int t_length=IMAGE_NUM,int t_pos=0){
    int a;
    unsigned char tmp;
    std::ifstream fin;

    data.resize(t_length);

    fin.open(t_imgfile, std::ios::binary | std::ios::in);
    fin.seekg(0, std::ios::beg);

    for(int i=0;i<4;i++){
        fin.read((char*)&a,sizeof(a));
    }

    fin.seekg(t_pos*IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(char), std::ios::cur);
    for(int i=0;i<data.size();i++){
        for(int j=0;j<IMAGE_WIDTH*IMAGE_HEIGHT;j++){
            fin.read((char*)&tmp, sizeof(char));
            data[i].pixel[j] = tmp; //div by 255
        }
    }
    fin.close();

    fin.open(t_lblfile, std::ios::binary | std::ios::in);
    fin.seekg(0, std::ios::beg);

    for(int i=0;i<2;i++){
        fin.read((char*)&a, sizeof(a));
    }

    fin.seekg(t_pos*sizeof(char), std::ios::cur);
    for(int i=0;i<data.size();i++){
        fin.read((char*)&tmp, sizeof(char));
        data[i].label=(tmp);
    }
    fin.close();
}
