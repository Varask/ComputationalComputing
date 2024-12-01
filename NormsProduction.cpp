#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include "Tools/Norms.cpp"

namespace fs = std::filesystem;

// Fonction pour lire la colonne "f" depuis un fichier CSV
std::vector<long double> readFColumn(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<long double> f_values;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filePath << std::endl;
        return f_values;
    }

    // Sauter la première ligne (entêtes)
    std::getline(file, line);

    // Lire chaque ligne et extraire la colonne "f"
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string x, t, f;
        std::getline(ss, x, ','); // Lire x
        std::getline(ss, t, ','); // Lire t
        std::getline(ss, f, ','); // Lire f
        f_values.push_back(std::stold(f));
    }

    return f_values;
}

// Fonction pour extraire le schéma, le type de set, les échantillons et tmax à partir du nom de fichier
void extractFileInfo(const std::string& fileName, std::string& scheme, std::string& setType, int& samples, int& tmax) {
    // Utilisation d'une expression régulière pour extraire les informations
    std::regex regexPattern(R"((\w+)_SET(\d+)_(\w+)_(\d+)_(\d+))");
    std::smatch match;

    if (std::regex_match(fileName, match, regexPattern) && match.size() == 6) {
        scheme = match[1];                   // Exemple : "E_FTBS"
        setType = match[3];                  // Exemple : "sign" ou "exp"
        samples = std::stoi(match[4]);       // Exemple : 100, 200, etc.
        tmax = std::stoi(match[5]);          // Exemple : 5, 10, etc.
    } else {
        scheme = "Unknown";
        setType = "Unknown";
        samples = 0;
        tmax = 0;
    }
}

// Fonction pour calculer les normes et les ajouter au fichier consolidé
void processCSV(const std::string& inputPath, std::ofstream& outputFile) {
    std::vector<long double> f_values = readFColumn(inputPath);
    if (f_values.empty()) {
        std::cerr << "Erreur : Pas de données trouvées dans le fichier " << inputPath << std::endl;
        return;
    }

    // Calcul des normes
    long double l1 = Norms::L1(f_values);
    long double l2 = Norms::L2(f_values);
    long double linf = Norms::LInf(f_values);
    long double lp = Norms::Lp(f_values, 2.5); // Exemple pour p = 2.5

    // Extraire le nom du fichier sans extension
    std::string fileName = fs::path(inputPath).stem().string();

    // Extraire les informations du fichier
    std::string scheme, setType;
    int samples, tmax;
    extractFileInfo(fileName, scheme, setType, samples, tmax);

    // Ajouter les résultats au fichier CSV consolidé
    outputFile << fileName << "," << scheme << "," << setType << "," << samples << "," << tmax << ","
               << l1 << "," << l2 << "," << linf << "," << lp << "\n";
}

// Fonction principale
int main() {
    fs::path inputFolder = "Results";
    fs::path outputFolder = inputFolder / "NormsResult";
    fs::path outputFilePath = outputFolder / "Norms.csv";

    if (!fs::exists(inputFolder)) {
        std::cerr << "Erreur : Le dossier d'entrée " << inputFolder << " n'existe pas." << std::endl;
        return 1;
    }

    // Créer le dossier "NormsResult" s'il n'existe pas
    if (!fs::exists(outputFolder)) {
        fs::create_directory(outputFolder);
    }

    // Ouvrir le fichier consolidé pour écriture
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Erreur : Impossible de créer le fichier " << outputFilePath << std::endl;
        return 1;
    }

    // Écrire les en-têtes
    outputFile << "FileName,Scheme,SetType,Samples,Tmax,L1,L2,LInf,Lp(p=2.5)\n";

    // Traiter chaque fichier CSV dans le dossier d'entrée
    for (const auto& entry : fs::directory_iterator(inputFolder)) {
        if (entry.path().extension() == ".csv" && entry.path().filename() != "Norms.csv") {
            std::cout << "Traitement du fichier : " << entry.path() << std::endl;
            processCSV(entry.path().string(), outputFile);
        }
    }

    outputFile.close();
    std::cout << "Les normes ont été consolidées dans : " << outputFilePath << std::endl;

    return 0;
}
