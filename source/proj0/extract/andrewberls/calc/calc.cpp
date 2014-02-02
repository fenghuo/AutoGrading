#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stack>

#include "terms.h"
#include "scanner.h"
#include "parsetree.h"
#include "parser.h"

using namespace std;

/*** Main ***********************************************/

int main()
{
    parser_t parser;
    parser.parse();
    return 0;
}
