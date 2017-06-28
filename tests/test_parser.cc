#include "parser.h" 
#include <memory> 
#include <iostream>
#include <cassert>
using namespace std;

vector<string> tests = { 
    "let x = 1;",
    "let y = 1;",
    "return 1;" 
};

struct TestExpresion {
    string expr;
    string res;
    TestExpresion(const string& e, const string& r): expr(e), res(r) {}
}; 

vector<TestExpresion> testOperationPrecedenceSets  = { 
    { "-a * b", "((-a) * b)"},
    {"!-a", "(!(-a))"},
    {"a+b+c", "((a + b) + c)"},
    {"a+b-c", "((a + b) - c)"},
    {"a*b*c", "((a * b) * c)"},
    {"a*b/c", "((a * b) / c)"},
    {"a-b/c", "(a - (b / c))"},
    {"a+b*c+d/e-f", "(((a + (b * c)) + (d / e)) - f)"},
    {"3+4; -5 * 5", "(3 + 4)((-5) * 5)"},
    {"5>4==3<4", "((5 > 4) == (3 < 4))"},
    {"5>4!=3<4", "((5 > 4) != (3 < 4))"},
    {"3+4*5==3*1+4*5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
    {"1+(2+3)+4", "((1 + (2 + 3)) + 4)"},
    {"(2+3)*4", "((2 + 3) * 4)"},
    {"!(5+5)", "(!(5 + 5))"},
    {"!(true == false)", "(!(true == false))"},
    {"a+add(b*c)+d", "((a + add((b * c))) + d)"},
    {"add(a+b+c*d/f+g)", "add((((a + b) + ((c * d) / f)) + g))"},
};


struct TestPrefixExpression { 
    string input;
    string op;
    string value;
    TestPrefixExpression(const string& in, const string& o, const string& v):input(in), op(o), value(v) {}
};

vector<TestPrefixExpression> testPrefixExpressionSet = { 
    {"!5", "!", "5"},
    {"-51", "-", "51"},
    {"!true", "!", "true"},
    {"!false", "!", "false"},
};

struct TestIfExpression {
    string input;
    string if_token;
    string condtion;
    string consequence;
    string alternative; 
    TestIfExpression(const string& in, const string& if_tok, const string& cond, const string& cons, const string& alter=""):
        input(in), if_token(if_tok), condtion(cond), consequence(cons), alternative(alter) { }

    bool operator==(const TestIfExpression& o) { 
        return (input == o.input) && (if_token == o.if_token) 
            && (condtion == o.condtion) && (alternative == o.alternative);
    }
};

vector<TestIfExpression> testIfExpression {
    {"if(x < y)  { x }", "if", "(x < y)", "x", ""},
    {"if(x < y)  { x } else { y }", "if", "(x < y)", "x", "y"}, 
};




static void checkParseErros(Parser* p) { 
    static int i = 0;
    auto errors =  p->getErrors();

    i++;
    
    if (errors.empty()) { 
        cout << "test " << i << " are success" << endl;
        return;
    } else { 
        cout << "parser has " << errors.size() << " errors " << endl;

        for(const auto& e : errors) { 
            cout << e;
        }
    }

} 

struct TestLetStatement {
    string statement;
    string name;
    string val;
    TestLetStatement(const string& s, const string& n, const string& v):statement(s), name(n), val(v) { }
    bool operator==(const TestLetStatement& t) const {
        return t.statement == statement && t.name == name && t.val == val;
    }
};

vector<TestLetStatement> testsForLetStatement {
    {"let x = 1;", "x", "1"},
    {"let foo = bar;", "foo", "bar"},
    {"let bar = true;", "bar", "true"}, 
};

static void testLetStatement() { 
    cout << "Tests for Let statement " << endl;

    for(const auto& t : testsForLetStatement) { 
        unique_ptr<Parser> p(new Parser(t.statement)); 
        auto program = unique_ptr<Program>(p->parseProgram());
        checkParseErros(p.get());
        auto s = program->getStatements(); 
        assert(s->size() == 1); 
        LetStatement* ls = dynamic_cast<LetStatement*>(s->front().get());
        assert(ls != nullptr);
        TestLetStatement test { 
            t.statement,
            ls->getName(),
            ls->getValue() 
        };
        assert(test == t);
    }

}

static void testPrefixExpresion() { 
    cout << "Tests for PrefixExpression parsing " << endl;

    for(const auto& t : testPrefixExpressionSet) { 
        unique_ptr<Parser> p(new Parser(t.input)); 
        auto program = unique_ptr<Program>(p->parseProgram());
        checkParseErros(p.get());
        auto s = program->getStatements(); 
        assert(s->size() == 1); 
        ExpressionStatement* pes = dynamic_cast<ExpressionStatement*>(s->front().get());
        auto expr = pes->getExpression(); 
        PrefixExpression* pre_expr = dynamic_cast<PrefixExpression*>(expr); 
        assert(pre_expr->getOP() == t.op); 
        auto operand = pre_expr->getExpression()->toString();

        if(operand != t.value ) { 
           cout << "expect " << t.value  << ", get " << operand << "\n";
        }
    } 
}

static void testIfStatement() {
    cout << "Tests for If statement " << "\n";

    for(const auto& t : testIfExpression) {
       unique_ptr<Parser> p(new Parser(t.input)); 
       checkParseErros(p.get());
       auto program = unique_ptr<Program>(p->parseProgram()); 
       auto s = program->getStatements(); 
       assert(s->size() == 1);
       auto if_expr_statement = dynamic_cast<ExpressionStatement*>(s->front().get());
       assert(if_expr_statement != nullptr);
       IfExpression* if_expr = dynamic_cast<IfExpression*>(if_expr_statement->getExpression()); 
       
       BlockStatement* alter = if_expr->getAlternative(); 
       string alter_str = "";
       if(alter != nullptr) {
            alter_str = alter->toString();
       }

       TestIfExpression test_if_expr {
           t.input,
           if_expr->getToken().value,
           if_expr->getCondition()->toString(),
           if_expr->getConsequence()->toString(), 
           alter_str 
       };
       assert(test_if_expr == t); 
   }
} 


static void testOperationPrecedence() {
    cout << "Tests for Operator Precedence" << "\n";

    int i = 0; 
    for(const auto& t : testOperationPrecedenceSets) {
       unique_ptr<Parser> p(new Parser(t.expr)); 
       auto program = unique_ptr<Program>(p->parseProgram()); 
       auto res = program->toString();

       if (res != t.res) { 
           cout << "expected = " << t.res << ",  got = " << res << "\n"; 
           i++;
       }
       
    } 

    if(i) {
        cout << i << " Tests are failed " << endl; 
    } else {
        cout << "All Operator Precedence tests are passed " << endl;
    } 
}


struct TestParamParsing {
    string input;
    vector<string> params;
    TestParamParsing(const string& in, const vector<string>& p): input(in), params(p) { }
    bool operator==(const TestParamParsing& o) const  { 
        return o.input == input && params == o.params;
        
    }
};

vector<TestParamParsing> testsForParams = {
    {"fn() {}", {}},
    {"fn(x) {}", {"x"}},
    {"fn(x, y, z) {}", {"x", "y", "z"}},

};

static void testFuncParamParsing() { 
    cout << "Tests for func param parsing " << endl; 

    for(const auto& t : testsForParams) {
       unique_ptr<Parser> p(new Parser(t.input)); 
       auto program = unique_ptr<Program>(p->parseProgram());
       checkParseErros(p.get());
       auto s = program->getStatements(); 
       assert(s->size() == 1);
       auto func_expr_statement = dynamic_cast<ExpressionStatement*>(s->front().get());
       assert(func_expr_statement != nullptr);
       FunctionLiteral* func = dynamic_cast<FunctionLiteral*>(func_expr_statement->getExpression()); 
       assert(func != nullptr);
       const vector<unique_ptr<IdentifierNode>>& param = func->getParam(); 

       vector<string> param_str{};
       for(auto i = 0; i < param.size(); i++) { 
           param_str.push_back(param[i]->toString());
       }
       TestParamParsing got{t.input, param_str};
       assert(t == got);

    } 
}

int main() { 
    cout << "Tests for statement parsing " << endl;

    for(const auto& t : tests) {
       unique_ptr<Parser> p(new Parser(t)); 
       auto program = unique_ptr<Program>(p->parseProgram());
       checkParseErros(p.get());
    } 
    testPrefixExpresion();
    testOperationPrecedence();
    testIfStatement();
    testFuncParamParsing();
    testLetStatement();
    return 0;
}
