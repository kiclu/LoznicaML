#include<iostream>
#include<string.h>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

struct Config{
    bool training = false;

    int layer_count = 0;
    int* layer_size = nullptr;

    int data_size = 0;
    int subset_size = 0;
    char* data_file = nullptr;
    char* label_file = nullptr;

    char* network_data = nullptr;

    bool verbose = false;
    bool debug = false;
}global_cfg;

void argument_parser(int, char**);
void show_help();

int main(int argc, char* argv[]){
    argument_parser(argc, argv);

    Dataset data(
        global_cfg.data_file,
        global_cfg.label_file,
        global_cfg.data_size,
        0
    );

    MLL::Network net(
        std::vector<int>(global_cfg.layer_size,
        global_cfg.layer_size + global_cfg.layer_count)
    );

    if(!global_cfg.training) net.read_from_file(global_cfg.network_data);

    for(int ss = 0; ss < global_cfg.data_size; ss += global_cfg.subset_size){
        for(int k = 0; k < global_cfg.subset_size; ++k){
            int index = ss + k;


            net.set_input_layer(data[index].pixel, data[index].label);
            net.calculate();

            auto& res = net.get_result();
            double rm = res[0];
            int rmp = 0;
            for(int i=0;i<res.size();i++){
                if(res[i] > rm){
                    rm = res[i];
                    rmp = i;
                }
            }

            if(data[index].label == rmp)
                correct++;
            all++;

            cost += net.get_cost();
        }
        if(global_cfg.training) net.backprop(global_cfg.subset_size);

        float time_elapsed_s = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
        float avg_time = time_elapsed_s / i;
        float eta = (avg_time * (10000 - i)) / (float)3600;
        std::cout << "Images done: " << i << '\n';
        std::cout << "Avg cost: " << cost/(double)i << '\n';
        std::cout << "Time elapsed: " << time_elapsed_s << "s" << '\n';
        std::cout << "ETA: " << eta << "h" << '\n';
    }

    if(global_cfg.training) net.write_to_file(global_cfg.network_data);

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

        for(int i=1;i<argc;i++){

        }
    }
}

void show_help(){
    std::ifstream fin("help_page.txt");

    std::string help_text((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    std::cout << help_text << std::endl;
    exit(0xF0);
}
