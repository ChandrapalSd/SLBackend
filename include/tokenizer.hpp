#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cstdint>

namespace sl
{

    // also update the print function if you modify this
    enum class TokenType
    {
        // Literal Types
        NUMBER,
        IDENTIFIER,

        // Keywords
        INT,
        IF,

        // Grouping * Operators
        BinaryOperator,
        ASSIGN,
        OpenParen,
        CloseParen,

        OPENCURLY,
        CLOSEDCURLY,
        SEMICOLON,
        TEOF,

        Count
    };

    extern const std::unordered_map<std::string, TokenType> reservedKeywords;
    extern const std::unordered_map<TokenType, std::string> tokenTypeName;

    class Token
    {
    public:
        TokenType type;
        std::string text;

    public:
        Token() = delete;
        Token(TokenType type, std::string text);
        Token(TokenType type, const char text);
    };

    typedef std::vector<Token> Tokens;

    Tokens tokenize(const std::string &src);
    void print(Tokens tokens);
    std::ostream &operator<<(std::ostream &, const Token &);

}
