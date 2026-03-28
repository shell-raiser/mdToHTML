#include <sstream>
#include <string>
#include <vector>

static std::string escape_html(const std::string& text) {
    std::string out;
    out.reserve(text.size());

    for (char c : text) {
        if (c == '&') out += "&amp;";
        else if (c == '<') out += "&lt;";
        else if (c == '>') out += "&gt;";
        else if (c == '"') out += "&quot;";
        else out += c;
    }

    return out;
}

static std::string render_inline_markdown(const std::string& text) {
    std::string html;
    std::size_t i = 0;

    while (i < text.size()) {
        if (i + 1 < text.size() && text[i] == '*' && text[i + 1] == '*') {
            std::size_t end = text.find("**", i + 2);
            if (end != std::string::npos) {
                html += "<strong>" + escape_html(text.substr(i + 2, end - (i + 2))) + "</strong>";
                i = end + 2;
                continue;
            }
        }

        if (text[i] == '*') {
            std::size_t end = text.find('*', i + 1);
            if (end != std::string::npos) {
                html += "<em>" + escape_html(text.substr(i + 1, end - (i + 1))) + "</em>";
                i = end + 1;
                continue;
            }
        }

        if (text[i] == '`') {
            std::size_t end = text.find('`', i + 1);
            if (end != std::string::npos) {
                html += "<code>" + escape_html(text.substr(i + 1, end - (i + 1))) + "</code>";
                i = end + 1;
                continue;
            }
        }

        if (text[i] == '[') {
            std::size_t mid = text.find(']', i + 1);
            std::size_t open = (mid == std::string::npos) ? std::string::npos : text.find('(', mid + 1);
            std::size_t close = (open == std::string::npos) ? std::string::npos : text.find(')', open + 1);
            if (mid != std::string::npos && open == mid + 1 && close != std::string::npos) {
                std::string link_text = escape_html(text.substr(i + 1, mid - (i + 1)));
                std::string url = escape_html(text.substr(open + 1, close - (open + 1)));
                html += "<a href=\"" + url + "\">" + link_text + "</a>";
                i = close + 1;
                continue;
            }
        }

        html += escape_html(std::string(1, text[i]));
        i++;
    }

    return html;
}

std::string render_html_document(const std::vector<AstNode>& nodes) {
    std::ostringstream out;

    out << "<!doctype html>\n";
    out << "<html>\n";
    out << "<head>\n";
    out << "  <meta charset=\"UTF-8\">\n";
    out << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    out << "  <title>Converted Markdown</title>\n";
    out << "</head>\n";
    out << "<body>\n";

    for (const AstNode& node : nodes) {
        if (node.type == AstNodeType::Heading) {
            out << "  <h" << node.heading_level << ">" << render_inline_markdown(node.text)
                << "</h" << node.heading_level << ">\n";
        } else if (node.type == AstNodeType::Paragraph) {
            out << "  <p>" << render_inline_markdown(node.text) << "</p>\n";
        } else if (node.type == AstNodeType::UnorderedList) {
            out << "  <ul>\n";
            for (const std::string& item : node.items) {
                out << "    <li>" << render_inline_markdown(item) << "</li>\n";
            }
            out << "  </ul>\n";
        } else if (node.type == AstNodeType::CodeBlock) {
            out << "  <pre><code>" << escape_html(node.text) << "</code></pre>\n";
        }
    }

    out << "</body>\n";
    out << "</html>\n";

    return out.str();
}
