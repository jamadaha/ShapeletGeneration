#ifndef SHAPELETGENERATION_FILEHANDLER_H
#define SHAPELETGENERATION_FILEHANDLER_H

#include <string>
#include <optional>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include "Types.hpp"
#include "Shapelets.hpp"

namespace ShapeletGeneration {
    static std::vector<LabelledSeries> ReadCSV(const std::string& filePath, std::string delimiter = ",") {
        printf("---Reading Data---\n");
        std::ifstream file(filePath);
        std::string line;
        std::vector<LabelledSeries> dataPoints;
        int lineNum = 0;
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
            if (type.has_value())
                dataPoints.emplace_back(type.value(), series);
            else
                throw std::logic_error(&"Missing type on line " [lineNum]);

            lineNum++;
        }
        printf("Total Series: %zu\n", dataPoints.size());
        printf("---Finish Reading Data---\n");
        file.close();
        return dataPoints;
    }

    static void WriteFeatureFile(const std::string &filePath, const std::vector<double> &values) {
        std::ofstream out(filePath);
        for (const auto &point : values) {
            out << point << '\n';
        }
        out.close();
    }

    static void WriteSplitValuesToFile(const std::vector<LabelledSeries> &series, const std::vector<Split> &splits) {
        std::unordered_map<int, std::vector<Series>> mSeries;
        for (const auto &s : series)
            mSeries[s.label].push_back(s.series);
        if (std::filesystem::is_directory("out"))
            std::filesystem::remove_all("out");
        std::filesystem::create_directories("out");
        for (const auto &sPair : mSeries) {
            const auto tempPath = "out/" + std::to_string(sPair.first);
            std::filesystem::create_directories(tempPath);
            for (int i = 0; i < sPair.second.size(); i++) {
                std::vector<double> splitValues;
                for (const auto &split: splits)
                    splitValues.push_back(split.attribute->GenerateValue(sPair.second.at(i), (Window) split.shapelet));
                WriteFeatureFile(tempPath + "/" + std::to_string(i), splitValues);
            }
        }
    }
}

#endif //SHAPELETGENERATION_FILEHANDLER_H
