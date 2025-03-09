#pragma once
#include "tokenizer.hpp"
#include <memory>
#include <span>
#include <expected>

/*
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

*/

namespace sl
{
    class Program;

    class Statement
    {
    public:
        enum class Type
        {
            VarDec,
            VarAssign,
            IfCond,
            IfBlock,
            Expr
        };

    public:
        Type type;
        std::span<Token> tokens;
        std::string resultedAsm;

    public:
        Statement(Type type, std::span<Token> tokens);
        static Type getType(std::span<Token> tokens);
    };

    class Expression : public Statement
    {
    public:
        Expression(std::span<Token> tokens);
        static bool isValid(std::span<Token> tokens);
        static bool isValid(std::span<Token> tokens, std::span<std::string> identifiers);
        std::string getAsm();
    };

    class VarDecStatement : public Statement
    {
    public:
        const std::string identName;

    public:
        VarDecStatement(const std::string identName, std::span<Token> tokens);
        std::string getAsm();
    };

    class VarAssignStatement : public Statement
    {
    public:
        const std::string identName;
        Expression expr;

    public:
        VarAssignStatement(const std::string identName, std::span<Token> tokens, Expression expr);
        std::string getAsm();
    };

    class IfCondStatement : public Statement
    {
    public:
        const uint32_t id;
        Expression expr;

    public:
        IfCondStatement(const uint32_t id, std::span<Token> tokens, Expression expr);
        std::string getAsm();
    };

    class IfBlockStatement : public Statement
    {
    public:
        const uint32_t id;
        //std::vector< std::unique_ptr<Statement>> body;
        std::unique_ptr<Program> subProgram;

    public:
        IfBlockStatement(const uint32_t id, std::span<Token> tokens, std::unique_ptr<Program> subProgram /*std::vector< std::unique_ptr<Statement>>&& body*/);
        std::string getAsm();
    };

    class Program
    {
    private:
        std::span<Token> tokens;
        std::vector<std::unique_ptr<Statement>> statements;
        uint32_t identCount = 0;
        uint32_t ifCount = 0;
        std::string resultedAsm;
        bool mParsed = false;
    private:
        void parse();

    public:
        Program(std::span<Token> tokens, uint32_t ifCountStart = 0);
        std::string getAsm(bool isSubProgram = false);
        void print();
        //std::vector<std::unique_ptr<Statement>>&& moveOutStatements();
    };


    std::ostream& operator<<(std::ostream& os, const VarDecStatement& s);
    std::ostream& operator<<(std::ostream& os, const VarAssignStatement& s);
    std::ostream& operator<<(std::ostream& os, const Expression& s);
    std::ostream& operator<<(std::ostream& os, const IfCondStatement& s);
    std::ostream& operator<<(std::ostream& os, const IfBlockStatement& s);
} // namespace sl
