// ...existing code...
#include "csv_parser.h"

#include <src/misc/utils.h>

#include <sstream>
#include <vector>

ME::CSVParser::CSVParser() {}

ME::CSVParser::~CSVParser() {}

static inline std::string Trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

void ME::CSVParser::Parse(CSVData* csvData, const char* filePath, const bool flipVertical) {
    csvData->ClearData();
    std::string fileName = ME::GetResourcesPath() + filePath;
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    std::vector<std::vector<uint32_t>> rows;
    std::string line;
    size_t maxCols = 0;

    while (std::getline(file, line)) {
        // skip empty lines
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        std::vector<uint32_t> rowValues;
        while (std::getline(ss, token, ',')) {
            std::string t = Trim(token);
            if (t.empty()) {
                rowValues.push_back(0);
                continue;
            }
            try {
                uint32_t value = static_cast<uint32_t>(std::stoul(t));
                rowValues.push_back(value);
            } catch (...) {
                // non-numeric token -> treat as 0
                rowValues.push_back(0);
            }
        }

        if (!rowValues.empty()) {
            maxCols = std::max(maxCols, rowValues.size());
            rows.push_back(std::move(rowValues));
        }
    }

    file.close();

    // populate csvData in row-major order
    size_t rowCount = rows.size();
    size_t colCount = maxCols;
    if (rowCount == 0 || colCount == 0) {
        // nothing to do
        return;
    }

    csvData->SetSize(rowCount, colCount);

    if (flipVertical) {
        for (size_t r = 0; r < rowCount; ++r) {
            const auto& rv = rows[rowCount - 1 - r];
            for (size_t c = 0; c < rv.size(); ++c) {
                csvData->SetValue(r, c, rv[c]);
            }
        }
    } else {
        for (size_t r = 0; r < rowCount; ++r) {
            const auto& rv = rows[r];
            for (size_t c = 0; c < rv.size(); ++c) {
                csvData->SetValue(r, c, rv[c]);
            }
        }
    }
}
