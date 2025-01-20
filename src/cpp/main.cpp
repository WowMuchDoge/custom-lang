#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Constants.hpp"
#include "compiler/Parser.hpp"

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

	Parser parser(readFile(argv[1]));

	Expr* expr = parser.GetAst();

	expr->ToString();

	return 0;
}
