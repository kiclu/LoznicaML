/*
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<time.h>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

std::vector<MLL::Network> n({
    MLL::Network(std::vector<int>({784, 16, 16, 10})),
    MLL::Network(std::vector<int>({784, 300, 100, 10})),
    MLL::Network(std::vector<int>({784, 500, 150, 10})),
    MLL::Network(std::vector<int>({784, 2500, 2000, 1500, 1000, 500, 10}))
});

bool new_training = 0;

int layer_count = 0;
int* layer_size = nullptr;

int data_size = 0;
int subset_size = 0;
char* data_file = nullptr;
char* label_file = nullptr;


char* output_data = nullptr;
char* input_data = nullptr;


bool debug = 0;

void argument_parser(int, char**);
void show_help();

void digit_out();

int main(int argc, char* argv[]){
    //argument_parser(argc, argv);

    int correct = 0;
    int all = 0;

    std::cout << "Reading input files..." << std::endl;
    Dataset training_data("resources/train-images.idx3-ubyte",
    "resources/train-labels.idx1-ubyte", 60000, 0);

    Dataset test_data("resources/t10k-images.idx3-ubyte",
    "resources/t10k-labels.idx1-ubyte", 10000, 0);

    Dataset& data = training_data;

    //std::cout << "Select network [1 - " << n.size() << "]: ";
    //int network_index;
    //std::cin >> network_index;
    //MLL::Network& network = n.at(network_index-1);

    MLL::Network network(std::vector<int>({784, 16, 16, 10}));

    std::cout << "Network setup..." << std::endl;
    //network.debug_out();
    //network.read_from_file("training1.mlltd");
    //network.debug_out();
    const clock_t begin_time = clock();
    double cost = 0;
    for(int i=0;i<60000;i+=30){
        std::cout << "===========================\n";

        for(int j=0;j<30;j++){
            //std::cout << "---------------------------" << std::endl;

            //digit_out(data.get_at(i+j));

            network.set_input_layer(data.get_at(i+j).pixel, data.get_at(i+j).label);
            network.calculate();


            network.update_gradients();
            //network.debug_out();

            auto& res = network.get_result();
            double rm = res[0];
            int rmp = 0;
            for(int i=0;i<res.size();i++){
                //std::cout << res[i] << std::endl;

                if(res[i] > rm){
                    rm = res[i];
                    rmp = i;
                }
            }

            all++;
            if(data.get_at(i+j).label == rmp)
                correct++;


            //std::cout << data.get_at(i+j).label << " ~ " << rmp << std::endl << std::endl;

            cost += network.get_cost();

            //std::cout << std::endl << std::endl;
        }
        network.backprop(30);

        float time_elapsed_s = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
        float avg_time = time_elapsed_s / i;
        float eta = (avg_time * (10000 - i)) / (float)3600;
        std::cout << "Images done: " << i << '\n';
        std::cout << "Avg cost: " << cost/(double)i << '\n';
        std::cout << "Time elapsed: " << time_elapsed_s << "s" << '\n';
        std::cout << "ETA: " << eta << "h" << '\n';
    }


    network.write_to_file("training3.mlltd");
    std::cout << "PERCENT GUESSED: " << (correct/(double)all)*100 << "%" << std::endl;

    return 0;
}

void argument_parser(int argc, char* argv[]){
    for(int i=0;i<argc;i++){
        std::cout << argv[i] << ", ";
    }
    std::cout << std::endl << "--------------------" << std::endl;

    if(argc > 1){
        if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
            show_help();

        for(int i=0;i<argc;i++){
            if(!strcmp(argv[i], "-o") || !strcmp(argv[i], "--open")){
                new_training = false;
            }
        }
    }
}

void show_help(){
    std::ifstream fin("help_page.txt");

    std::string help_text((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    std::cout << help_text << std::endl;
    exit(0xF0);
}

void digit_out(Image& t_image){
    for(int p=0;p<28;p++){
        for(int q=0;q<28;q++){
            if(t_image.pixel.at(p*28 + q) > 0.1){
                std::cout << (char)219 << (char)219;
            }
            else{
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}
*/
