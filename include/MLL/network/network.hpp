#ifndef _MLL_NETWORK_HPP
#define _MLL_NETWORK_HPP

#include<vector>
#include<fstream>
#include<chrono>
#include<random>

#include"../matrix/matrix.hpp"

namespace MLL{
    class Network{
    public:     // TODO: MAKE PRIVATE
        bool m_training_mode;

        std::vector<int> m_layers;
        int m_last_layer;

        std::vector<Matrix> m_activations;
        std::vector<Matrix> m_weights;
        std::vector<Matrix> m_biases;
        std::vector<Matrix> m_sums;

        void init();

        int m_label;
        double m_cost;
        std::vector<double> m_target;

        std::vector<Matrix> m_activation_gradient;
        std::vector<Matrix> m_weight_gradient;
        std::vector<Matrix> m_bias_gradient;

        void randomize();
        void write_to_file(const char*);
        void read_from_file(const char*);


        void dela(int);
        void dela(){ dela(m_layers.size() - 1); }
        void delw();
        void delb();

        void update_gradients();

        void clear_activation_gradient();
        void clear_weight_gradient();
        void clear_bias_gradient();
        void clear_gradients();
    public:
        Network(std::vector<int>);  // Create a new network
        Network(const char*);       // Load a trained network from a file

        void set_input_layer(std::vector<double>, int);

        void calculate();
        std::vector<double>& get_result();
        double get_cost();

        void backprop(int);

        void debug_out();
    };
}

#endif//_MLL_NETWORK_HPP
