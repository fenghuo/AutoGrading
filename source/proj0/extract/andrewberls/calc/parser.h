#ifndef PARSER_H
#define PARSER_H

#include <sstream>

using namespace std;

/*** Parser Class ***********************************************/

// The parser_t class handles everything.  It has an instance of scanner_t
// so it can peek at tokens and eat them up.  It also has access to the
// parsetree_t class so it can print out the parse tree as it figures it out.
// To make the recursive decent parser work, you will have to add some
// methods to this class.  The helper functions are described below
//
class parser_t {
    private:
    scanner_t scanner;
    parsetree_t parsetree;
    void eat_token(token_type t);
    void syntax_error(nonterm_type);

    // Code buffer that is built-up and flushed to cerr
    string linebuf;

    // Append partially-formed code to the line buffer
    void append(string s);
    void append(int i);

    // Emit a fully formed code line
    void emit();

    void Start();
    void Statements();
    void Statement();
    void Label();
    void Jump();
    void Assignment();
    void Print();
    void Expression();
    void EAddSub();
    void MemAccess();
    void Term();
    void TMultDiv();
    void Factor();
    void NumParen();
    void Exponen();

    public:
    parser_t();
    void parse();
};


// This function not only eats the token (moving the scanner forward one
// token), it also makes sure that token is drawn in the parse tree
// properly by calling push and pop.
void parser_t::eat_token(token_type t)
{
    parsetree.push(t);
    scanner.eat_token(t);
    parsetree.pop();
}


// Call this syntax error when you are trying to parse the
// non-terminal nt, but you fail to find a token that you need
// to make progress. You should call this as soon as you can know
// there is a syntax_error.
void parser_t::syntax_error(nonterm_type nt)
{
    printf("syntax error: found %s in parsing %s - line %d\n",
        token_to_string(scanner.next_token()),
        nonterm_to_string(nt),
        scanner.get_line() );
    exit(1);
}


// Constructor
parser_t::parser_t()
{
    linebuf = "";
}


// One the recursive decent parser is set up, you simply call parse()
// to parse the entire input, all of which can be derived from the start
// symbol
void parser_t::parse()
{
    append("#include <stdio.h>\n");
    append("#include <math.h>\n");
    append("int main(void) {\n");
    append("int m[101] = {};\n");
    emit();

    Start();

    append("return 0;\n");
    append("}\n\n");
    emit();
}


// Production procedures
// ---------------------------------
void parser_t::Start()
{
    parsetree.push(NT_Start);

    Statements();

    parsetree.pop();
}

void parser_t::Statements() {
    parsetree.push(NT_Statements);

    if (scanner.next_token() == T_eof) {
        parsetree.drawepsilon();
    } else {
        Statement();
        Statements();
    }

    parsetree.pop();
}

void parser_t::Statement() {
    parsetree.push(NT_Statement);

    switch (scanner.next_token()) {
        case T_label: Label(); break;
        case T_goto:  Jump(); break;
        case T_m:     Assignment(); break;
        case T_print: Print(); break;
        case T_eof:   break;
        default:      syntax_error(NT_Statement);
    }

    parsetree.pop();
}

void parser_t::Label() {
    parsetree.push(NT_Label);

    eat_token(T_label);
    eat_token(T_num);
    eat_token(T_colon);
    append("L");
    append(scanner.get_value());
    append(":\n");

    parsetree.pop();
}

void parser_t::Jump() {
    parsetree.push(NT_Jump);

    eat_token(T_goto);
    eat_token(T_num);

    int loc = scanner.get_value();

    if (scanner.next_token() == T_if) {
        eat_token(T_if);
        append("if (");
        Expression();
        append(")\n");
    }

    append("  goto ");
    append("L");
    append(loc);
    append(";\n");
    emit();

    parsetree.pop();
}

void parser_t::Assignment() {
    parsetree.push(NT_Assignment);

    MemAccess();
    eat_token(T_equals);
    append("=");
    Expression();
    append(";\n");
    emit();

    parsetree.pop();
}

void parser_t::Print() {
    parsetree.push(NT_Print);

    eat_token(T_print);
    eat_token(T_m);
    eat_token(T_opensquare);
    append("printf(\"%d\\n\", m[");
    Expression();
    append("]);\n");
    emit();
    eat_token(T_closesquare);

    parsetree.pop();
}

void parser_t::Expression() {
    parsetree.push(NT_Expression);

    Term();
    EAddSub();

    parsetree.pop();
}

void parser_t::EAddSub() {
    parsetree.push(NT_EAddSub);

    switch (scanner.next_token()) {
        case T_plus:
            eat_token(T_plus);
            append("+");
            Term();
            EAddSub();
            break;
        case T_minus:
            eat_token(T_minus);
            append("-");
            Term();
            EAddSub();
            break;
        default:
            parsetree.drawepsilon();
            break;
    }

    parsetree.pop();
}

void parser_t::Term() {
    parsetree.push(NT_Term);

   if (scanner.next_token() == T_m) {
        MemAccess();
    } else {
        Factor();
        TMultDiv();
    }

    parsetree.pop();
}

void parser_t::MemAccess() {
    parsetree.push(NT_MemAccess);

    eat_token(T_m);
    eat_token(T_opensquare);
    append("m[");
    Expression();
    append("]");
    eat_token(T_closesquare);

    parsetree.pop();
}

void parser_t::TMultDiv() {
    parsetree.push(NT_TMultDiv);

    switch (scanner.next_token()) {
        case T_times:
            eat_token(T_times);
            append("*");
            Factor();
            TMultDiv();
            break;
        case T_divide:
            eat_token(T_divide);
            append("/");
            Factor();
            TMultDiv();
            break;
        default:
            parsetree.drawepsilon();
            break;
    }

    parsetree.pop();
}

void parser_t::Factor() {
    parsetree.push(NT_Factor);

    NumParen();
    Exponen();

    parsetree.pop();
}

void parser_t::NumParen() {
    parsetree.push(NT_NumParen);

    switch (scanner.next_token()) {
        case T_openparen:
            eat_token(T_openparen);
            append("(");
            Expression();
            eat_token(T_closeparen);
            append(")");
            break;
        case T_num:
            eat_token(T_num);
            append(scanner.get_value());
            break;
        default:
            syntax_error(NT_NumParen);
            break;
    }

    parsetree.pop();
}

void parser_t::Exponen() {
    parsetree.push(NT_Exponen);

    if (scanner.next_token() == T_power) {
        eat_token(T_power);

        // We need to grab the last num off the line buffer
        // and insert it as the first argument to emitted pow()
        // We do this by collecting the number backwards into an
        // accumulator `right` and reversing it, assigning the previous
        // portions of the line buffer to `left`
        //
        // "You should just have built an AST", you say.
        // You're probably right.
        //
        string left  = "";
        string right = "";
        int left_len = linebuf.length();

        string::reverse_iterator rit;
        for (rit = linebuf.rbegin(); rit < linebuf.rend(); rit++) {
            char c = *rit;
            if (isdigit(c)) {
                right += c;
                left_len--;
            } else {
                break;
            }
        }
        right.assign(right.rbegin(), right.rend()); // Reverse the accumulator
        left    = linebuf.substr(0, left_len);      // Grab the remaining slice
        linebuf = left;
        append("pow(");
        append(right);
        append(",");
        emit();

        NumParen();
        Exponen();

        append(")");
    } else {
        parsetree.drawepsilon();
    }

    parsetree.pop();
}


// Append values to the line buffer
void parser_t::append(string s) {
    linebuf.append(s);
}

void parser_t:: append(int i) {
    stringstream ss; ss << i;
    linebuf.append(ss.str());
}

// Flush the line buffer to cerr and reset it
void parser_t::emit() {
    cerr << linebuf;
    linebuf.clear();
}

#endif
