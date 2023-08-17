#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<map>
#include<cmath>
#include <fstream>
#include <cctype>
#include <stdexcept>
//#include <filesystem>

typedef std::set<char> Alphabet;
typedef std::map<std::string,unsigned int> Model;

struct Markov_model{
    unsigned int order;
    Alphabet alphabet;
    Model model;
};


std::ifstream* open_test (std::string filename);
std::ifstream* open_training (std::string filename);
void close_file (std::ifstream* file);
std::string get_firstline (std::ifstream* file);
std::string token_test_file (std::ifstream* file);
std::string token_training_file (std::ifstream* file);
//DA RIMUOVERE(SE NEWS SU ALFABETO E NEL CASO CAPIRE QUALE VERSIONE DA USARE) Alphabet build_alphabet (std::string file);
Alphabet build_alphabet (Alphabet alphabet, std::string file);
void build_model (int order, Model& model, std::string file);
void markov_model(Markov_model& markov_m, unsigned int order, const std::string& training_file);
double laplace (const Markov_model& markov_m,const std::string& string);
double likelihood(Markov_model& markov_m,const std::string& input_data);