#include "ParseArgs.h"

bool CanBeNumMaxIter (const std::string& str, Options* arguments) {
    try {
        arguments->max_iter_option = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool CanBeNumFreq (const std::string& str, Options* arguments) {
    try {
        arguments->freq_option = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

OptionStatus InputOption(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    if (argv_first == "-i") {
        arguments->input_option.open(argv_second);
        if (arguments->input_option.fail()) {
            return OptionStatus::Error;
        }
        return OptionStatus::ShortOption;
    }
    if (argv_first == "--input") {
        arguments->input_option.open(argv_second);
        if (arguments->input_option.fail()) {
            return OptionStatus::Error;
        }
        return OptionStatus::LongOption;
    }
    return OptionStatus::WrongOption;
}

OptionStatus OutputOption(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    if (argv_first == "-o") {
        arguments->output_option = argv_second;
        return OptionStatus::ShortOption;
    }
    if (argv_first == "--output"){
        arguments->output_option = argv_second;
        return OptionStatus::LongOption;
    }
    return OptionStatus::WrongOption;
}

OptionStatus MaxIterOption(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    if (argv_first != "-m" && argv_first != "--max-iter"){
        return OptionStatus::WrongOption;
    }
    if (CanBeNumMaxIter(argv_second, arguments)){
        if (argv_first == "-m") {
            return OptionStatus::ShortOption;
        }
        if (argv_first == "--max-iter"){
            return OptionStatus::LongOption;
        }
    } else {
        return OptionStatus::Error;
    }
    return OptionStatus::WrongOption;
}

OptionStatus FreqOption(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    if (argv_first != "-f" && argv_first != "--freq"){
        return OptionStatus::WrongOption;
    }
    if (CanBeNumFreq(argv_second, arguments)){
        if (argv_first == "-f") {
            return OptionStatus::ShortOption;
        }
        if (argv_first == "--freq"){
            return OptionStatus::LongOption;
        }
    } else {
        return OptionStatus::Error;
    }
    return OptionStatus::WrongOption;
}

SeparatedData SplitLongArgument(const std::string& str) {
    SeparatedData result;
    bool flag = false;
    for (int i = 0; i<str.length(); ++i) {
        if (str[i] == '=') {
            if (!flag) {
                result.flag=true;
                flag = true;
                ++i;
            } else {
                result.flag = false;
                return result;
            }
        }
        if (flag) {
            result.value += str[i];
        } else {
            result.option += str[i];
        }
    }
    return result;
}

OptionStatus CheckParsedInfo(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    OptionStatus max_iter_check = MaxIterOption(argv_first, argv_second, arguments);
    OptionStatus freq_check = FreqOption(argv_first, argv_second, arguments);
    OptionStatus output_check = OutputOption(argv_first, argv_second, arguments);
    OptionStatus input_check = InputOption(argv_first, argv_second, arguments);
    if (max_iter_check == OptionStatus::Error || freq_check == OptionStatus::Error || output_check == OptionStatus::Error || input_check == OptionStatus::Error) {
        return OptionStatus::Error;
    }
    if (max_iter_check == OptionStatus::LongOption || freq_check == OptionStatus::LongOption || output_check == OptionStatus::LongOption || input_check == OptionStatus::LongOption) {
        return OptionStatus::LongOption;
    }
    if (max_iter_check == OptionStatus::ShortOption || freq_check == OptionStatus::ShortOption || output_check == OptionStatus::ShortOption || input_check == OptionStatus::ShortOption) {
        return OptionStatus::ShortOption;
    }
    return OptionStatus::WrongOption;
}

ParseOutput ParseInfo(const std::string& argv_first, const std::string& argv_second, Options* arguments) {
    switch (CheckParsedInfo(argv_first, argv_second, arguments)) {
        case OptionStatus::Error: return ParseOutput::Error;
        case OptionStatus::LongOption: return ParseOutput::LongOption;
        case OptionStatus::ShortOption: return ParseOutput::ShortOption;
        case OptionStatus::WrongOption: {
            SeparatedData result = SplitLongArgument(argv_first);
            OptionStatus temp;
            if (result.flag) {
                temp = CheckParsedInfo(result.option, result.value, arguments);
                switch (temp) {
                    case OptionStatus::Error: return ParseOutput::Error;
                    case OptionStatus::LongOption: return ParseOutput::LongOption;
                    case OptionStatus::ShortOption: return ParseOutput::ShortOption;
                }
            }
        }
    }
    return ParseOutput::Error;
}

bool GetOptions(int argc, char** argv, Options* arguments) {
    ParseOutput temp;
    for (int i = 1; i<argc; ++i) {
        if (i + 1 < argc)
        {
            temp = ParseInfo(argv[i], argv[i+1], arguments);
        } else {
            temp = ParseInfo(argv[i], "", arguments);
        }
        if (temp == ParseOutput::ShortOption) {
            ++i;
            continue;
        }
        else if (temp == ParseOutput::Error) {
            return false;
        }
    }
    return true;
}
