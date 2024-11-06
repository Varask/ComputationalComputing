#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

class IOFileManager {
public:
    // Function to read data from a CSV file
    static std::vector<std::vector<double>> readCSV(const std::string& filename) {
        std::vector<std::vector<double>> data;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<double> row;
            std::stringstream ss(line);
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(std::stod(cell));
            }
            data.push_back(row);
        }
        file.close();
        return data;
    }

    // Function to write data to a CSV file
    static void writeCSV(const std::string& filename, const std::vector<std::vector<double>>& data) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }
        file.close();
    }

    // Function to print the data read from CSV
    static void printData(const std::vector<std::vector<double>>& data) {
        for (const auto& row : data) {
            for (double value : row) {
                std::cout << value << " ";
            }
            std::cout << "\n";
        }
    }
};
 