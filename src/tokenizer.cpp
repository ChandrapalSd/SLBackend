#include "tokenizer.hpp"
#include <iostream>
#include <cctype>
#include <iomanip>

namespace sl
{

    const std::unordered_map<std::string, TokenType> reservedKeywords = {
        {"int", TokenType::INT},
        {"if", TokenType::IF},
    };

    const std::unordered_map<TokenType, std::string> tokenTypeName = {
        {TokenType::NUMBER, "NUMBER"},
        {TokenType::IDENTIFIER, "IDENTIFIER"},
        {TokenType::INT, "INT"},
        {TokenType::IF, "IF"},
        {TokenType::BinaryOperator, "BinaryOperator"},
        {TokenType::ASSIGN, "ASSIGN"},
        {TokenType::OpenParen, "OpenParen"},
        {TokenType::CloseParen, "CloseParen"},
        {TokenType::OPENCURLY, "OPENCURLY"},
        {TokenType::CLOSEDCURLY, "CLOSEDCURLY"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::TEOF, "TEOF"},
    };

    Token::Token(TokenType type, std::string text)
        : type(type), text(text)
    {
    }
    Token::Token(TokenType type, const char text)
        : type(type), text(1, text)
    {
    }

    void print(Tokens tokens)
    {
        for (const Token &token : tokens)
        {
            std::cout << token << "\n";
        }
    }

    std::ostream &operator<<(std::ostream &os, const Token &t)
    {
        //return os << std::left << std::setw(20) << "[ " << tokenTypeName.at(t.type) << " " << t.text << " ]";
        return os << "[ " << tokenTypeName.at(t.type) << " " << t.text << " ]";
    }

    Tokens tokenize(const std::string &src)
    {
        Tokens tokens;
        std::string::const_iterator it = src.cbegin();
        uint32_t lineCount = 1;
        while (it != src.cend())
        {
            if (*it == '\n')
                lineCount++;
            // std::cout << *it << std::endl;

            if (*it == '(')
            {
                tokens.emplace_back(TokenType::OpenParen, *it++);
            }
            else if (*it == ')')
            {
                tokens.emplace_back(TokenType::CloseParen, *it++);
            }
            else if (*it == '{')
            {
                tokens.emplace_back(TokenType::OPENCURLY, *it++);
            }
            else if (*it == '}')
            {
                tokens.emplace_back(TokenType::CLOSEDCURLY, *it++);
            }
            // comments
            else if (*it == '/' && *(it + 1) == '/')
            {
                // std::cout << "Comment st \n";
                while (it != src.cend() && *it != '\n')
                    it++;
            }
            else if (std::isspace(*it))
            {
                it++;
            }
            // * and / are not supported
            else if (*it == '+' || *it == '-')
            {
                tokens.emplace_back(TokenType::BinaryOperator, *it++);
            }
            else if (*it == ';')
            {
                tokens.emplace_back(TokenType::SEMICOLON, *it++);
            }
            else if (*it == '=')
            {
                if (*(it + 1) == '=')
                {
                    tokens.emplace_back(TokenType::BinaryOperator, std::string(it, it + 2));
                    it += 2;
                }
                else
                    tokens.emplace_back(TokenType::ASSIGN, *it++);
            }
            // Handle numeric literals -> Integers
            else if (isdigit(*it))
            {
                std::string num = "";
                while (it != src.cend() && isdigit(*it))
                {
                    num += *it++;
                }

                // append new numeric token.
                tokens.emplace_back(TokenType::NUMBER, num);
            } // Handle Identifier & Keyword Tokens.
            else if (isalpha(*it))
            {
                // std::cout << "alpha st \n";
                std::string ident = "";
                while (it != src.cend() && isalpha(*it))
                {
                    ident += *it++;
                }
                // std::cout << ident << "\n";

                // CHECK FOR RESERVED KEYWORDS
                auto reserved = reservedKeywords.find(ident);
                // If value is not undefined then the identifier is
                // reconized keyword
                if (reserved != reservedKeywords.end())
                {
                    tokens.emplace_back((*reserved).second, ident);
                }
                else
                {
                    // Unreconized name must mean user defined symbol.
                    tokens.emplace_back(TokenType::IDENTIFIER, ident);
                }
            }
            // Handle unreconized characters.
            // TODO: Impliment better errors and error recovery.
            else
            {
                std::cerr << "Unreconized character found in source: `" << *it << "`. At line " << lineCount;
                exit(1);
            }
        }

        return tokens;
    }
}
