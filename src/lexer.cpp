#include <sstream>
#include <string>
#include <vector>

enum class BlockTokenType {
    Heading,
    ParagraphLine,
    ListItem,
    CodeFence,
    CodeLine,
    BlankLine
};

struct BlockToken {
    BlockTokenType type;
    int heading_level;
    std::string text;
};

static bool lexer_starts_with(const std::string& text, const std::string& prefix) {
    return text.rfind(prefix, 0) == 0;
}

static int lexer_count_heading_marks(const std::string& line) {
    int count = 0;
    while (count < static_cast<int>(line.size()) && line[count] == '#') {
        count++;
    }

    if (count == 0 || count > 6) {
        return 0;
    }

    if (count < static_cast<int>(line.size()) && line[count] == ' ') {
        return count;
    }

    return 0;
}

std::vector<BlockToken> tokenize_markdown(const std::string& markdown_text) {
    std::vector<BlockToken> tokens;
    std::istringstream input(markdown_text);
    std::string line;
    bool inside_code_block = false;

    while (std::getline(input, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (lexer_starts_with(line, "```")) {
            BlockToken token;
            token.type = BlockTokenType::CodeFence;
            token.heading_level = 0;
            token.text = line;
            tokens.push_back(token);
            inside_code_block = !inside_code_block;
            continue;
        }

        if (inside_code_block) {
            BlockToken token;
            token.type = BlockTokenType::CodeLine;
            token.heading_level = 0;
            token.text = line;
            tokens.push_back(token);
            continue;
        }

        if (line.empty()) {
            BlockToken token;
            token.type = BlockTokenType::BlankLine;
            token.heading_level = 0;
            token.text = "";
            tokens.push_back(token);
            continue;
        }

        int heading_level = lexer_count_heading_marks(line);
        if (heading_level > 0) {
            BlockToken token;
            token.type = BlockTokenType::Heading;
            token.heading_level = heading_level;
            token.text = line.substr(heading_level + 1);
            tokens.push_back(token);
            continue;
        }

        if (lexer_starts_with(line, "- ") || lexer_starts_with(line, "* ") || lexer_starts_with(line, "+ ")) {
            BlockToken token;
            token.type = BlockTokenType::ListItem;
            token.heading_level = 0;
            token.text = line.substr(2);
            tokens.push_back(token);
            continue;
        }

        BlockToken token;
        token.type = BlockTokenType::ParagraphLine;
        token.heading_level = 0;
        token.text = line;
        tokens.push_back(token);
    }

    return tokens;
}
