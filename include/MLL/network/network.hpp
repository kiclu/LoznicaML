#ifndef _MLL_NETWORK_HPP
#define _MLL_NETWORK_HPP

#include<vector>
#include<fstream>

#include"../matrix/matrix.hpp"

namespace MLL{
    class Network{
    private:
        std::vector<int> m_layers;

        int m_label;
        double m_cost;
        std::vector<Matrix> m_activations;
        std::vector<Matrix> m_weights;
        std::vector<Matrix> m_biases;

        void randomize();
        void write_to_file(const char*);
        void read_from_file(const char*);
    public:
        Network(std::vector<int>);  // Create a new network
        Network(const char*);       // Load a trained network from a file

        void set_input(std::vector<double>, int);

        void calculate();

        std::vector<double> get_result();
        double get_cost();


    };
}

#endif//_MLL_NETWORK_HPP
