#include <cstddef>
#include <string>
#include <vector>

enum class AstNodeType {
    Heading,
    Paragraph,
    UnorderedList,
    CodeBlock
};

struct AstNode {
    AstNodeType type;
    int heading_level;
    std::string text;
    std::vector<std::string> items;
};

std::vector<AstNode> parse_tokens(const std::vector<BlockToken>& tokens) {
    std::vector<AstNode> nodes;
    std::size_t i = 0;

    while (i < tokens.size()) {
        const BlockToken& token = tokens[i];

        if (token.type == BlockTokenType::BlankLine) {
            i++;
            continue;
        }

        if (token.type == BlockTokenType::Heading) {
            AstNode node;
            node.type = AstNodeType::Heading;
            node.heading_level = token.heading_level;
            node.text = token.text;
            nodes.push_back(node);
            i++;
            continue;
        }

        if (token.type == BlockTokenType::ListItem) {
            AstNode node;
            node.type = AstNodeType::UnorderedList;
            node.heading_level = 0;

            while (i < tokens.size() && tokens[i].type == BlockTokenType::ListItem) {
                node.items.push_back(tokens[i].text);
                i++;
            }

            nodes.push_back(node);
            continue;
        }

        if (token.type == BlockTokenType::CodeFence) {
            AstNode node;
            node.type = AstNodeType::CodeBlock;
            node.heading_level = 0;
            i++;

            while (i < tokens.size() && tokens[i].type != BlockTokenType::CodeFence) {
                node.text += tokens[i].text;
                if (i + 1 < tokens.size() && tokens[i + 1].type != BlockTokenType::CodeFence) {
                    node.text += "\n";
                }
                i++;
            }

            if (i < tokens.size() && tokens[i].type == BlockTokenType::CodeFence) {
                i++;
            }

            nodes.push_back(node);
            continue;
        }

        if (token.type == BlockTokenType::ParagraphLine) {
            AstNode node;
            node.type = AstNodeType::Paragraph;
            node.heading_level = 0;

            while (i < tokens.size() && tokens[i].type == BlockTokenType::ParagraphLine) {
                if (!node.text.empty()) {
                    node.text += " ";
                }
                node.text += tokens[i].text;
                i++;
            }

            nodes.push_back(node);
            continue;
        }

        i++;
    }

    return nodes;
}
