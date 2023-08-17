#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include "markov_model.hpp"

int main (int argc, char *argv[]) {

    int order;
    try {
        order = std::stoi(argv[1]);
        
        if (order < 1) {
            std::cout << "Order must be greater than 0" << std::endl;
            return 1;
        }
    } catch (const std::invalid_argument& e) {
        std::cout << "[INVALID ORDER]: " << "Usage: " << argv[0] << " <order> <training_file> --- <test_file>" << std::endl;
        return 1;
    }
    std::vector<std::string> training_files;
    std::vector<std::string> test_files;
    bool delim = false;
    std::vector<Markov_model> models;
    std::vector<std::string> names;
    std::string line;

    for (int i = 2; i < argc; i++) {
        if (std::string(argv[i]) == "---") {
        delim = true;
        continue; // Skip processing this argument and move to the next one
        }
        if (!delim) {
            training_files.push_back(argv[i]);
        } 
        else {
            test_files.push_back(argv[i]);
        }
    }

    if (training_files.size() == 0 || test_files.size() == 0) {
        std::cout << "Usage: " << argv[0] << " <order> <training_file> --- <test_file>" << std::endl;
        return 1;
    }

    /* std::cout << "Training files:" << std::endl;
    for (size_t i = 0; i < training_files.size(); i++) {
        std::cout << training_files[i] << std::endl;
    }
    std::cout << "Test files:" << std::endl;
    for (size_t i = 0; i < test_files.size(); i++) {
        std::cout << test_files[i] << std::endl;
    }*/
    std::string training_str[argc-1]; 
    std::string test_str[argc-1]; 

    for (size_t i = 0; i < training_files.size(); i++) {

        std::ifstream training(training_files[i]);
        getline(training, line);
        names.push_back(line);

        while (getline(training, line)) {
            training_str[i] += line; //+ '\n';
        }
        training_str[i].erase(std::remove_if(training_str[i].begin(), training_str[i].end(), ::isspace), training_str[i].end());
        //std::cout << training_str[i] << std::endl;
        //DA RIMUOVERE
        //build_alphabet(training_str[i]);
    }

    for (size_t i = 0; i < training_files.size(); i++) {

        std::ifstream test(test_files[i]);

        while (getline(test, line)) {
            test_str[i] += line; //+ '\n';
        }
        test_str[i].erase(std::remove_if(test_str[i].begin(), test_str[i].end(), ::isspace), test_str[i].end());
        //std::cout << training_str[i] << std::endl;
        //DA RIMUOVERE
        //build_alphabet(training_str[i]);
    }


/*     for (size_t i = 0; i < test_files.size(); i++) {
        std::cout << "Test: " << test_str[i] << std::endl;
    } */
    
    for (size_t i = 0; i < training_files.size(); i++) {
        Markov_model model;

        //std::ifstream *file = open_training(std::string(training_files[i]));
        //names.push_back(get_firstline(file));
        //close_file(file);
       
        markov_model(model, order, training_str[i]);

        models.push_back(model);
    }
    

/*     for (size_t i = 0; i < models.size(); i++) {
        std::cout << "Model " << names[i] << std::endl;
        std::cout << "Order: " << models[i].order << std::endl;
        for (int j : models[i].alphabet) {
            std::cout << "Alphabetfin: " << static_cast<char>(j) << "\n";
        }
        for (const auto& entry : models[i].model) {
            std::cout << entry.first << ": " << entry.second << std::endl;
        }
        std::cout << std::endl;
    } */
 
    std::vector<double> results;

    for (size_t i = 0; i < test_files.size(); i++) {
        double max_likelihood = -std::numeric_limits<double>::infinity();
        int max_likelihood_model_index = -1;

        for (size_t j = 0; j < models.size(); j++) {
            //std::cout << "Testing " << test_files[i] << " with model " << names[j] << std::endl;
            try {
                /* std::cout << "Testing: " << test_str[i] << " with " << std::endl;
                
                std::cout << "Model: " << names[j] << std::endl;
                std::cout << "Order: " << models[j].order << std::endl;
                std::cout << "Alphabet:" << std::endl;
                
                for (const auto& entry : models[j].alphabet) {
                    std::cout << entry << std::endl;
                }
                
                for (const auto& entry : models[j].model) {
                    std::cout << entry.first << ": " << entry.second << std::endl;
                } */ 
                double res = likelihood(models[j], test_str[i]);
                //std::cout << "likelihood: " << res << std::endl;
                if (res > max_likelihood) {
                    max_likelihood = res;
                    max_likelihood_model_index = j;
                }
                std::cout << names[j] << ": " << res << "\n" << std::endl;
            } catch (const std::domain_error& e) {
                results.push_back(-1.0); // Use -1.0 to indicate test data is incompatible
                //std::cout << names[j] << ": -" << "\n" << std::endl;
                std::cout << "ERORRE" << std::endl;
            } catch (const std::length_error& e) {
                results.push_back(-1.0); // Use -1.0 to indicate test data is incompatible
                //std::cout << names[j] << ": -" << "\n" << std::endl;
                //std::cout << "ERORRE" << std::endl;
            }
        }
        //std::cout << "max_likelihood: " << max_likelihood << std::endl;
        // Check if a model exists with a likelihood greater than -1.0 (incompatible)
        if (max_likelihood_model_index != -1 && max_likelihood != -1.0) {
            std::cout << test_files[i] << " attributed to model " << names[max_likelihood_model_index] << "\n" << std::endl;
        } else {
            std::cout << test_files[i] << " attributed to model -\n" << std::endl;
        }
    }
    return 0;
}