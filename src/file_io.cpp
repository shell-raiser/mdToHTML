#include <fstream>
#include <sstream>
#include <string>

bool read_text_file(const std::string& path, std::string& out_content, std::string& out_error) {
    std::ifstream input(path);
    if (!input.is_open()) {
        out_error = "Could not open input file: " + path;
        return false;
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();

    if (input.bad()) {
        out_error = "Error while reading input file: " + path;
        return false;
    }

    out_content = buffer.str();
    return true;
}

bool write_text_file(const std::string& path, const std::string& content, std::string& out_error) {
    std::ofstream output(path);
    if (!output.is_open()) {
        out_error = "Could not open output file: " + path;
        return false;
    }

    output << content;
    if (!output.good()) {
        out_error = "Error while writing output file: " + path;
        return false;
    }

    return true;
}
