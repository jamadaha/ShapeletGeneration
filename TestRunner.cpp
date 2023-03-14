#include <unordered_map>
#include "src/Shapelets.hpp"

using namespace ShapeletGeneration;

int main(int, char* argv[]) {
    auto shapelets = GenerateShapelets("data/UCRArchive_2018/SwedishLeaf/SwedishLeaf_TRAIN.tsv",
                                       '\t',2, 8);
    return 0;
}