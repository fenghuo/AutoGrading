#ifndef PARSETREE_H
#define PARSETREE_H

using namespace std;

/*** ParseTree Class **********************************************/

// Just dumps out the tree as a dot file.  The interface is described below
// on the actual methods.  This class is full and complete.  You should not
// have to touch a thing if everything goes according to plan.  While you don't
// have to modify it, you will have to call it from your recursive decent
// parser, so read about the interface below.
class parsetree_t {
    public:
    void push(token_type t);
    void push(nonterm_type nt);
    void pop();
    void drawepsilon();
    parsetree_t();

    private:
    enum stype_t{
        TERMINAL = 1,
        NONTERMINAL = 0,
        UNDEF = -1
    };
    struct stuple {
        nonterm_type nt;
        token_type t;
        stype_t stype;
        int uniq;
    };
    void printedge(stuple temp); //prints edge from TOS->temp
    stack<stuple> stuple_stack;
    char* stuple_to_string(const stuple& s);
    int counter;
};


//the constructer just starts by initializing a counter (used to uniquely
//name all the parse tree nodes) and by printing out the necessary dot commands
parsetree_t::parsetree_t()
{
    counter = 0;
    printf("digraph G {\n");
}


// This push function takes a non terminal and keeps it on the parsetree
// stack. The stack keeps trace of where we are in the parse tree as
// we walk it in a depth first way. You should call push when you start
// expanding a symbol, and call pop when you are done.  The parsetree_t
// will keep track of everything, and draw the parse tree as you go.
// This particular function should be called if you are pushing a non-terminal
void parsetree_t::push(nonterm_type nt)
{
    counter++;
    stuple temp;
    temp.nt = nt;
    temp.stype = NONTERMINAL;
    temp.uniq = counter;
    printedge(temp);
    stuple_stack.push(temp);
}


// Same as above, but for terminals
void parsetree_t::push(token_type t)
{
    counter++;
    stuple temp;
    temp.t = t;
    temp.stype = TERMINAL;
    temp.uniq = counter;
    printedge(temp);
    stuple_stack.push(temp);
}


// When you are done parsing a symbol, pop it. That way the parsetree_t will
// know that you are now working on a higher part of the tree.
void parsetree_t::pop()
{
    if (!stuple_stack.empty()){
        stuple_stack.pop();
    }

    if (stuple_stack.empty()){
        printf( "};\n" );
    }
}


//draw an epsilon on the parse tree hanging off of the top of stack
void parsetree_t::drawepsilon()
{
    push(epsilon);
    pop();
}


// This private print function is called from push. Basically it
// just prints out the command to draw an edge from the top of the stack (TOS)
// to the new symbol that was just pushed. If it happens to be a terminal
// then it makes it a snazzy blue color so you can see your program on the leaves
void parsetree_t::printedge(stuple temp)
{
    if (temp.stype == TERMINAL) {
        printf("\t\"%s%d\" [label=\"%s\",style=filled,fillcolor=powderblue]\n",
            stuple_to_string(temp),
            temp.uniq,
            stuple_to_string(temp));
    } else {
        printf("\t\"%s%d\" [label=\"%s\"]\n",
            stuple_to_string(temp),
            temp.uniq,
            stuple_to_string(temp));
    }

    // no edge to print if this is the first node
    if (!stuple_stack.empty()){
        //print the edge
        printf( "\t\"%s%d\" ", stuple_to_string(stuple_stack.top()), stuple_stack.top().uniq );
        printf( "-> \"%s%d\"\n", stuple_to_string(temp), temp.uniq );
    }
}

//just a private utility for helping with the printing of the dot stuff
char* parsetree_t::stuple_to_string(const stuple& s)
{
    static char buffer[MAX_SYMBOL_NAME_SIZE];
    if ( s.stype == TERMINAL ) {
        snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "%s", token_to_string(s.t) );
    } else if ( s.stype == NONTERMINAL ) {
        snprintf( buffer, MAX_SYMBOL_NAME_SIZE, "%s", nonterm_to_string(s.nt) );
    } else {
        assert(0);
    }

    return buffer;
}

#endif
