#include "iostream"
#include "fstream"
#include <cinttypes>
#include "ToBmp.h"


enum class ParseOutput {
    Error,
    LongOption,
    ShortOption
};

enum class OptionStatus {
    WrongOption,
    Error,
    LongOption,
    ShortOption
};

struct Options {
    std::fstream input_option;
    std::string output_option;
    int max_iter_option = 0;
    int freq_option = 0;
};

struct SeparatedData {
    std::string option;
    std::string value;
    bool flag = false;
};


bool CanBeNumMaxIter (const std::string& str, Options* arguments);

bool CanBeNumFreq (const std::string& str, Options* arguments);

OptionStatus InputOption(const std::string& argv_first, const std::string& argv_second, Options* arguments);

OptionStatus OutputOption(const std::string& argv_first, const std::string& argv_second, Options* arguments);

OptionStatus MaxIterOption(const std::string& argv_first, const std::string& argv_second, Options* arguments);

OptionStatus FreqOption(const std::string& argv_first, const std::string& argv_second, Options* arguments);

SeparatedData SplitLongArgument(const std::string& str);

OptionStatus CheckParsedInfo(const std::string& argv_first, const std::string& argv_second, Options* arguments);

ParseOutput ParseInfo(const std::string& argv_first, const std::string& argv_second, Options* arguments);

bool GetOptions(int argc, char** argv, Options* arguments);
