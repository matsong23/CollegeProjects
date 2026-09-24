/*******************************************************/
/*                     Cminus Parser                   */
/*                                                     */
/*******************************************************/

/*********************DEFINITIONS***********************/
%{
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <util/general.h>
#include <util/symtab.h>
#include <util/symtab_stack.h>
#include <util/dlink.h>
#include <util/string_utils.h>
#include "CminusParser.h"

#define SYMTABLE_SIZE 100

/*********************EXTERNAL DECLARATIONS***********************/

EXTERN(void,Cminus_error,(const char*));

EXTERN(int,Cminus_lex,(void));

char *fileName;

extern int Cminus_lineno;
extern YYSTYPE Cminus_lval;

extern FILE *Cminus_in;

SymtabStack stack; // stack for table

// Globals for using read
int nextArgIndex = 2;
int argAmt;
char** args;
%}

%name-prefix="Cminus_"
%defines

%start Program

%token AND
%token ELSE
%token EXIT
%token FOR
%token IF 		
%token INTEGER 
%token NOT 		
%token OR 		
%token READ
%token WHILE
%token WRITE
%token LBRACE
%token RBRACE
%token LE
%token LT
%token GE
%token GT
%token EQ
%token NE
%token ASSIGN
%token COMMA
%token SEMICOLON
%token LBRACKET
%token RBRACKET
%token LPAREN
%token RPAREN
%token PLUS
%token TIMES
%token <string> IDENTIFIER
%token DIVIDE
%token RETURN
%token <string> STRING	
%token <num> INTCON
%token MINUS

%left OR
%left AND
%left NOT
%left LT LE GT GE NE EQ
%left PLUS MINUS
%left TIMES DIVDE

%type <string> StringConstant FunctionDecl VarDecl 
%type <num> Constant Factor MulExpr AddExpr SimpleExpr Expr 
%type <var> Variable

%union {
    int ival;
	int num;
    char *string;
	struct {
        char *name;
        int value;
    } var;
}


/***********************PRODUCTIONS****************************/
%%
Program		: Procedures 
		{
		}
	  		| DeclList Procedures
		{
		}
        	;

Procedures 	: ProcedureDecl Procedures
		{
		}
	   		|
		{
		}
	   		;

ProcedureDecl : ProcedureHead ProcedureBody
		{
			endScope(stack);
		}
              ;

ProcedureHead : FunctionDecl DeclList 
		{
		}
	    	| FunctionDecl
		{
		}
              ;

FunctionDecl :  Type IDENTIFIER LPAREN RPAREN LBRACE 
		{
			beginScope(stack);
			SymTable tab = lastSymtab(stack);
			if (!tab) exit(1);

			int i = SymQueryIndex(tab, $2);
			if (i == SYM_INVALID_INDEX) {
				i = SymIndex(tab, $2);
				SymPutFieldByIndex(tab, i, "t", "f");
			} else exit(1);
		}
	      	;

ProcedureBody : StatementList RBRACE
		{
		}
	      ;


DeclList 	: Type IdentifierList  SEMICOLON 
		{
		}		
	   		| DeclList Type IdentifierList SEMICOLON
	 	{
	 	}
          	;


IdentifierList 	: VarDecl  
		{
		}		
            | IdentifierList COMMA VarDecl
		{
		}
            ;

VarDecl 	: IDENTIFIER
		{ 
			SymTable tab = currentSymtab(stack);
			int i = SymQueryIndex(tab, $1);
			if (i != SYM_INVALID_INDEX) exit(1);
		
			i = SymIndex(tab, $1);
			SymPutFieldByIndex(tab, i, "t", "int");
			SymPutFieldByIndex(tab, i, "v", 0); 
		}
			| IDENTIFIER LBRACKET INTCON RBRACKET
		{
		}
			;

Type     	: INTEGER 
		{ 
			
		}
            ;

Statement 	: Assignment
		{ 
		}
            | IfStatement
		{ 
		}
			| WhileStatement
		{ 
		}
            | IOStatement 
		{ 
		}
			| ReturnStatement
		{ 
		}
			| ExitStatement	
		{ 
		}
			| CompoundStatement
		{ 
		}
                ;

Assignment      : Variable ASSIGN Expr SEMICOLON
		{
			SymTable tab = findSymtab(stack, $1.name);
			if (!tab) exit(1);
			int i = SymQueryIndex(tab, $1.name);
			if (i == SYM_INVALID_INDEX) exit(1);
			SymPutFieldByIndex(tab, i, "v", $3);
		}
                ;
				
IfStatement	: IF TestAndThen ELSE CompoundStatement
		{
		}
			| IF TestAndThen
		{
		}
			;
		
				
TestAndThen	: Test CompoundStatement
		{
		}
			;
				
Test		: LPAREN Expr RPAREN
		{
		}
			;
	

WhileStatement  : WhileToken WhileExpr Statement
		{
		}
                ;
                
WhileExpr	: LPAREN Expr RPAREN
		{
		}
			;
				
WhileToken	: WHILE
		{
		}
			;


IOStatement     : READ LPAREN Variable RPAREN SEMICOLON
		{
			SymTable tab = findSymtab(stack, $3.name);
			if (!tab || nextArgIndex >= argAmt) exit(1); // verify table and if argument avaible
			
			int i = SymQueryIndex(tab, $3.name);
			if (i == SYM_INVALID_INDEX) exit(1);

			int v = atoi(args[nextArgIndex]); 
			nextArgIndex++; 
			SymPutFieldByIndex(tab, i, "v", v);
		}
            | WRITE LPAREN Expr RPAREN SEMICOLON
		{
			printf("%d\n", $3);
		}
            | WRITE LPAREN StringConstant RPAREN SEMICOLON         
		{
			printf("%s\n", $3);
			free($3);
		}
            ;

ReturnStatement : RETURN Expr SEMICOLON
		{
		}
            ;

ExitStatement 	: EXIT SEMICOLON
		{
		}
			;

CompoundStatement : LBRACE StatementList RBRACE
		{
		}
            ;

StatementList   : Statement
		{		
		}
            | StatementList Statement
		{		
		}
            ;

Expr        : SimpleExpr
		{
			$$ = $1;
		}
            | Expr OR SimpleExpr 
		{
			$$ = $1 || $3;
		}
            | Expr AND SimpleExpr 
		{
			$$ = $1 && $3;
		}
            | NOT SimpleExpr 
		{
			$$ = !$2;
		}
            ;

SimpleExpr	: AddExpr
		{
			$$ = $1;
		}
            | SimpleExpr EQ AddExpr
		{
			if ($1 == $3) $$ = 1;
			else $$ = 0;
		}
            | SimpleExpr NE AddExpr
		{
			if ($1 != $3) $$ = 1;
			else $$ = 0;
		}
            | SimpleExpr LE AddExpr
		{
			if ($1 <= $3) $$ = 1;
			else $$ = 0;
		}
            | SimpleExpr LT AddExpr
		{
			if ($1 < $3) $$ = 1;
			else $$ = 0;
		}
            | SimpleExpr GE AddExpr
		{
			if ($1 >= $3) $$ = 1;
			else $$ = 0;
		}
            | SimpleExpr GT AddExpr
		{
			if ($1 > $3) $$ = 1;
			else $$ = 0;
		}
            ;

AddExpr		:  MulExpr            
		{
			$$ = $1;
		}
            |  AddExpr PLUS MulExpr
		{
			$$ = $1 + $3;
		}
            |  AddExpr MINUS MulExpr
		{
			$$ = $1 - $3;
		}
            ;

MulExpr		:  Factor
		{
			$$ = $1;
		}
            |  MulExpr TIMES Factor
		{
			$$ = $1 * $3;
		}
            |  MulExpr DIVIDE Factor
		{
			$$ = $1 / $3;
		}		
            ;
				
Factor      : Variable
		{ 
			$$ = $1.value;
		}
            | Constant
		{ 
			$$ = $1;
		}
            | IDENTIFIER LPAREN RPAREN
       	{	
		}
         	| LPAREN Expr RPAREN
		{
			$$ = $2;
		}
            ;  

Variable    : IDENTIFIER
		{
			SymTable tab = findSymtab(stack, $1);
			int i = SymQueryIndex(tab, $1);
			Generic g = SymGetFieldByIndex(tab, i, "v");
			$$.value = g; 
			$$.name = $1;
        }
			| IDENTIFIER LBRACKET Expr RBRACKET
        {
        }
            ;			       

StringConstant 	: STRING
		{ 
        	$$ = $1;
		}
            ;

Constant    : INTCON
		{ 
			$$ = $1;
		}
            ;

%%


/********************C ROUTINES *********************************/

void Cminus_error(const char *s)
{
  fprintf(stderr,"%s: line %d: %s\n",fileName,Cminus_lineno,s);
}

int Cminus_wrap() {
	return 1;
}

static void initialize(char* inputFileName) {

	Cminus_in = fopen(inputFileName,"r");
        if (Cminus_in == NULL) {
          fprintf(stderr,"Error: Could not open file %s\n",inputFileName);
          exit(-1);
        }

	char* dotChar = rindex(inputFileName,'.');
	int endIndex = strlen(inputFileName) - strlen(dotChar);
	char *outputFileName = nssave(2,substr(inputFileName,0,endIndex),".s");
	stdout = freopen(outputFileName,"w",stdout);
        if (stdout == NULL) {
          fprintf(stderr,"Error: Could not open file %s\n",outputFileName);
          exit(-1);
        }

}

static void finalize() {

    fclose(Cminus_in);
    fclose(stdout);
    

}

int main(int argc, char** argv)

{	
	// set globals
	argAmt = argc;
	args = argv;
	stack = symtabStackInit();

	fileName = argv[1];
	initialize(fileName);

	beginScope(stack);
	
    Cminus_parse();
  
	endScope(stack);

  	finalize();
  	return 0;
}
/******************END OF C ROUTINES**********************/
