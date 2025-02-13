#include "parser.hpp"

namespace sl
{

    Parser::Parser(const std::string &sourceCode)
        : tokens(tokenize(sourceCode)), tIt(tokens.cbegin())
    {
    }

    Program Parser::produceAST()
    {
        Program program;
        while (tIt != tokens.cend())
        {
            program.body.push_back(parseStatement());
        }
        return program;
    }

    std::unique_ptr<Statement> Parser::parseStatement()
    {
        return parseExpr();
    }

    std::unique_ptr<Expression> Parser::parseExpr()
    {
        return parseAdditiveExpr();
    }

    std::unique_ptr<Expression> Parser::parseAdditiveExpr()
    {
        auto left = parsePrimaryExpr();

        while (tIt != tokens.cend() && (tIt->text == "+" || tIt->text == "-"))
        {
            std::string operator_ = tIt->text;
            tIt++;
            auto right = parsePrimaryExpr();
            left = std::make_unique<BinaryExpression>(*left, *right, operator_);
        }

        return left;
    }

    std::unique_ptr<Expression> Parser::parsePrimaryExpr()
    {
        switch (tIt->type)
        {
        case TokenType::NUMBER:
        {
            std::string value = tIt->text;
            tIt++;
            return std::make_unique<NumericLiteral>(std::stoi(value));
        }
        case TokenType::IDENTIFIER:
        {

            std::string symbol = tIt->text;
            tIt++;
            return std::make_unique<Identifier>(symbol);
        }
        case TokenType::OpenParen:
        {
            tIt++;
            auto value = parseExpr();
            if (tIt == tokens.cend() || tIt->type != TokenType::CloseParen)
            {
                std::cerr << "No Closing Parenthesis : " << "\n";
                exit(1);
            }
            tIt++;
            return value;
        }
        default:
            std::cerr << "Unexpected toke : " << *tIt << "\n";
            exit(1);
            break;
        }
    }

#pragma region printing
    std::ostream &operator<<(std::ostream &os, const Program &p)
    {
        os << "---------- Program ----------\n";
        for (const auto& s : p.body)
        {
            os << s.get() << "\n";
        }
        os << "********** Program **********\n";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Statement *s)
    {
        switch (s->kind)
        {
        case NodeType::NumericLiteral:
            return os << *static_cast<const NumericLiteral *>(s);
        case NodeType::Identifier:
            return os << *static_cast<const Identifier *>(s);
        case NodeType::BinaryExpr:
            return os << static_cast<const BinaryExpression *>(s);
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Expression *s)
    {
        switch (s->kind)
        {
        case NodeType::NumericLiteral:
            return os << *static_cast<const NumericLiteral *>(s);
        case NodeType::Identifier:
            return os << *static_cast<const Identifier *>(s);
        case NodeType::BinaryExpr:
            return os << static_cast<const BinaryExpression *>(s);
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const BinaryExpression *e)
    {
        return os << "BinaryExpression : \n"
                  << "Operator : " << e->operator_ << "\n"
                  << "Left : { \n"
                  << &(e->left) << "\n       }"
                  << "Right : { \n"
                  << &(e->right) << "\n        }"
                  << '\n';
    }

    std::ostream &operator<<(std::ostream &os, const Identifier &i)
    {
        return os << "Identifier : " << i.symbol << '\n';
    }

    std::ostream &operator<<(std::ostream &os, const NumericLiteral &l)
    {
        return os << "NumericLiteral : " << l.value << '\n';
    }

#pragma endregion printing
}
