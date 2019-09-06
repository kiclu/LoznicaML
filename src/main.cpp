#include<iostream>
#include<iomanip>
#include<time.h>
#include<string>
#include<string.h>

#include<MLL/mll.hpp>
#include<mnist_parser/mnist_parser.hpp>

int correct = 0;
int all = 0;
double cost = 0;
double cost_sum = 0;

struct Config{
    bool training = false;
    bool test = false;

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

bool flag(char*, char*);

void time_out(float);

void cfg_debug(){
    std::cout << global_cfg.training << std::endl;
    std::cout << global_cfg.test << std::endl;

    std::cout << global_cfg.layer_count << std::endl;
    std::cout << "[ ";
    for(int i=0;i<global_cfg.layer_count;i++){
        std::cout << global_cfg.layer_size[i] << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << global_cfg.data_size << std::endl;
    std::cout << global_cfg.subset_size << std::endl;
    std::cout << global_cfg.data_file << std::endl;
    std::cout << global_cfg.label_file << std::endl;

    std::cout << global_cfg.network_data << std::endl;

    std::cout << global_cfg.verbose << std::endl;
    std::cout << global_cfg.debug << std::endl;
}

int main(int argc, char* argv[]){
    argument_parser(argc, argv);

    //cfg_debug();

    std::cout << "Reading the data..." << std::endl;
    Dataset data(
        global_cfg.data_file,
        global_cfg.label_file,
        global_cfg.data_size,
        0
    );

    std::cout << "Creating the network..." << std::endl;
    MLL::Network net(
        std::vector<int>(global_cfg.layer_size,
        global_cfg.layer_size + global_cfg.layer_count)
    );

    if(!global_cfg.training) net.read_from_file(global_cfg.network_data);

    std::cout << "Begin!" << std::endl;
    const clock_t begin_time = clock();
    for(int ss = 0; ss < global_cfg.data_size; ss += global_cfg.subset_size){
        for(int k = 0; k < global_cfg.subset_size; k++){
            int index = ss + k;


            net.set_input_layer(data[index].pixel, data[index].label);
            net.calculate();

            if(global_cfg.training) net.update_gradients();

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
            cost_sum += net.get_cost();
        }
        if(global_cfg.training) net.backprop(global_cfg.subset_size);

        float time_elapsed = float( clock () - begin_time ) /  CLOCKS_PER_SEC;
        float avg_time = time_elapsed / (ss + global_cfg.subset_size);
        float eta = (avg_time * (global_cfg.data_size - ss - global_cfg.subset_size));
        std::cout << "Images done: " << ss + global_cfg.subset_size << '\n';
        std::cout << "Avg cost: " << cost/(double)(ss + global_cfg.subset_size) << '\n';
        std::cout << "Time elapsed: ";
        time_out(time_elapsed);
        std::cout << std::setprecision(3) << "Average time: " << avg_time << "s\n";
        std::cout << "ETA: ";
        time_out(eta);

        if(!global_cfg.training){
            std::cout << "\n\n";
            std::cout << std::setprecision(4) << "Success rate: " << (correct / (double)all) * 100 << "%";
        }
        std::cout << "\n====================\n";
    }

    std::cout << std::endl << std::endl;
    std::cout << "Images done: " << global_cfg.data_size << '\n';
    std::cout << "Avg cost: " << cost_sum/(double)all << '\n';
    std::cout << "Time elapsed: ";
    time_out(float( clock () - begin_time ) /  CLOCKS_PER_SEC);


    if(!global_cfg.training){
        std::cout << std::endl << std::endl;
        std::cout << "SUCCESS RATE: " << std::setprecision(5) << (correct / (double)all) * 100 << "%\n";
    }

    if(global_cfg.training) net.write_to_file(global_cfg.network_data);

    return 0;
}

void argument_parser(int argc, char* argv[]){
    if(argc > 1){
        if(flag(argv[1], "-h") || flag(argv[1], "--help") || flag(argv[1], "?"))
            show_help();

        for(int i=1;i<argc;i++){
            //  MODE
            if(flag(argv[i], "-tr") || flag(argv[i], "--training")){
                global_cfg.training = true;
            }
            if(flag(argv[i], "-t") || flag(argv[i], "--test")){
                global_cfg.test = true;
            }

            //  NETWORK INFO
            if(flag(argv[i], "-lc") || flag(argv[i], "--layer-count")){
                global_cfg.layer_count = atoi(argv[++i]);
            }

            if(flag(argv[i], "-ls") || flag(argv[i], "--layer-size")){
                if(!global_cfg.layer_count > 0)
                    exit(0xEC0);
                global_cfg.layer_size = new int(global_cfg.layer_count);
                for(int j=0;j<global_cfg.layer_count;j++){
                    global_cfg.layer_size[j] = atoi(argv[++i]);
                }
            }

            //  FILE INFO
            if(flag(argv[i], "-df") || flag(argv[i], "--data")){
                global_cfg.data_file = new char(strlen(argv[++i]) + 1);
                strcpy(global_cfg.data_file, argv[i]);
            }

            if(flag(argv[i], "-lf") || flag(argv[i], "--labels")){
                global_cfg.label_file = new char(strlen(argv[++i]) + 1);
                strcpy(global_cfg.label_file, argv[i]);
            }

            if(flag(argv[i], "-nd") || flag(argv[i], "--network-data")){
                global_cfg.network_data = new char(strlen(argv[++i]) + 1);
                strcpy(global_cfg.network_data, argv[i]);
            }

            //  DATASET INFO
            if(flag(argv[i], "-ds") || flag(argv[i], "--data-size")){
                global_cfg.data_size = atoi(argv[++i]);
            }

            if(flag(argv[i], "-ss") || flag(argv[i], "--subset-size")){
                global_cfg.subset_size = atoi(argv[++i]);
            }

            //  MISC
            if(flag(argv[i], "-d") || flag(argv[i], "--debug")){
                global_cfg.debug = true;
            }
            if(flag(argv[i], "-v") || flag(argv[i], "--verbose")){
                global_cfg.verbose = true;
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

bool flag(char* t_str1, char* t_str2){
    return !strcmp(t_str1, t_str2);
}

void time_out(float t_sec){
    int sec = t_sec;
    int hr = sec / 3600;
    sec = sec % 3600;
    int mn = sec / 60;
    sec = sec % 60;

    std::cout << hr << "h " << mn << "m " << sec << "s\n";
}
