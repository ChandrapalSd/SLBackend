#pragma once
#include "tokenizer.hpp"
#include <memory>

namespace sl
{

    enum class NodeType
    {
        Program,
        NumericLiteral,
        Identifier,
        BinaryExpr
    };

    class Statement
    {
    public:
        NodeType kind;

    protected:
        Statement(NodeType kind)
            : kind(kind)
        {
        }
    };

    typedef std::vector<std::unique_ptr<Statement>> Statements;
    typedef int32_t Number;

    class Program : public Statement
    {
    public:
        Statements body;

    public:
        Program()
            : Statement(NodeType::Program)
        {
        }
    };

    class Expression : public Statement
    {
    public:
        Expression(NodeType kind)
            : Statement(kind)
        {
        }
    };

    class BinaryExpression : public Expression
    {
    public:
        std::string operator_;
        Expression left;
        Expression right;

    public:
        BinaryExpression(Expression left, Expression right, const std::string operator_)
            : Expression(NodeType::BinaryExpr), left(left), right(right), operator_(operator_)
        {
        }
    };

    class Identifier : public Expression
    {
    public:
        std::string symbol;

    public:
        Identifier(const std::string symbol)
            : Expression(NodeType::Identifier), symbol(symbol)
        {
        }
    };

    class NumericLiteral : public Expression
    {
    public:
        Number value;

    public:
        NumericLiteral(Number value)
            : Expression(NodeType::NumericLiteral), value(value)
        {
        }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Parser
    {
    private:
        Tokens tokens;
        Tokens::const_iterator tIt; // token iterator

    public:
        Parser(const std::string &sourceCode);
        Program produceAST();
        std::unique_ptr<Statement> parseStatement();
        std::unique_ptr<Expression> parseExpr();
        std::unique_ptr<Expression> parseAdditiveExpr();
        std::unique_ptr<Expression> parsePrimaryExpr();
    };

    std::ostream &operator<<(std::ostream &, const Statement *);
    std::ostream &operator<<(std::ostream &, const Program &);
    std::ostream &operator<<(std::ostream &, const Expression *);
    std::ostream &operator<<(std::ostream &, const BinaryExpression *);
    std::ostream &operator<<(std::ostream &, const Identifier &);
    std::ostream &operator<<(std::ostream &, const NumericLiteral &);

}