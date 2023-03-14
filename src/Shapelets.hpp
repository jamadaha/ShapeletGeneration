#ifndef SHAPELETGENERATION_SHAPELETS_HPP
#define SHAPELETGENERATION_SHAPELETS_HPP

#include <unordered_map>
#include <optional>
#include <functional>
#include <map>
#include "Types.hpp"
#include "InformationGain.hpp"
#include "SequenceMatching.hpp"

namespace ShapeletGeneration {
    static std::vector<std::pair<int, const Series*>> InterleavedSeries(std::pair<const std::vector<Series>*, const std::vector<Series>*> series) {
        std::vector<std::pair<int, const Series*>> interleavedSeries;
        for (uint i = 0; i < std::min(series.first->size(), series.second->size()); i++) {
            interleavedSeries.emplace_back(0, &series.first->at(i));
            interleavedSeries.emplace_back(1, &series.second->at(i));
        }

        if (series.first->size() > series.second->size()) {
            for (uint i = series.second->size(); i < series.first->size(); i++)
                interleavedSeries.emplace_back(0, &series.first->at(i));
        } else if (series.first->size() < series.second->size()) {
            for (uint i = series.first->size(); i < series.second->size(); i++)
                interleavedSeries.emplace_back(1, &series.second->at(i));
        }

        return interleavedSeries;
    }

    static double EvaluateWindow(const Window &window,
                                 std::pair<const std::vector<Series>*, const std::vector<Series>*> series,
                                 double bestScore) {
        const double priorEntropy = CalculateEntropy({ (int) series.first->size(), (int) series.second->size() });
        std::map<double, std::unordered_map<int, int>> matchFrequency;

        const std::vector<std::pair<int, const Series*>> interleavedSeries = InterleavedSeries(series);

        int diff[2] {  (int)series.first->size(), (int) series.second->size() };
        for (const auto &s : interleavedSeries) {
            matchFrequency[MatchFrequency(*s.second, window)][s.first]++;
            diff[s.first]--;

            for (uint i = 0; i < 2; i++) {
                for (int t = 0; t < 2; t++)
                    matchFrequency[1 - i][t] += diff[t];
                double optimalGain = CalculateInformationGain(matchFrequency, priorEntropy);
                for (int t = 0; t < 2; t++)
                    matchFrequency[1 - i][t] -= diff[t];
                if (optimalGain < bestScore)
                    return 0;
            }

        }

        return CalculateInformationGain(matchFrequency, priorEntropy);
    }

    static Shapelet GenerateShapelet(std::pair<const std::vector<Series>*, const std::vector<Series>*> series,
                                     std::pair<const std::vector<Window>*, const std::vector<Window>*> windows) {
        double bestEval = 0;
        std::optional<Shapelet> bestShapelet;

        for (const auto &w : { std::cref(windows.first), std::cref(windows.second) }) {
            for (const auto& window : *w.get()) {
                auto eval = EvaluateWindow(window, series, bestEval);
                assert(eval >= 0 && eval <= 1);

                if (!bestShapelet.has_value() || eval < bestEval) {
                    bestEval = eval;
                    bestShapelet = window;
                }
            }

        }

        assert(bestShapelet.has_value());
        return bestShapelet.value();
    }

    static std::vector<Shapelet> GenerateShapelets(const std::unordered_map<int, std::vector<Series>> &series,
                                                 const std::unordered_map<int, std::vector<Window>> &windows) {
        std::vector<Shapelet> shapelets;

        // All unique pairs of classes
        std::vector<std::pair<int, int>> pairs;
        for (auto iter = series.begin(); iter != series.end(); iter++)
            for (auto iter2 = std::next(iter, 1); iter2 != series.end(); iter2++)
                pairs.emplace_back((*iter).first, (*iter2).first);

        for (const auto &p : pairs)
            shapelets.push_back(GenerateShapelet(
                    std::make_pair(&series.at(p.first), &series.at(p.second)),
                    std::make_pair(&windows.at(p.first), &windows.at(p.second))
                    ));

        return shapelets;
    }
}

#endif //SHAPELETGENERATION_SHAPELETS_HPP
