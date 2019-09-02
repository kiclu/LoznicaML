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
        std::vector<int> m_layers;

        std::vector<Matrix> m_activations;
        std::vector<Matrix> m_weights;
        std::vector<Matrix> m_biases;

        int m_label;
        double m_cost;

        std::vector<Matrix> m_weight_gradient;
        std::vector<Matrix> m_bias_gradient;

        void randomize();
        void write_to_file(const char*);
        void read_from_file(const char*);

        void clear_gradient();
    public:
        Network(std::vector<int>);  // Create a new network
        Network(const char*);       // Load a trained network from a file

        void set_input_layer(std::vector<double>, int);

        void calculate();

        std::vector<double> get_result();
        double get_cost();

        void backprop();

        void debug_out();
    };
}

#endif//_MLL_NETWORK_HPP
