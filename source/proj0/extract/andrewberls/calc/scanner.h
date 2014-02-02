#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*** Scanner Class ***********************************************/

class scanner_t {
    public:

    // Eats the next token and prints an error if it is not of type c
    void eat_token(token_type c);

    // Peeks at the lookahead token
    token_type next_token();

    // Return the current numeric token value
    unsigned int get_value();

    // Return line number for errors
    int get_line();

    // Constructor
    scanner_t();

    private:

    // Error message and exit for unrecognized character
    void scan_error(char x);

    // Error message and exit for number out of range
    void range_error();

    // Error message and exit for token mismatch
    void mismatch_error(token_type c);

    // Raw program contents
    string buffer;

    // Current index in input stream
    int pos;

    // Last val read (may be garbage!)
    unsigned int value;

    // Current line number
    int lnum;

    // Move the input tpr forwards as necessary for a token and return it
    token_type advance(token_type t);

    // Read a keyword (print, goto, if)
    token_type tokenize_keyword();

    // Read a number
    token_type tokenize_num();

    // Read a single token from the stream and advance the input ptr
    token_type read_token();

    // The next token in the stream
    token_type next;
};


// Advance the input ptr as appropriate for a token and return it
// Note: multichar tokens such as keywords and nums advance themselves
token_type scanner_t::advance(token_type t) {
   pos += (t == T_power ? 2 : 1);
   return t;
}


// Read a number token and advance the input ptr
token_type scanner_t::tokenize_num() {
    unsigned int val = 0;
    char c = buffer[pos];
    while (c != '\0' && isdigit(c)) {
        val = val*10 + (c - '0');
        c = buffer[++pos];
    }
    if (val > (unsigned int) pow(2.0,31.0)-1) range_error();
    value = val;

    return T_num;
}


// Return the 'current' numeric token value
// May return a leftover/old value depending on when it is called!
unsigned int scanner_t::get_value()
{
    return value;
}


// Read a keyword token and advance the input ptr
token_type scanner_t::tokenize_keyword() {
    int start = pos;
    int len   = 0;

    char c = buffer[pos];
    while (c != '\0' && isalpha(c)) {
        len++;
        c = buffer[++pos];
    }
    string keyword = buffer.substr(start, len);

    if (keyword == "L")     return T_label;
    if (keyword == "m")     return T_m;
    if (keyword == "print") return T_print;
    if (keyword == "if")    return T_if;
    if (keyword == "goto")  return T_goto;

    scan_error(keyword[0]);
}


// Read a single token, advancing the input pointer
token_type scanner_t::read_token() {
    int len = buffer.length();
    char c  = buffer[pos];

    while (c !=  EOF && pos < len-1) {
        if (isdigit(c)) return tokenize_num();
        if (isalnum(c)) return tokenize_keyword();

        switch (c) {
            case '+': return advance(T_plus);
            case '-': return advance(T_minus);
            case '*': return (buffer[pos+1] == '*') ? advance(T_power) : advance(T_times);
            case '/': return advance(T_divide);
            case '=': return advance(T_equals);
            case '(': return advance(T_openparen);
            case ')': return advance(T_closeparen);
            case '[': return advance(T_opensquare);
            case ']': return advance(T_closesquare);
            case ':': return advance(T_colon);
            case '\n':
            case '\t':
            case ' ':
                if (c == '\n') ++lnum;
                c = buffer[++pos]; continue;
            default: scan_error(c);
        }

        c = buffer[++pos];
    }

    return T_eof;
}


// Take a peek at the next token and return it to the parser.
// Does _not_ actually consume a token - can call  multiple
// times without actually reading any more tokens in
token_type scanner_t::next_token()
{
    return next;
}


// If we are supposed to eat token c, and it does not match
// what we are supposed to be reading from file, then it is a
// mismatch error
void scanner_t::eat_token(token_type c)
{
    if (next == c) {
        next = read_token();
    } else {
        mismatch_error(c);
    }
}


// Scanner constructor
scanner_t::scanner_t()
{
    // Preload input from stdin into buffer
    char c;
    while ((c = getchar()) != EOF) { buffer += c; }

    pos   = 0;
    lnum  = 1;
    next  = read_token();
}


// Return the current line number (used for error reporting)
int scanner_t::get_line()
{
    return lnum;
}


void scanner_t::scan_error(char x)
{
    printf("scan error: unrecognized character '%c' -line %d\n",x, get_line());
    exit(1);
}


void scanner_t::mismatch_error(token_type x)
{
    printf("syntax error: found %s ",token_to_string(next_token()) );
    printf("expecting %s - line %d\n", token_to_string(x), get_line());
    exit(1);
}


void scanner_t::range_error()
{
    printf("range error on line %d - value exceeds allowed limit\n", get_line());
    exit(1);
}


#endif
