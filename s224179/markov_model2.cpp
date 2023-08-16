#include <iostream>
#include <fstream>
#include "markov_model.hpp"
#include <cctype>
#include <algorithm>

Alphabet alphabet;

Alphabet build_alphabet (std::string file) {
    
    for (char c : file) {
        if (std::isalpha(c)) {
            //std::cout << "Character " << c << " is alpha" << std::endl;
            //bool a = alphabet.find(c)!=alphabet.end();
            //std::cout << c << ":" << a << std::endl;
            //std::cout << "Inserting element" << c << std::endl;
            alphabet.insert(c);
        }
    }
    /* for (char letter : alphabet) {
        std::cout << letter << " ";
    } */
    return alphabet;   
}

void markov_model(Markov_model& markov_m, unsigned int order, const std::string& training_file) {

    markov_m.order = order;
    std::string file_circular = training_file + training_file.substr(0, order-1);

    if (training_file.length() < order ) {
        throw std::length_error("[MARKOV MODEL ERROR]: Incompatible input data and order size");
    }

/*     for (size_t i = 0; i < training_file.length(); ++i) {
        if (std::isalpha(training_file[i])) {
            markov_m.alphabet.insert(training_file[i]);
        }
    } */
    markov_m.alphabet = alphabet;

    for (size_t k = 0; k + order <= training_file.length(); k++) {

        markov_m.model[file_circular.substr(k, order+1)]++;
    }

    for (size_t k = 0; k + order <= training_file.length(); k++) {

        markov_m.model[file_circular.substr(k, order)]++;
    }    
        
}

double laplace (const Markov_model& markov_m,const std::string& string) {
    
    if (string.length() < markov_m.order) {
        throw std::length_error("[LAPLACE ERROR]: Incompatible input data and order size");
    }

    for (size_t i = 0; i < string.length(); i++) {
        if (markov_m.alphabet.find(string[i]) == markov_m.alphabet.end()) {
            throw std::domain_error("[LAPLACE ERROR]: String contains illegal characters.");
        }
    }

    double res = 0.0;
    std::string substring = string.substr(0, string.length() - 1);
    auto iterString = markov_m.model.find(string);
    auto iterSubstring = markov_m.model.find(substring);

    if (iterString != markov_m.model.end()) {
        res = (iterString->second + 1.0) / (iterSubstring->second + markov_m.alphabet.size());
    } else {
        res = 1.0 / (iterSubstring->second + markov_m.alphabet.size());
    }

    return res;

}

double likelihood(Markov_model& markov_m,const std::string& input_data) {

    double res = 0.0;
    std::string circ_input = input_data + input_data.substr(0, markov_m.order-1);

    for (size_t k = 0; k + markov_m.order <= input_data.length(); k++) {
        res += std::log(laplace(markov_m, circ_input.substr(k, markov_m.order+1)));
    }

    return res;
}