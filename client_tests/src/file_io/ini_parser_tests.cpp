/**
 * Tests for the INI parser. Parses in-memory text, so no files or resource paths are needed.
 */

#include "shared/src/file_io/ini/ini_parser.h"
#include "test_framework/src/test_framework.h"

TEST(INIParser, SectionsAndKeys) {
    ME::INIMap iniMap = ME::INIParser::Parse(
        "[logging]\n"
        "level=info\n"
        "[network]\n"
        "port=9310\n");

    EXPECT(iniMap["logging"]["level"] == "info");
    EXPECT(iniMap["network"]["port"] == "9310");
}

TEST(INIParser, MissingKeyIsEmpty) {
    ME::INIMap iniMap = ME::INIParser::Parse("[logging]\nlevel=info\n");

    EXPECT(iniMap["logging"]["missing"].empty());
    EXPECT(iniMap["missing"]["level"].empty());
}
