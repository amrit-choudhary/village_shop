#include "ini_parser.h"

#include "../../misc/utils.h"

std::string fileName = "config/settings.ini";

INIMap Load() {
    INIMap iniMap;
    fileName = ME::Utils::GetResourcesPath() + fileName;

    std::ifstream file(fileName);
    std::stringstream buffer;

    if (file.is_open()) {
        buffer << file.rdbuf();

        char ch = ' ';
        char lines[256][256];
        char lhs[256];
        char rhs[256];
        char *hs;
        uint8_t charCount = 0;
        uint8_t lineCount = 0;
        uint8_t varLineCharIndex = 0;
        uint8_t varLineVarIndex = 0;

        while (buffer.peek() != EOF && buffer.get(ch)) {
            // Separate in lines (not-comments).
            if (ch == '\n') {
                lines[lineCount][charCount] = '\0';

                if (charCount == 0) {
                    continue;
                } else if (lines[lineCount][0] == ';') {
                    charCount = 0;
                    continue;
                } else {
                    charCount = 0;
                    ++lineCount;
                }
            } else {
                lines[lineCount][charCount] = ch;
                ++charCount;
            }
        }

        // Read significant lines one by one.
        uint8_t categoryNumber = 0;
        std::map<std::string, std::string> categoryVal = {};
        std::string categoryName = "";
        for (int i = 0; i < lineCount; ++i) {
            // Category start
            if (lines[i][0] == '[') {
                char categoryNameFiltered[256];
                RemoveSpacesAndBrackets(lines[i], categoryNameFiltered);
                categoryName = categoryNameFiltered;
                categoryVal.clear();
                iniMap.insert({categoryName, categoryVal});
            } else {
                char *varLine = lines[i];

                hs = lhs;
                varLineCharIndex = 0;
                varLineVarIndex = 0;
                while (varLine[varLineCharIndex] != '\0') {
                    if (varLine[varLineCharIndex] == '=') {
                        lhs[varLineCharIndex] = '\0';
                        hs = rhs;
                        varLineVarIndex = -1;
                    } else {
                        hs[varLineVarIndex] = varLine[varLineCharIndex];
                    }

                    ++varLineVarIndex;
                    ++varLineCharIndex;
                }
                rhs[varLineVarIndex] = '\0';

                iniMap[categoryName][lhs] = rhs;
            }
        }

        if (buffer.eof()) {
            std::cout << "\nReached end of stream" << std::endl;
        } else if (buffer.fail()) {
            std::cout << "\nError occurred while reading" << std::endl;
        }

        file.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }

    return iniMap;
}

void RemoveSpacesAndBrackets(char *input, char *output) {
    uint8_t index = 0;
    uint8_t filterIndex = 0;
    while (input[index] != '\0') {
        if (!(input[index] == ' ' || input[index] == '[' || input[index] == ']')) {
            output[filterIndex] = input[index];
            ++filterIndex;
        }
        ++index;
    }
    output[filterIndex] = '\0';
}

void PrintINI(const INIMap &iniMap) {
    for (const auto &[k, v] : iniMap) {
        std::cout << k << '\n';
        for (const auto &[k2, v2] : v) {
            std::cout << "\t" << k2 << " = " << v2 << '\n';
        }
    }
}
