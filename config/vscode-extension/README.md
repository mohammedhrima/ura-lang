# Ura Language Support for Visual Studio Code

Syntax highlighting and smart indentation support for the [Ura programming language](https://github.com/mohammedhrima/ura-lang) in Visual Studio Code.

## Features

- **Syntax Highlighting** - Full syntax highlighting for `.ura` files
- **Smart Indentation** - Automatic indentation based on language structure
- **Auto-Closing Pairs** - Automatic closing of brackets, quotes, and parentheses
- **Comment Support** - Line (`//`) and block (`/* */`) comment toggling
- **Custom File Icons** - Distinctive icons for `.ura` files

## Installation

### Local Installation (Current)

1. Clone or download this repository
2. Run the build script:
   ```bash
   ./build.sh
   ```
3. Restart VSCode completely (Cmd+Q on macOS, then reopen)
4. Open any `.ura` file

The extension will be installed to `~/.vscode/extensions/mohamedhrima.ura-lang-0.0.1/`

### Marketplace Installation (Coming Soon)

The extension will be available on the Visual Studio Code Marketplace.

## Language Overview

Ura is a statically-typed, compiled programming language that compiles to LLVM IR. It features Python-like indentation syntax with C-style semantics.

### Syntax Examples

#### Variables and Types
```ura
main():
    a int = 10
    name chars = "Hello"
    flag bool = True
    c char = 'x'
```

#### Functions
```ura
fn add(a int, b int) int:
    return a + b

main():
    result int = add(5, 3)
    return result
```

#### Control Flow
```ura
main():
    a int = 5
    if a < 10:
        return 1
    elif a < 20:
        return 2
    else:
        return 3
```

#### While Loops
```ura
main():
    i int = 0
    while i < 10:
        i += 1
    return i
```

#### Structs
```ura
struct User:
    name chars
    age int
    active bool

main():
    user User
    user.age = 25
```

#### References
```ura
main():
    a int = 10
    b ref int = a    // b is a reference to a
    b = 20           // modifies a through the reference
    return a         // returns 20
```

#### C Interop
```ura
proto fn puts(str chars) int
proto fn malloc(size int) pointer

main():
    puts("Hello, World!")
    return 0
```

## Supported Language Features

### Data Types
- `int`, `long`, `short` - Integer types
- `float`, `double` - Floating point types
- `char`, `chars` - Character and string types
- `bool` - Boolean type (`True`, `False`)
- `void` - No return value
- `pointer` - Generic pointer type
- `ref` - Reference type modifier

### Keywords
- **Control Flow**: `if`, `elif`, `else`, `while`, `for`, `return`, `break`, `continue`
- **Logical**: `and`, `or`, `not`, `is`
- **Declarations**: `fn`, `struct`, `as`
- **Library**: `use`, `proto`, `typeof`

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`
- **Comparison**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Logical**: `&&`, `||`, `!`
- **Assignment**: `=`, `+=`, `-=`, `*=`, `/=`, `%=`

### Built-in Functions
- `stack()` - Stack allocation
- `output()` - Output function

### Comments
```ura
// Single line comment

/*
   Multi-line
   comment
*/
```

## Editor Configuration

The extension automatically configures the following settings for `.ura` files:

- **Tab Size**: 4 spaces
- **Insert Spaces**: Enabled
- **Auto Indent**: Advanced
- **Trim Auto Whitespace**: Enabled

You can override these in your VSCode settings if needed.

## Syntax Highlighting

The extension provides highlighting for:

- Keywords and control flow statements
- Data types and type declarations
- Function definitions and calls
- Struct definitions
- String literals with escape sequences
- Printf-style format specifiers in strings
- Character literals
- Numeric literals (integers and floats)
- Comments (line and block)
- Operators
- Boolean constants (`True`, `False`, `NULL`)
- Built-in functions

## Development

### Building from Source

Prerequisites:
- Node.js and npm
- `@vscode/vsce` package (installed automatically by build script)

Build and install:
```bash
./build.sh
```

### Project Structure
```
.
├── package.json                 # Extension manifest
├── language-configuration.json  # Language configuration
├── syntaxes/
│   └── ura.tmLanguage.json     # TextMate grammar
├── icons/
│   ├── icon.png                # Extension icon
│   ├── icon.svg                # Extension icon (SVG)
│   └── file.json               # File icon theme
├── build.sh                    # Build and install script
└── README.md                   # This file
```

### Uninstalling

To remove the extension:
```bash
rm -rf ~/.vscode/extensions/mohamedhrima.ura-lang-*
```

Then restart VSCode.

## About Ura Language

Ura is a work-in-progress compiled programming language that compiles to LLVM IR. The compiler is written in C and uses LLVM for code generation and optimization.

**Key Features:**
- Statically typed
- Python-like indentation syntax
- Direct compilation to native code via LLVM
- C library interoperability
- References and pointers
- Module system

For more information about the Ura language:
- [Ura Compiler Repository](https://github.com/mohammedhrima/ura-lang)
- [Language Documentation](https://github.com/mohammedhrima/ura-lang#readme)

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

MIT License - see LICENSE file for details

## Author

**Hrima Mohammed**
- GitHub: [@mohamedhrima](https://github.com/mohammedhrima)

## Changelog

### 0.0.1 (Initial Release)
- Syntax highlighting for all Ura language constructs
- Smart indentation with Python-like block structure
- Auto-closing pairs for brackets, quotes, and parentheses
- Comment toggling support
- Custom file icons
- Editor configuration defaults

## Acknowledgments

- Inspired by Python and C language extensions
- Built with VSCode Extension API
- TextMate grammar for syntax highlighting
