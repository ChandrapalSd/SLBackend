#include "parser.hpp"
#include <cassert>
#include <format>

/*
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
                  << "Left : \n{ \n"
                  << &(e->left) << "}\n"
                  << "Right : \n{ \n"
                  << &(e->right) << "}\n"
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
*/

namespace sl
{

    Statement::Statement(Type type, std::span<Token> tokens)
        : type(type), tokens(tokens)
    {
    }

    Statement::Type Statement::getType(std::span<Token> tokens)
    {
        switch (tokens[0].type)
        {
        case TokenType::INT:
        {
            if (tokens.size() == 3 && tokens[1].type == TokenType::IDENTIFIER && tokens[2].type == TokenType::SEMICOLON)
                return Type::VarDec;
            std::cerr << "Invalid token at variable declation.\nExpected int [identifier];.\nActual : ";
            for (const Token& t : tokens)
                std::cerr << t.text << " ";
            assert(false);
        }
        case TokenType::IDENTIFIER:
        {
            if (tokens.size() >= 4 && tokens[1].type == TokenType::ASSIGN /*&& [Expression]*/ && tokens.back().type == TokenType::SEMICOLON)
                return Type::VarAssign;
            std::cerr << "Invalid token at variable assignment.\nExpected [identifier] = [expression];.\nActual : \n";
            for (const Token &t : tokens)
                std::cerr << t.text << " ";
            std::cerr << std::endl;
            assert(false);
        }
        case TokenType::IF:
        {
            // TODO(CP):
            std::cerr << "Not Implemented\n";
            assert(false);
        }
        default:
            std::cerr << "Invalid token.\nExpected int / if / [identifier].\nActual : " << tokens[0] << "\n";
            assert(false);
        }
        return (Type)NULL;
    }

    VarDecStatement::VarDecStatement(const std::string identName, std::span<Token> tokens)
        : Statement(Type::VarDec, tokens), identName(identName)
    {
    }

    std::string VarDecStatement::getAsm()
    {
        return std::format("{} = 0\n", identName);
    }
    
    std::string VarAssignStatement::getAsm()
    {
        
        return std::format("{}\nsta %{}\n", expr.getAsm(), identName);
    }
    
    std::string Expression::getAsm()
    {
        bool calcReq = false;
        std::string operation = "";
        for (const Token& t : tokens)
        {
            if(t.type == TokenType::NUMBER){
                resultedAsm += std::format("ldi {} {}", calcReq ? 'B' : 'A', t.text);
                if (resultedAsm.length() > 0) {
                    resultedAsm += "\n" + operation;
                    operation = "";
                }
            }
            else if(t.type == TokenType::IDENTIFIER){
                resultedAsm += std::format("mov {} M %{}", calcReq ? 'B' : 'A', t.text);
                if (resultedAsm.length() > 0) {
                    resultedAsm += "\n" + operation;
                    operation = "";
                }
            }
            else if(t.type == TokenType::BinaryOperator){
                if(t.text == "=="){
                    operation = "call %eqeq";
                }
                else {
                    operation = t.text == "+" ? "add" : "sub";
                }
            }
            else
            {
                assert(false);
            }

            resultedAsm += '\n';
            calcReq = true;
        }

        return resultedAsm;
    }

    VarAssignStatement::VarAssignStatement(const std::string identName, std::span<Token> tokens, Expression expr)
        : Statement(Type::VarAssign, tokens), identName(identName), expr(expr)
    {
    }

    IfCondStatement::IfCondStatement(const uint32_t id, std::span<Token> tokens, Expression expr)
        : Statement(Type::IfCond, tokens), id(id), expr(expr)
    {
    }

    std::string IfCondStatement::getAsm()
    {
        std::string res;
        std::string strId = std::to_string(id);

        res += expr.getAsm() + '\n';
        res += std::string("ldi B 0")     + '\n';
        res += std::string("cmp")     + '\n';
        res += std::string("je %if_") + strId + '\n';

         return res;
    }

    IfBlockStatement::IfBlockStatement(const uint32_t id, std::span<Token> tokens, std::unique_ptr<Program> subProgram )
        : Statement(Type::IfBlock, tokens), id(id), subProgram(std::move(subProgram))//body(std::move(body))
    {
    }

    std::string IfBlockStatement::getAsm()
    {

        return
            subProgram->getAsm(true) + "\n"
            + "if_" + std::to_string(id) + ": \n";
    }

    Program::Program(std::span<Token> tokens, uint32_t ifCountStart)
        : tokens(tokens), ifCount(ifCountStart)
    {
    }

    void Program::parse()
    {
        if (mParsed)
            return;
        mParsed = true;
        auto tIt = tokens.begin(); // Token Iterator
        auto sIt = tIt;            // Start Statement Iterator
        while (tIt != tokens.end())
        {
            switch (tIt->type)
            {
            case TokenType::IF:
            {
                assert(tIt == sIt);
                ifCount++;
                tIt++;
                assert(tIt->type == TokenType::OpenParen);
                tIt++;
                sIt = tIt;
                while (tIt->type != TokenType::CloseParen)
                    tIt++;
                statements.push_back(std::make_unique<IfCondStatement>(ifCount, std::span(sIt-2, tIt+1), Expression(std::span(sIt, tIt))));
                
                tIt++;
                assert(tIt->type == TokenType::OPENCURLY);
                tIt++;
                sIt = tIt;
                while (tIt->type != TokenType::CLOSEDCURLY)
                    tIt++;

                //Program subProgram{ std::span(sIt, tIt), ifCount+1 };
                auto subProgram = std::make_unique<Program>(std::span(sIt, tIt), ifCount + 1);
                subProgram->parse();
                statements.push_back(std::make_unique<IfBlockStatement>(ifCount, std::span(sIt - 1, tIt + 1), std::move(subProgram)));

                sIt = tIt + 1;
            }
                break;
            case TokenType::SEMICOLON:
            {
                const auto type = Statement::getType(std::span(sIt, tIt+1));
                switch (type)
                {
                case Statement::Type::VarDec:
                {
                    statements.push_back(std::make_unique<VarDecStatement>((sIt+1)->text, std::span(sIt, tIt+1)));
                }
                break;
                case Statement::Type::VarAssign:
                {
                    // a = [expr] ;
                    // do not include identifier a, Assign operator = and semicolon ; in Expression constructor span
                    statements.push_back(std::make_unique<VarAssignStatement>(sIt->text, std::span(sIt, tIt+1), Expression(std::span(sIt+2, tIt ))));
                }
                break;
                default:
                    assert(false && "Invalid statement type");
                }
                sIt = tIt+1;
            }
            break;
            case TokenType::NUMBER:
            case TokenType::IDENTIFIER:
            case TokenType::INT:
            case TokenType::BinaryOperator:
            case TokenType::ASSIGN:
                break;
            default:
                std::cerr << "Invalid token type : " << *tIt << "\n";
                assert(false);
            }
            tIt++;
        }
    }

    std::string Program::getAsm(bool isSubProgram)
    {
        parse();
        print();

        std::string textSection;
        if(!isSubProgram)
            textSection += R"(
.text
start:

)";

        std::string dataSection;
        if(!isSubProgram)
            dataSection = ".data\n\n";

        for (const auto& s : statements)
        {
            switch(s->type)
            {
            case Statement::Type::VarDec:
                if(!isSubProgram)
                    dataSection += static_cast<VarDecStatement*>(s.get())->getAsm();
                break;
            case Statement::Type::VarAssign:
                textSection += static_cast<VarAssignStatement*>(s.get())->getAsm();
                break;
            case Statement::Type::IfCond:
                textSection += static_cast<IfCondStatement*>(s.get())->getAsm();
                break;
            case Statement::Type::IfBlock:
                textSection += static_cast<IfBlockStatement*>(s.get())->getAsm();
                break;
            }

        }

        if(!isSubProgram)
        textSection += R"(
hlt

eqeq:
    cmp
    je %eqeqeq
    ldi A 0
    ret
eqeqeq:
    ldi A 1
    ret
)";

        resultedAsm = textSection + dataSection;

        return resultedAsm;
    }

    void printStatements(std::span<std::unique_ptr<Statement>> statements)
    {
        for (const std::unique_ptr<Statement>& s : statements)
        {
            switch (s->type)
            {
            case Statement::Type::VarDec:
                std::cout << *static_cast<VarDecStatement*>(s.get());
                break;
            case Statement::Type::VarAssign:
                std::cout << *static_cast<VarAssignStatement*>(s.get());
                break;
            case Statement::Type::IfCond:
                std::cout << *static_cast<IfCondStatement*>(s.get());
                break;
            case Statement::Type::IfBlock:
                std::cout << *static_cast<IfBlockStatement*>(s.get());
                break;
            default:
                assert(false);
            }
            std::cout << '\n';
        }
    }

    void Program::print()
    {
        printStatements(std::span(statements));
    }

   /* std::vector<std::unique_ptr<Statement>>&& Program::moveOutStatements()
    {
        return std::move(statements);
    }*/

    Expression::Expression(std::span<Token> tokens)
        :Statement(Type::Expr, tokens)
    {
        assert(isValid(tokens) && "Invalid Expression");
    }


    bool Expression::isValid(std::span<Token> tokens)
    {
        std::vector<Token> stack;
        for (const Token& token : tokens)
        {
            switch (token.type)
            {
                case TokenType::IDENTIFIER:
                case TokenType::NUMBER:
                // TODO(CP) : Check if variable is defined
                {
                    if (stack.size() == 0 || stack.back().type == TokenType::BinaryOperator)
                    {
                        stack.push_back(token);
                    }
                    else
                    {
                        return false;
                    }
                }
                    break;
                case TokenType::BinaryOperator:
                {
                    if (stack.size() > 0 && (stack.back().type == TokenType::IDENTIFIER || stack.back().type == TokenType::NUMBER))
                    {
                        stack.push_back(token);
                    }
                    else
                    {
                        return false;
                    }
                }
                    break;
                default:
                    std::cerr << "Invalid token type: " << token << std::endl;
                    return false;
            }
        }
        return (stack.back().type == TokenType::IDENTIFIER || stack.back().type == TokenType::NUMBER);
    }

    bool Expression::isValid(std::span<Token> tokens, std::span<std::string> identifiers)
    {
        assert(false && "Not implemented"); // TODO(CP)
        return false;
    }

    std::ostream& operator<<(std::ostream& os, const VarDecStatement& s)
    {
        return os <<  "VarDecStatement [ " << s.identName << " ]";
    }

    std::ostream& operator<<(std::ostream& os, const VarAssignStatement& s)
    {
        return os <<  "VarAssignStatement [ " << s.identName << " = " << s.expr << " ]";
    }

    std::ostream& operator<<(std::ostream& os, const Expression& s)
    {
        os << "Expression { ";
        for (const Token& t : s.tokens)
        {
            os << t << " ";
        }
        return os << "}";
    }

    std::ostream& operator<<(std::ostream& os, const IfCondStatement& s)
    {
        return os << "IfCondStatement (" << s.id << ") [ " << s.expr << "]";
    }

    std::ostream& operator<<(std::ostream& os, const IfBlockStatement& s)
    {
        os << "IfBlockStatement (" << s.id <<") {\n";
        //printStatements(std::span(* const_cast<std::vector<std::unique_ptr<Statement>>*>(&s.body)));
        // TODO
        os << s.subProgram;
        return os << "\n}";
    }

} // namespace sl
