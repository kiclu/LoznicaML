#include<MLL/mll.hpp>

#define SQR(x) ((x)*(x))
#define agrad m_activation_gradient
#define bgrad m_bias_gradient
#define wgrad m_weight_gradient


void MLL::Matrix::matrix_alloc(int t_height, int t_width){
    m_matrix.resize(t_height);
    for(int i=0;i<t_height;i++){
        m_matrix.at(i).resize(t_width);
    }
}

MLL::Matrix::Matrix(){

}

MLL::Matrix::Matrix(int t_height, int t_width){
    matrix_alloc(t_height, t_width);
}

MLL::Matrix::Matrix(int t_height, int t_width, double* t_data){
    matrix_alloc(t_height, t_width);
    copy_data(t_data);
}

MLL::Matrix& MLL::Matrix::operator=(MLL::Matrix& t_matrix){
    matrix_alloc(t_matrix.get_height(), t_matrix.get_width());
    copy_data(t_matrix.get_data());
    return *this;
}

MLL::Matrix& MLL::operator+(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if( t_matrix1.get_height() != t_matrix2.get_height() ||
        t_matrix1.get_width() != t_matrix2.get_width()){
        std::cout << "Unmatched sizes (+)" << std::endl;
        t_matrix1.debug_out();
        t_matrix2.debug_out();
        exit(0xE00);
    }

    static MLL::Matrix result;
    result.matrix_alloc(t_matrix1.get_height(), t_matrix1.get_width());

    for(int i=0;i<t_matrix1.get_height();i++){
        for(int j=0;j<t_matrix1.get_width();j++){
            result[i][j] = t_matrix1[i][j] + t_matrix2[i][j];
        }
    }

    return result;
}

MLL::Matrix& MLL::operator*(MLL::Matrix& t_matrix1, MLL::Matrix& t_matrix2){
    if(t_matrix1.get_width() != t_matrix2.get_height()){
        std::cout << "Unmatched sizes (*)" << std::endl;
        t_matrix1.debug_out();
        t_matrix2.debug_out();
        exit(0xE01);
    }

    static MLL::Matrix result;
    result.matrix_alloc(t_matrix1.get_height(), t_matrix2.get_width());

    for(int i=0;i<t_matrix1.get_height();i++){
        for(int j=0;j<t_matrix2.get_width();j++){
            result[i][j] = 0;
            for(int k=0;k<t_matrix1.get_width();k++){
                result[i][j] += t_matrix1[i][k] * t_matrix2[k][j];
            }
        }
    }

    return result;
}

MLL::Matrix& MLL::operator~(MLL::Matrix& t_matrix){
    static MLL::Matrix result;
    result.matrix_alloc(t_matrix.get_height(), t_matrix.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result[i][j] = sigm(t_matrix[i][j]);
        }
    }

    return result;
}

MLL::Matrix& MLL::operator!(MLL::Matrix& t_matrix){
    static MLL::Matrix result;
    result.matrix_alloc(t_matrix.get_height(), t_matrix.get_width());

    for(int i=0;i<result.get_height();i++){
        for(int j=0;j<result.get_width();j++){
            result[i][j] = sigm_deriv(t_matrix[i][j]);
        }
    }

    return result;
}

void MLL::Matrix::randomize(){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 0.5);

    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = distribution(generator) * 0.5;
        }
    }
}

void MLL::Matrix::randomize(int t_layer_size){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(0.0, 1.0);

    double coef = sqrt(1 / (double)t_layer_size);
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = distribution(generator) * coef;
        }
    }
}

void MLL::Matrix::fill(int t_height, int t_width, double t_fill){
    matrix_alloc(t_height, t_width);
    for(int i=0;i<t_height;i++){
        for(int j=0;j<t_width;i++){
            m_matrix[i][j] = t_fill;
        }
    }
}

void MLL::Matrix::debug_out(){
    std::cout << "-------------------------------" << std::endl << "Matrix debug:";
    std::cout << std::endl;
    std::cout << "[" << get_height() << " x " << get_width() << "]" << std::endl;
    std::cout << std::endl;
    std::cout << "Show? y/n ";
    int c = getchar();
    int e = getchar();
    if(c == 'y'){
        for(int i=0;i<get_height();i++){
            for(int j=0;j<get_width();j++){
                std::cout << m_matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << "-------------------------------" << std::endl;
}

void MLL::Matrix::copy_data(double* t_data){
    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            m_matrix[i][j] = t_data[i*get_width() + j];
        }
    }
}

double* MLL::Matrix::get_data(){
    double* data = new double[get_height() * get_width()];

    for(int i=0;i<get_height();i++){
        for(int j=0;j<get_width();j++){
            data[i*get_width() + j] = m_matrix[i][j];
        }
    }

    return data;
}

double MLL::sigm(double x){ return 1 / (1 + exp(-x)); }
double MLL::sigm_deriv(double x){ return sigm(x) * (1 - sigm(x)); }

void MLL::Network::init(){
    m_activations.resize(m_layers.size());
    if(m_training_mode) m_activation_gradient.resize(m_layers.size());
    for(unsigned i=0;i<m_activations.size();i++){
        m_activations[i].matrix_alloc(m_layers[i], 1);
        if(m_training_mode)
            m_activation_gradient[i].matrix_alloc(m_layers[i], 1);
    }

    m_weights.resize(m_layers.size() - 1);
    if(m_training_mode) m_weight_gradient.resize(m_layers.size() - 1);
    for(unsigned i=0;i<m_weights.size();i++){
        m_weights[i].matrix_alloc(m_layers[i+1], m_layers[i]);
        if(m_training_mode)
            m_weight_gradient[i].matrix_alloc(m_layers[i+1], m_layers[i]);
    }
    randomize();

    m_biases.resize(m_layers.size() - 1);
    if(m_training_mode) m_bias_gradient.resize(m_layers.size() - 1);
    for(unsigned i=0;i<m_biases.size();i++){
        m_biases[i].matrix_alloc(m_layers[i+1], 1);
        if(m_training_mode)
            m_bias_gradient[i].matrix_alloc(m_layers[i+1], 1);
    }


    m_sums.resize(m_layers.size() - 1);
    for(unsigned i=0;i<m_sums.size();i++){
        m_sums[i].matrix_alloc(m_layers[i], 1);
    }

    m_target.resize(m_layers.back());
}

void MLL::Network::randomize(){
    for(unsigned i=0;i<m_weights.size();i++){
        m_weights[i].randomize(m_layers[i]);
    }
}

void MLL::Network::write_to_file(const char* t_filename){
    std::ofstream fout(t_filename, std::ios::binary | std::ios::out);
    if(!fout.is_open()){
        std::cout << "Couldn't open file!" << std::endl;
        exit(0xE10);
    }

    fout.seekp(0, std::ios::beg);

    for(int k=0;k<m_weights.size();k++){
        for(int i=0;i<m_weights[k].get_height();i++){
            for(int j=0;j<m_weights[k].get_width();j++){
                fout.write((char*)&m_weights[k][i][j], sizeof(double));
            }
        }
    }

    for(int k=0;k<m_biases.size();k++){
        for(int i=0;i<m_biases[k].get_height();i++){
            for(int j=0;j<m_biases[k].get_width();j++){
                fout.write((char*)&m_biases[k][i][j], sizeof(double));
            }
        }
    }
}

void MLL::Network::read_from_file(const char* t_filename){
    std::ifstream fin(t_filename, std::ios::binary | std::ios::in);
    if(!fin.is_open()){
        std::cout << "Couldn't open file!" << std::endl;
        exit(0xE11);
    }

    fin.seekg(0, std::ios::beg);

    for(int k=0;k<m_weights.size();k++){
        for(int i=0;i<m_weights[k].get_height();i++){
            for(int j=0;j<m_weights[k].get_width();j++){
                fin.read((char*)&m_weights[k][i][j], sizeof(double));
            }
        }
    }

    for(int k=0;k<m_biases.size();k++){
        for(int i=0;i<m_biases[k].get_height();i++){
            for(int j=0;j<m_biases[k].get_width();j++){
                fin.read((char*)&m_biases[k][i][j], sizeof(double));
            }
        }
    }
}

void MLL::Network::dela(int t_layer){
    if(t_layer>=0){
        if(t_layer ==  m_last_layer){
            for(int i=0;i<m_layers.back();i++){
                agrad[t_layer][i][0]=2*(m_activations[t_layer][i][0] - m_target[i]);
            }
            dela(t_layer - 1);
        }
        else{
            Matrix matr = !m_sums[t_layer];
            for(int i=0;i<m_layers[t_layer];i++){
                agrad[t_layer][i][0] = 0;
                for(int j=0;j<m_layers[t_layer + 1];j++){
                    agrad[t_layer][i][0] += matr[j][0] * agrad[t_layer + 1][j][0] * m_weights[t_layer][j][i];
                }
            }
            dela(t_layer - 1);
        }
    }
}

void MLL::Network::delw(){
    for(int l=0;l<wgrad.size();l++){
        Matrix matr = !m_sums[l];
        for(int i=0;i<m_layers[l + 1];i++){
            for(int j=0;j<m_layers[l];j++){
                wgrad[l][i][j] += m_activations[l][j][0] * matr[i][0] * agrad[l + 1][i][0];
            }
        }
    }
}

void MLL::Network::delb(){
    for(int l=0;l<bgrad.size();l++){
        Matrix matr = !m_sums[l];
        for(int i=0;i<m_layers[l+1];i++){
            bgrad[l][i][0] += matr[i][0] * agrad[l + 1][i][0];
        }
    }
}

void MLL::Network::update_gradients(){
    dela();
    delw();
    delb();
    clear_activation_gradient();
}

void MLL::Network::clear_activation_gradient(){
    for(int i=0;i<m_activation_gradient.size();i++){
        m_activation_gradient[i].matrix_alloc(m_layers[i], 1);
    }
}

void MLL::Network::clear_weight_gradient(){
    for(int i=0;i<m_weight_gradient.size();i++){
        m_weight_gradient[i].matrix_alloc(m_layers[i+1], m_layers[i]);
    }
}

void MLL::Network::clear_bias_gradient(){
    for(int i=0;i<m_bias_gradient.size();i++){
        m_bias_gradient[i].matrix_alloc(m_layers[i+1], 1);
    }
}

void MLL::Network::clear_gradients(){
    clear_activation_gradient();
    clear_weight_gradient();
    clear_bias_gradient();
}

MLL::Network::Network(std::vector<int> t_layers){
    m_layers = t_layers;
    m_last_layer = m_layers.size() - 1;
    m_training_mode = false;

    init();
}

MLL::Network::Network(const char* t_filename){

}

void MLL::Network::set_input_layer(std::vector<double> t_activations, int t_label){
    for(unsigned i=0;i<m_layers.front();i++){
        m_activations[0][i][0] = t_activations[i];
    }

    m_label = t_label;

    for(unsigned i=0;i<m_target.size();i++){
        m_target[i] = (i == m_label);
    }
}

void MLL::Network::calculate(){
    for(int i=0;i<m_layers.size()-1;i++){
        m_sums[i] = (m_weights[i] * m_activations[i]) + m_biases[i];
        m_activations[i+1] = ~m_sums[i];
    }

    if(m_training_mode){
        update_gradients();
    }
}

std::vector<double>& MLL::Network::get_result(){
    static std::vector<double> result;
    result.resize(m_layers.back());

    for(int i=0;i<result.size();i++){
        result[i] = m_activations.back()[i][0];
    }

    return result;
}

double MLL::Network::get_cost(){
    double result=0;
    for(int i=0;i<m_activations[m_last_layer].get_height();i++)
        result+=SQR(m_activations[m_last_layer][i][0] - m_target[i]);
    return result;
}

void MLL::Network::backprop(int t_subset_size){
    for(int l=0;l<wgrad.size();l++){
        for(int i=0;i<m_layers[l + 1];i++){
            bgrad[l][i][0] /= t_subset_size;
            m_biases[l][i][0] -= bgrad[l][i][0];
            for(int j=0;j<m_layers[l];j++){
                wgrad[l][i][j] /= t_subset_size;
                m_weights[l][i][j] -= wgrad[l][i][j];
            }
        }
    }
    clear_gradients();
}

void MLL::Network::debug_out(){
    std::cout << "===============================" << std::endl << "Network debug:";
    std::cout << std::endl << std::endl;

    std::cout << "Layer count: " << m_layers.size() << std::endl << "[ ";
    for(unsigned i=0;i<m_layers.size();i++){
        std::cout << m_layers[i] << " ";
    }
    std::cout << "]" << std::endl << std::endl;

    {
        std::cout << "Activation layers: " << m_activations.size() << std::endl;
        std::cout << "Show? y/n " ;
        int c = getchar();
        int e = getchar();
        if(c == 'y'){
            for(unsigned i=0;i<m_activations.size();i++){
                m_activations[i].debug_out();
            }
        }
    }



    {
        std::cout << "Weight layers: " << m_weights.size() << std::endl;
        std::cout << "Show? y/n " ;
        int c = getchar();
        int e = getchar();
        if(c == 'y'){
            for(unsigned i=0;i<m_weights.size();i++){
                m_weights[i].debug_out();
            }
        }
    }



    {
        std::cout << "Bias layers: " << m_biases.size() << std::endl;
        std::cout << "Show? y/n " ;
        int c = getchar();
        int e = getchar();
        if(c == 'y'){
            for(unsigned i=0;i<m_biases.size();i++){
                m_biases[i].debug_out();
            }
        }
    }

    std::cout << std::endl << std::endl << "===============================" << std::endl;
}
