#ifndef SHAPELETGENERATION_DATAREADER_H
#define SHAPELETGENERATION_DATAREADER_H

#include <string>
#include <optional>
#include <fstream>
#include <unordered_map>
#include "Types.hpp"

namespace ShapeletGeneration {
    static std::unordered_map<int, std::vector<Series>> ReadCSV(const std::string& filePath, std::string delimiter = ",") {
        printf("---Reading Data---\n");
        std::ifstream file(filePath);
        std::string line;
        std::unordered_map<int, std::vector<Series>> dataPoints;
        int lineNum = 0;
        uint totalSeries = 0;
        while (std::getline(file, line)) {
            // Read line
            std::optional<int> type;
            std::vector<double> series;
            while (!line.empty()) {
                std::string token;
                while (!line.empty() && line.substr(0, delimiter.size()) != delimiter) {
                    token += line[0];
                    line.erase(line.begin());
                }

                if (!type.has_value())
                    type = std::atoi(token.c_str());
                else
                    series.push_back(std::atof(token.c_str()));

                if (!line.empty() && line.substr(0, delimiter.size()) == delimiter)
                    line.erase(line.begin());
            }

            // Append if valid line
            if (type.has_value()) {
                dataPoints[type.value()].push_back(series);
                totalSeries++;
            } else
                throw std::logic_error(&"Missing type on line " [lineNum]);

            lineNum++;
        }
        printf("Total Series: %d\n", totalSeries);
        printf("---Finish Reading Data---\n");
        file.close();
        return dataPoints;
    }

    static void WriteCSV(const std::string &filePath, const std::vector<std::vector<double>> &data, std::string delimiter = ",") {
        std::ofstream out(filePath);
        for (const auto &series : data) {
            for (const auto &point : series) {
                out << point;
                if (point != series.at(series.size() - 1))
                    out << delimiter;
            }
            out << "\n";
        }
        out.close();
    }
}

#endif //SHAPELETGENERATION_DATAREADER_H
