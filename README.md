# mdToHTML (C++ project structured for C learners)

This project converts a Markdown file into an HTML file with **minimal external dependencies** (only the C++ standard library).

To match a C-style learning flow, each module keeps declaration + implementation in the same `.cpp` file (no separate `.h/.hpp` files).

The code is still split by responsibility:

- **Lexical analysis** (`src/lexer.cpp`): turns markdown text into block-level tokens.
- **Syntax analysis / parsing** (`src/parser.cpp`): turns tokens into a simple AST-like structure.
- **Rendering** (`src/renderer.cpp`): converts parsed nodes into HTML.
- **File I/O** (`src/file_io.cpp`): handles reading/writing files.
- **Entry point** (`src/main.cpp`): includes module `.cpp` files and runs the pipeline.

## Supported Markdown (current scope)

- Headings (`#` to `######`)
- Paragraphs
- Unordered lists (`-`, `*`, `+`)
- Fenced code blocks (```)
- Inline formatting:
  - bold (`**text**`)
  - italic (`*text*`)
  - inline code (`` `code` ``)
  - links (`[text](url)`)

## Build

### Option 1: CMake

```bash
cmake -S . -B build
cmake --build build
```

Binary output:

```bash
./build/mdtohtml
```

### Option 2: Direct g++

```bash
g++ -std=c++17 src/main.cpp -o mdtohtml
```

## Run

```bash
./build/mdtohtml input.md [output.html]
```

or (if built with direct g++):

```bash
./mdtohtml input.md [output.html]
```

If `output.html` is omitted, the program uses the input filename with a `.html` extension.

```bash
./build/mdtohtml notes.md
# writes notes.html
```

## Example

Example markdown file (`examples/sample.md`):

```md
# Demo

A paragraph with **bold**, *italic*, and `inline code`.

- item 1
- item 2 with a [link](https://example.com)

```cpp
int x = 42;
```
```

Generate HTML:

```bash
./build/mdtohtml examples/sample.md examples/sample.html
```
