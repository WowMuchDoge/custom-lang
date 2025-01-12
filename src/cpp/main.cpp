#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Constants.hpp"
#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"
#include "scanner/Scanner.hpp"

std::string getExtension(std::string fileName) {
    int pos = fileName.find(".");

    if (pos == std::string::npos) return "";

    return fileName.substr(pos, fileName.size() - pos);
}

std::string readFile(std::string filename) {
    std::stringstream fileContent;

    std::ifstream file;
    file.open(filename);

    std::string extension = getExtension(filename);

    if (!file.is_open()) {
        std::cout << "Could not open file '" << filename << "'." << std::endl;
        return "";
    } else if (extension != FileConstants::kExtension) {
        std::cout << "Invalid file extension '" << extension << "'." << std::endl;
        return "";
    }

    fileContent << file.rdbuf();
    return fileContent.str();
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: custom-lang <filename>" << std::endl;
        return -1;
    }

    std::string fileContent = readFile(argv[1]);

    std::cout << fileContent << std::endl;

    if (fileContent == "") return -1;

    Scanner scanner(fileContent);

    Token t(TokenType::UNKNOWN);

    while (true) {
        try {
            t = scanner.ScanToken();
            t.Print();
        } catch (Error* e) {
            e->Print();
        }

        if (t.GetType() == TokenType::END) break;
    }
}