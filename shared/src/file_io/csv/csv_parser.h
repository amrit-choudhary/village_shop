#pragma once

#include "csv_data.h"

namespace ME {

class CSVParser {
   public:
    CSVParser();
    ~CSVParser();

    // Parses a CSV file.
    static void Parse(CSVData* csvData, const char* filePath);

   private:
};

}  // namespace ME
