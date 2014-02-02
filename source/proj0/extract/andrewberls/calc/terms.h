#ifndef TERMS_H
#define TERMS_H

using namespace std;

/*** Enums and Print Functions for Terminals and Non-Terminals  **********************/

#define MAX_SYMBOL_NAME_SIZE 25

// All of the terminals in the language
typedef enum {
    T_eof = 0,     // 0: end of file
    T_num,         // 1: numbers
    T_plus,        // 2: +
    T_minus,       // 3: -
    T_times,       // 4: *
    T_divide,      // 5: /
    T_power,       // 6: **
    T_equals,      // 7: =
    T_openparen,   // 8: (
    T_closeparen,  // 9: )
    T_opensquare,  // 10: [
    T_closesquare, // 11: ]
    T_m,           // 12: m
    T_goto,        // 13: goto
    T_if,          // 14: if
    T_print,       // 15: print
    T_label,       // 16: L
    T_colon        // 17: :
} token_type;


// Return a string for the token.
// Used in the parsetree_t class to actually dump the parsetree to a dot file
// (which can then be turned into a picture).
// Note that the return char* is a reference to a local copy and it needs to be
// duplicated if you are a going require multiple instances simultaniously
char* token_to_string(token_type c)
{
    static char buffer[MAX_SYMBOL_NAME_SIZE];
    switch (c) {
        case T_eof:         strncpy(buffer,"eof",MAX_SYMBOL_NAME_SIZE); break;
        case T_num:         strncpy(buffer,"num",MAX_SYMBOL_NAME_SIZE); break;
        case T_plus:        strncpy(buffer,"+",MAX_SYMBOL_NAME_SIZE); break;
        case T_minus:       strncpy(buffer,"-",MAX_SYMBOL_NAME_SIZE); break;
        case T_times:       strncpy(buffer,"*",MAX_SYMBOL_NAME_SIZE); break;
        case T_divide:      strncpy(buffer,"/",MAX_SYMBOL_NAME_SIZE); break;
        case T_power:       strncpy(buffer,"**",MAX_SYMBOL_NAME_SIZE); break;
        case T_equals:      strncpy(buffer,"=",MAX_SYMBOL_NAME_SIZE); break;
        case T_openparen:   strncpy(buffer,"(",MAX_SYMBOL_NAME_SIZE); break;
        case T_closeparen:  strncpy(buffer,")",MAX_SYMBOL_NAME_SIZE); break;
        case T_opensquare:  strncpy(buffer,"[",MAX_SYMBOL_NAME_SIZE); break;
        case T_closesquare: strncpy(buffer,"]",MAX_SYMBOL_NAME_SIZE); break;
        case T_m:           strncpy(buffer,"m",MAX_SYMBOL_NAME_SIZE); break;
        case T_goto:        strncpy(buffer,"goto",MAX_SYMBOL_NAME_SIZE); break;
        case T_if:          strncpy(buffer,"if",MAX_SYMBOL_NAME_SIZE); break;
        case T_print:       strncpy(buffer,"print",MAX_SYMBOL_NAME_SIZE); break;
        case T_label:       strncpy(buffer,"L",MAX_SYMBOL_NAME_SIZE); break;
        case T_colon:       strncpy(buffer,":",MAX_SYMBOL_NAME_SIZE); break;
        default:            strncpy(buffer,"unknown_token",MAX_SYMBOL_NAME_SIZE); break;
    }
    return buffer;
}


// All of the non-terminals in the grammar
typedef enum {
    epsilon = 100,
    NT_Start,
    NT_Statements,
    NT_Statement,
    NT_Label,
    NT_Jump,
    NT_Assignment,
    NT_Print,
    NT_Expression,
    NT_EAddSub,
    NT_MemAccess,
    NT_Term,
    NT_TMultDiv,
    NT_Factor,
    NT_NumParen,
    NT_Exponen
} nonterm_type;


// Returns a string for the non-terminals.
// Used in the parsetree_t class to actually dump the parsetree to a dot file
// (which can then be turned into a picture).
// Note that the return char* is a reference to a local copy and it needs to be
// duplicated if you are a going require multiple instances simultaneously.
char* nonterm_to_string(nonterm_type nt)
{
    static char buffer[MAX_SYMBOL_NAME_SIZE];
    switch (nt) {
        case epsilon:        strncpy(buffer,"e",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Start:       strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Statements:  strncpy(buffer,"Statements",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Statement:   strncpy(buffer,"Statement",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Label:       strncpy(buffer,"Label",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Jump:        strncpy(buffer,"Jump",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Assignment:  strncpy(buffer,"Assignment",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Print:       strncpy(buffer,"Print",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Expression:  strncpy(buffer,"Expression",MAX_SYMBOL_NAME_SIZE); break;
        case NT_EAddSub:     strncpy(buffer,"EAddSub",MAX_SYMBOL_NAME_SIZE); break;
        case NT_MemAccess:   strncpy(buffer,"MemAccess",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Term:        strncpy(buffer,"Term",MAX_SYMBOL_NAME_SIZE); break;
        case NT_TMultDiv:    strncpy(buffer,"TMultDiv",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Factor:      strncpy(buffer,"Factor",MAX_SYMBOL_NAME_SIZE); break;
        case NT_NumParen:    strncpy(buffer,"NumParen",MAX_SYMBOL_NAME_SIZE); break;
        case NT_Exponen:     strncpy(buffer,"Start",MAX_SYMBOL_NAME_SIZE); break;

        default: strncpy(buffer,"unknown_nonterm",MAX_SYMBOL_NAME_SIZE); break;
    }
    return buffer;
}

#endif
