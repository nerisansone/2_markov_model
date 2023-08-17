#include "markov_model.hpp"



Model model2; //global variable for the model with shorter strings

//function that opens a test file and returns it
std::ifstream* open_test (std::string filename) {
    
    //std::cout << "Opening test file" << std::endl;
    std::ifstream* file = new std::ifstream;
    std::string path = "./tests/";

    std::string full_path = path + filename;
    //std::cout << full_path << std::endl;

    file->open (full_path, std::ios::in);

    if (!file) {
        delete file;
        throw std::runtime_error("ERROR: Couldn't open file'");
    }
    else {
        //std::cout << "File opened successfully" << std::endl;
        return file;
    }

}
//function that opens a training file and returns it
std::ifstream* open_training (std::string filename) {
    
    
    std::ifstream* file = new std::ifstream;
    std::string path = "./";//materials/training/";

    std::string full_path = path + filename;
    //std::cout << filename << std::endl;
    
    file->open (full_path, std::ios::in);
    //std::cout << "Opening training file" << std::endl;
    if (!file) {
        delete file;
        throw std::runtime_error("ERROR: Couldn't open file'");
    }
    else {
        //std::cout << "File opened successfully" << std::endl;
        return file;
    }

}

//function that closes a file
void close_file (std::ifstream* file) {
    file->close();
    delete file;
    //std::cout << "\nFile closed successfully" << std::endl;
}

std::string get_firstline (std::ifstream* file) {
    
    std::string str;
    std::getline(*file, str);
    return str;
}

std::string token_test_file (std::ifstream* file) {
    std::string str;
    char c;
    while (file->get(c)) {
        if (std::isalpha(c)) {
            str += c;
        }
    }
    return str;
}

std::string token_training_file(std::ifstream* file) {
    std::string str;
    char c;
    bool skipFirstLine = true;

    while (file->get(c)) {
        if (skipFirstLine) {
            if (c == '\n') {
                skipFirstLine = false;
            }
        } else {
            if (std::isalpha(c)) {
                str += c;
            }
        }
    }

    return str;
}
// DA RIMUOVERE
/* Alphabet alphabet; //global variable for the alphabet

Alphabet build_alphabet (std::string file) {
    
    for (char c : file) {
        if (std::isalpha(c)) {
            //std::cout << "Character " << c << " is alpha" << "and" << a << std::endl;
            alphabet.insert(c);
        }
    }
    //for (char letter : alphabet) {
        //std::cout << "Alphabet:" << letter << " \n";
    //}

    //std::cout << alphabet.size() << std::endl;
 
    return alphabet;
} */

Alphabet build_alphabet (Alphabet alphabet, std::string file) {
    
    for (char c : file) {
        if (std::isalpha(c)) {
            //std::cout << "Character " << c << " is alpha" << "and" << a << std::endl;
            alphabet.insert(c);
        }
    }
    /* for (char letter : alphabet) {
        std::cout << "Inserting in alphabet:" << letter << " \n";
    } */

    //std::cout << alphabet.size() << std::endl;
 
    return alphabet;
}

// function that maps each string relevant to the model to its appearance count
void build_model (int order, Model& model, std::string file) {
    //std::cout << "Building model" << std::endl;
    //std::string file_str = file;
    std::string file_substr = file.substr(0, order-1);
    //std::cout << file_substr << file_substr.length() << std::endl;
    file += file_substr;
    std::string substr;
    //std::cout << file << std::endl;
    
    for (size_t k = 0; k + order <= file.length(); k++) {
        substr = file.substr(k, order);
        model[substr]++;
    }

    /* for (const auto& entry : model) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    } */

    file = file.substr(0, file.length() - file_substr.length());
    //std::cout << file << std::endl;
}

void markov_model(Markov_model& markov_m, unsigned int order, const std::string& training_file) {

    //std::ifstream *file = open_training(training_file);
    /* for (size_t i = 0; i < order; i++) {
        std::string line;
        std::getline(*file, line);
        std::cout << line << std::endl;
    } */
    //std::string file_str = token_training_file(file);
    //std::cout << "File token " << file_str << std::endl;
    if (order == 0 || order > training_file.length()) {
        throw std::length_error("[MARKOV MODEL ERROR]: Incompatible input data and order size.");
    }
    markov_m.order = order;
// DA RIMUOVERE
/*  std::cout << "Order: " << markov_m.order << std::endl;
    for (char c : alphabet) {
        markov_m.alphabet.insert(c);
    } */

    //markov_m.alphabet = alphabet;
    markov_m.alphabet = build_alphabet(markov_m.alphabet, training_file);
    build_model(order, markov_m.model, training_file);
    build_model(order+1, markov_m.model, training_file);
/*     for (char letter : markov_m.alphabet) {
        std::cout << "Alphabet: " << letter << "\n";
    }

    for (const auto& entry : markov_m.model) {
        std::cout << "Model: " << entry.first << ": " << entry.second << std::endl;
    } */
    //close_file(file);

}

//function that, given a trained Markov model and an appropriate string, 
//returns the probability of occurrence of that string:
/* double laplace (const Markov_model& markov_m,const std::string& string) {

    if (string.length() != markov_m.order + 1) {
        throw std::length_error("[LAPLACE ERROR]: Input data is incompatible in size with the model.");
    }

    for (size_t i = 0; i < string.length(); i++) {
        if (markov_m.alphabet.find(string[i]) == markov_m.alphabet.end()) {
            throw std::domain_error("[LAPLACE ERROR]: String contains illegal characters.");
        }
    }

    double laplace;
    std::string str = string;
    //std::cout << "String: " << str << std::endl;
    std::string substr = str.substr(0, str.length() - 1);
    //std::cout << "Substr: " << substr << std::endl;
    Model markov_model_copy = markov_m.model;
    // for (const auto& entry : markov_model_copy) {
        //std::cout << "Copy Model: " << entry.first << ": " << entry.second << std::endl;
    //}

    double alphabet_size = markov_m.alphabet.size();
    int str_count;
    int substr_count;
    //std::cout << "Alphabet size: " << alphabet_size << std::endl;
    if (markov_model_copy.find(str) == markov_model_copy.end()) {
        str_count = 0;
    } else {
        str_count = markov_model_copy[str];
    }
    //std::cout << "String count: " << str_count << std::endl;
    if (markov_model_copy.find(substr) == markov_model_copy.end()) {
        substr_count = 0;
    } else {
        substr_count = markov_model_copy[substr];
    }
    // std::cout << "Substring: " << substr << std::endl;
    //std::cout << "Substring count: " << substr_count << std::endl;

    laplace = (str_count+1)/(substr_count + alphabet_size);
    //std::cout << "Laplace: " << laplace << std::endl;
    return laplace;
} */
double laplace (const Markov_model& markov_m,const std::string& str) {
    
    if (str.length() < markov_m.order) {
        throw std::length_error("[LAPLACE ERROR]: Incompatible input data and order size");
    }
/* 
    for (size_t i = 0; i < str.length(); i++) {
        if (markov_m.alphabet.find(str[i]) != markov_m.alphabet.end()) {
            throw std::domain_error("[LAPLACE ERROR]: String contains illegal characters.");
        }
    } */

    double res = 0.0;
    std::string substring = str.substr(0, str.length() - 1);
    auto iterString = markov_m.model.find(str);
    auto iterSubstring = markov_m.model.find(substring);
    //std::cout << "String: " << substring << iterSubstring->second << std::endl;

    if (iterString != markov_m.model.end()) {
        /* std::cout << "String: " << iterString->first << iterString->second << std::endl;
        std::cout << "Substring: " << iterSubstring->first << iterSubstring->second << std::endl; */
        res = (iterString->second + 1.0)/(iterSubstring->second + markov_m.alphabet.size());
    } else {
        //std::cout << "String: " << iterSubstring->first << iterSubstring->second << std::endl;
        res = (1.0)/(iterSubstring->second + markov_m.alphabet.size());
    }
    //std::cout << "Laplace: " << res << std::endl;
    return res;

}

/* double likelihood(Markov_model& markov_m,const std::string& input_data) {

    double res = 0;

    std::ifstream *file = open_test(input_data);
    std::string file_str = token_test_file(file);
    std::string substr;
    double tmp;

    for (int k = 0; k + markov_m.order <= file_str.length()-markov_m.order; k++) {
        substr = file_str.substr(k, markov_m.order+1);
        //std::cout << substr << " " << markov_m.order << std::endl;

        tmp = laplace(markov_m, substr);
        res += log(tmp);
        
    }

    std::cout << "likelihood: " << res << std::endl;
    return res;
    
} */
double likelihood(Markov_model& markov_m,const std::string& input_data) {

    double res = 0.0;
    std::string circ_input = input_data + input_data.substr(0, markov_m.order-1);
    std::string substr;
    /* std::cout << "Input length: " << input_data.length() << std::endl;
    std::cout << "Circ input: " << circ_input << " fine" << std::endl; */

    for (size_t k = 0; k + markov_m.order <= input_data.length(); k++) {
        substr = circ_input.substr(k, markov_m.order+1);
        res += std::log(laplace(markov_m, substr));
        //std::cout << "Substr: " << substr << std::endl;
        //std::cout << "Res: " << res << std::endl;
    }

    return res;
}