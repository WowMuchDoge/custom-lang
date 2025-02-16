#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>

#include "Constants.hpp"
#include "compiler/Parser.hpp"
#include "interpreter/StatementVisitor.hpp"

std::string getExtension(std::string fileName) {
	// find_last_of() does not work for some reason

	int pos;

	for (pos = fileName.size() - 1; pos >= 0; pos--) {
		if (fileName[pos] == '.') break;
	}

	if (pos == -1) return "";

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

	auto start = std::chrono::high_resolution_clock::now();
    
	if (argc != 2) {
		std::cout << "Usage: custom-lang <filename>" << std::endl;
		return -1;
    }

	Parser parser(readFile(argv[1]));
	StmtPtr programBlock;

	try {
		programBlock = parser.GetAst();
	} catch (Error* e) {
		e->Print();
	}

	SymbolTable st;
	ExpressionVisitor ev{st};
	StatementVisitor sv{st, ev};

	// for (auto statement : statements) {
	// 	std::cout << statement->ToString() << std::endl;
	// 	statement->Accept(sv);
	// }
	
	std::cout << programBlock->ToString();
	programBlock->Accept(sv);

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	std::cout << "Completed in " << duration.count() << "ms." << std::endl;
}
