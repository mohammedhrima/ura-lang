#include <string>
#include <vector>
#include <format>
#include <print>
#include <string_view>
#include <fstream>
#include <map>
#include <cstring>
#include <memory>
#include "lib.cpp"
// #include <map>

namespace ura
{
enum class Type
{
    none,
    head,
    eof,
    id,
    fdec,
    i32,

    dots,
    
    lpar, rpar,

    plus, minus, mul, div,
};

class Token
{
public:
    ura::Type type = ura::Type::none;
    bool is_dec = false;
    std::string name;
    struct { long value = 0; } i32;

    Token();
    Token(Type type);
    Token(std::string value);
    Token(long value);
};

std::vector<Token> tokens;

class Node
{
public:
    Token &token;
    Node(Token &token);
};

}

namespace std {
template <>
struct formatter<ura::Type> : formatter<string_view> {
    auto format(ura::Type type, format_context &ctx) const {
        string_view res = "UNKNOWN";
        switch(type) {
            case ura::Type::eof: res = "eof";   break;
            case ura::Type::id:  res = "id";    break;
            case ura::Type::fdec:res = "fdec";  break;
            case ura::Type::i32: res = "i32";   break;
            case ura::Type::none: res = "none"; break;
            case ura::Type::lpar: res = "("; break;
            case ura::Type::rpar: res = ")"; break;
            case ura::Type::dots: res = ":"; break;
            case ura::Type::plus: res = "plus"; break;
            case ura::Type::minus: res = "minus"; break;
            case ura::Type::mul: res = "mul"; break;
            case ura::Type::div: res = "div"; break;
            case ura::Type::head: res = "head"; break;
        };
        return formatter<string_view>::format(res, ctx);
    }
};

template <>
struct formatter<ura::Token> : formatter<string_view> {
    auto format(const ura::Token &token, format_context &ctx) const {
        std::string res;
        switch(token.type) {
            case ura::Type::id: {
                res = std::format("{} name {}", token.type, token.name);
                break;
            }
            case ura::Type::i32: {
                if (token.is_dec) res = std::format("{} is_dec ", token.type);
                else res = std::format("{} value {}", token.type, token.i32.value);
                break;
            }
            default: {
                res = std::format("{}", token.type); 
                break;
            }
        }
        return formatter<string_view>::format(res, ctx);
    }
};

}

namespace ura
{
Token::Token() {};

Token::Token(Type type) : type(type) {
    std::println("new token {}", *this);
}

Token::Token(std::string value) {
    if (value == "fn") type = Type::fdec;
    else if (value == "i32")
    {
        is_dec = true;
        type = Type::i32;
    }
    else {
        type = Type::id;
        name = value;
    }
    std::println("new token {}", *this);
}

Token::Token(long value) {
    type = Type::i32;
    i32.value = value;
    std::println("new token {}", *this);
}

Node::Node(Token &token) : token(token) {
    std::println("new node {}", token);
}
}

namespace ura {

void tokenize(std::string content)
{
    std::println("tokenize '{}'", content);
    for(size_t e = 0; e < content.length();)
    {
        size_t s = e;
        if(std::isspace(content[e]))
        {
            e++;
            continue;
        }
        if(std::isalpha(content[e]) || content[e] == '_')
        {
            while(std::isalnum(content[e]) || content[e] == '_')
                e++;
            tokens.push_back(Token(content.substr(s, e - s)));
            continue;
        }
        while(std::isdigit(content[e]))
            e++;
        if(e != s) {
            tokens.push_back(Token(std::atol(content.data() + s)));
            continue;
        }

        if(std::strchr("():+-*/", content[e]))
        {
            std::map<char, ura::Type> specials = {
                {'(', ura::Type::lpar},
                {')', ura::Type::rpar},
                {':', ura::Type::dots},
                {'+', ura::Type::plus},
                {'-', ura::Type::minus},
                {'*', ura::Type::mul},
                {'/', ura::Type::div},
            };
            tokens.push_back(Token(specials[content[e]]));
            e++;
            continue;
        }
        
        e++;
    }
    tokens.push_back(Token(ura::Type::eof));
}

std::shared_ptr<Node> prime()
{
    // TODO: implement it
}

std::shared_ptr<Node> expr(int min_op)
{
    // TODO: implment it
}

void compile(std::vector<std::string> av)
{
    // TODO: check arguments
    File file(av[1]);
    if(file.is_failed())
        throw std::runtime_error("error openning file '" + std::string(av[1]) + "'");
    tokenize(file.content());

    Token token;
    std::shared_ptr<Node> head = std::make_shared<Node>(token);
    for(auto token : tokens)
    {
        if(token.type == Type::eof) break;

    }
};
}

/*
+ read file
+ tokenize 
*/
int main(int ac, char **av)
{
    try
    {
        ura::compile(std::vector<std::string>(av,  av + ac));
    }
    catch(std::exception &err)
    {
        std::println("{}", err.what());
    }
}