#pragma once

#include "csv_data.h"

namespace ME {

class CSVParser {
   public:
    CSVParser();
    ~CSVParser();

    /**
     * Parses a CSV file.
     * For tilemaps, set flipVertical to true to flip the rows vertically.
     * Because it is common for tilemaps data to have (0,0) at bottom-left.
     */
    static void Parse(CSVData* csvData, const char* filePath, const bool flipVertical = false);

   private:
};

}  // namespace ME
