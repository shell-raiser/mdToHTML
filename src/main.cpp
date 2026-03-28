#include <iostream>
#include <string>
#include <vector>

// For a C-style learning flow, we keep each module in its own .cpp file
// and include them directly here so declarations + implementation stay together.
#include "file_io.cpp"
#include "lexer.cpp"
#include "parser.cpp"
#include "renderer.cpp"

static std::string default_output_path_from_input(const std::string& input_path) {
    std::size_t last_slash = input_path.find_last_of("/\\");
    std::size_t last_dot = input_path.find_last_of('.');

    if (last_dot == std::string::npos || (last_slash != std::string::npos && last_dot < last_slash)) {
        return input_path + ".html";
    }

    return input_path.substr(0, last_dot) + ".html";
}

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: mdtohtml <input.md> [output.html]\n";
        return 1;
    }

    const std::string input_path = argv[1];
    const std::string output_path = (argc == 3) ? argv[2] : default_output_path_from_input(input_path);

    std::string markdown;
    std::string error;

    if (!read_text_file(input_path, markdown, error)) {
        std::cerr << error << "\n";
        return 1;
    }

    std::vector<BlockToken> tokens = tokenize_markdown(markdown);
    std::vector<AstNode> nodes = parse_tokens(tokens);
    std::string html = render_html_document(nodes);

    if (!write_text_file(output_path, html, error)) {
        std::cerr << error << "\n";
        return 1;
    }

    std::cout << "Converted markdown file '" << input_path << "' to HTML file '" << output_path << "'.\n";
    return 0;
}
