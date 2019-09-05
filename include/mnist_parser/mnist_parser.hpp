#ifndef _MLL_MNIST_PARSER_HPP
#define _MLL_MNIST_PARSER_HPP

#define IMAGE_HEIGHT 28
#define IMAGE_WIDTH 28
#define IMAGE_NUM 60000

#include<vector>
#include<fstream>
#include<iostream>

/*
MNIST DATABASE
http://yann.lecun.com/exdb/mnist/


TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000801(2049) magic number (MSB first)
0004     32 bit integer  60000            number of items
0008     unsigned byte   ??               label
0009     unsigned byte   ??               label
........
xxxx     unsigned byte   ??               label
The labels values are 0 to 9.


TRAINING SET IMAGE FILE (train-images-idx3-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000803(2051) magic number
0004     32 bit integer  60000            number of images
0008     32 bit integer  28               number of rows
0012     32 bit integer  28               number of columns
0016     unsigned byte   ??               pixel
0017     unsigned byte   ??               pixel
........
xxxx     unsigned byte   ??               pixel
Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means foreground (black).



Networks:

784-16-16-10 ~ ??

784-300-100-10 ~ 96.95%

784-500-150-10 ~ 97.05%

6-layer NN 784-2500-2000-1500-1000-500-10 (on GPU) [elastic distortions] ~ 99.65%
*/

struct Image{
    std::vector<double> pixel;
    int label;

    Image(){ pixel.resize(IMAGE_HEIGHT * IMAGE_WIDTH); }
};


class Dataset{
private:
    std::vector<Image> data;
public:
    Dataset(int t_length){ data.resize(t_length); }
    //Dataset(Dataset&, int, int);
    Dataset(const char*, const char*, int, int);
    //readDataBase(ifstream, int);

    Image& get_at(int t_index){ return data[t_index]; }

    Image& operator[](int t_index){ return data[t_index]; }
};


#endif//_MLL_MNIST_PARSER_HPP
