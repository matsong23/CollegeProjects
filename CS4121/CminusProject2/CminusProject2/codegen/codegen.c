#include <stdlib.h>
#include <string.h>
#include <util/string_utils.h>
#include <util/symtab.h>
#include <util/dlink.h>
#include "reg.h"
#include "codegen.h"
#include "symfields.h"
#include "types.h"
#include <stdio.h>

// Current symbol table
SymTable currentSymTab = NULL;
extern SymTable symtab;

void startCodegen() {
    currentSymTab = symtab;
    initRegisters();
}

int varOffset = 0;

// String constants
typedef struct StrConst {
    int num;
    char *str;
    struct StrConst *next;
} StrConst;

StrConst *strHead = NULL;
int stringCount = 0;

// Statements
typedef struct Stmt {
    int num;
    char *str;
    struct Stmt *next;
} Stmt;

Stmt *stmtHead = NULL;
int stmtCount = 0;

// Add Var
int addVar(SymTable t, int i) {
    int o = varOffset;
    SymPutFieldByIndex(t, i, SYMTAB_OFFSET_FIELD, (Generic)o);
    varOffset += 4;
    return o;
}

// Get var value in reg
int loadVar(SymTable t, int i) {
    char buf[256];
    int len;
    int offset = (int)SymGetFieldByIndex(t, i, SYMTAB_OFFSET_FIELD);
    int reg = allocateIntegerRegister();
    
    len = sprintf(buf, "        addi %s, $gp, %d\n", getIntegerRegisterName(reg), offset);
    Stmt *s1 = (Stmt *)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    int res = allocateIntegerRegister();
    len = sprintf(buf, "        lw %s, 0(%s)\n", getIntegerRegisterName(res), getIntegerRegisterName(reg));
    Stmt *s2 = (Stmt *)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(reg);
    return res;
}

// Store value to integer variables
void storeVar(SymTable t, int i, int sourceReg) {
    int o = (int)SymGetFieldByIndex(t, i, SYMTAB_OFFSET_FIELD);
    char buf[256];
    int len;

    int reg = allocateIntegerRegister();
    len = sprintf(buf, "        addi %s, $gp, %d\n", getIntegerRegisterName(reg), o);
    Stmt *s1 = (Stmt *)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        sw %s, 0(%s)\n", getIntegerRegisterName(sourceReg), getIntegerRegisterName(reg));
    Stmt *s2 = (Stmt *)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(reg);
}

// Load immediate
int loadImmediate(int v) {
    int reg = allocateIntegerRegister();
    char buf[256];
    int len;

    len = sprintf(buf, "        li %s, %d\n", getIntegerRegisterName(reg), v);
    Stmt *s1 = (Stmt *)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;
    return reg;
}

// String constants
void assignStringConstant(char *s) {
    StrConst *newStr = (StrConst *)malloc(sizeof(StrConst));
    newStr->num = stringCount;
    newStr->str = strdup(s);
    newStr->next = strHead;
    strHead = newStr;
    stringCount++;
}

// Print Strings
void printString(char *s) {
    StrConst *current = strHead;
    while (current != NULL) {
        if (strcmp(current->str, s) == 0) break;
        current = current->next;
    }
    if (current == NULL) {
        fprintf(stderr, "Error: String constant not found: %s\n", s);
        return;
    }

    char buf[256];
    int len;

    len = sprintf(buf, "        la $a0, .string%d\n", current->num);
    Stmt *s1 = (Stmt *)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        li $v0, 4\n");
    Stmt *s2 = (Stmt *)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    len = sprintf(buf, "        syscall\n");
    Stmt *s3 = (Stmt *)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    // Print newline
    len = sprintf(buf, "        li  $v0, 4\n");
    Stmt *s4 = (Stmt *)malloc(sizeof(Stmt));
    s4->num = stmtCount;
    s4->str = strdup(buf);
    s4->next = stmtHead;
    stmtHead = s4;
    stmtCount++;

    len = sprintf(buf, "        la  $a0, .newline\n");
    Stmt *s5 = (Stmt *)malloc(sizeof(Stmt));
    s5->num = stmtCount;
    s5->str = strdup(buf);
    s5->next = stmtHead;
    stmtHead = s5;
    stmtCount++;

    len = sprintf(buf, "        syscall\n");
    Stmt *s6 = (Stmt *)malloc(sizeof(Stmt));
    s6->num = stmtCount;
    s6->str = strdup(buf);
    s6->next = stmtHead;
    stmtHead = s6;
    stmtCount++;
}

// Print integers
void printInt(int i) {
    char buf[256];
    int len;
    
    char *reg = getIntegerRegisterName(i);
    len = sprintf(buf, "        move $a0, %s\n", reg);
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        li $v0, 1\n");
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    len = sprintf(buf, "        syscall\n");
    Stmt *s3 = (Stmt *)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    // Print newline
    len = sprintf(buf, "        li  $v0, 4\n");
    Stmt *s4 = (Stmt *)malloc(sizeof(Stmt));
    s4->num = stmtCount;
    s4->str = strdup(buf);
    s4->next = stmtHead;
    stmtHead = s4;
    stmtCount++;

    len = sprintf(buf, "        la  $a0, .newline\n");
    Stmt *s5 = (Stmt *)malloc(sizeof(Stmt));
    s5->num = stmtCount;
    s5->str = strdup(buf);
    s5->next = stmtHead;
    stmtHead = s5;
    stmtCount++;

    len = sprintf(buf, "        syscall\n");
    Stmt *s6 = (Stmt *)malloc(sizeof(Stmt));
    s6->num = stmtCount;
    s6->str = strdup(buf);
    s6->next = stmtHead;
    stmtHead = s6;
    stmtCount++;
}

void assignVar(int r, int i) {
    storeVar(currentSymTab, i, r);
    freeIntegerRegister(r);
}

// Read integers
void readInt(int i) {
    char buf[256];
    int len;

    len = sprintf(buf, "        li $v0, 5\n");
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        syscall\n");
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    int o = (int)SymGetFieldByIndex(currentSymTab, i, SYMTAB_OFFSET_FIELD);
    int reg = allocateIntegerRegister();

    len = sprintf(buf, "        addi %s, $gp, %d\n", getIntegerRegisterName(reg), o);
    Stmt *s3 = (Stmt*)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    len = sprintf(buf, "        sw $v0, 0(%s)\n", getIntegerRegisterName(reg));
    Stmt *s4 = (Stmt*)malloc(sizeof(Stmt));
    s4->num = stmtCount;
    s4->str = strdup(buf);
    s4->next = stmtHead;
    stmtHead = s4;
    stmtCount++;

    freeIntegerRegister(reg);
}

// Compute integer expressions
// Adding
int addInt(int r1, int r2) {
    char buf[256];
    int len;

    int newReg = allocateIntegerRegister();
    len = sprintf(
        buf, 
        "        add %s, %s, %s\n", 
        getIntegerRegisterName(newReg), 
        getIntegerRegisterName(r1), 
        getIntegerRegisterName(r2)
    );
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    freeIntegerRegister(r1);
    freeIntegerRegister(r2);
    return newReg;
}

// Subtracting
int subInt(int v1, int v2) {
    char buf[256];
    int len;

    int newReg = allocateIntegerRegister();
    len = sprintf(
        buf, 
        "        sub %s, %s, %s\n", 
        getIntegerRegisterName(newReg), 
        getIntegerRegisterName(v1), 
        getIntegerRegisterName(v2)
    );
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;
    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// Multiplying
int multInt(int v1, int v2) {
    char buf[256];
    int len;

    len = sprintf(
        buf, 
        "        mult %s, %s\n", 
        getIntegerRegisterName(v1), 
        getIntegerRegisterName(v2)
    );
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    int newReg =    allocateIntegerRegister();
    len = sprintf(
        buf, 
        "        mflo %s\n", 
        getIntegerRegisterName(newReg)
    );
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// Dividing
int divInt(int v1, int v2) {
    char buf[256];
    int len;

    len = sprintf(
        buf, 
        "        div %s, %s\n", 
        getIntegerRegisterName(v1), 
        getIntegerRegisterName(v2)
    );
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    int newReg = allocateIntegerRegister();
    len = sprintf(
        buf, 
        "        mflo %s\n", 
        getIntegerRegisterName(newReg)
    );
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// Compute logic expressions
// ==
int eq(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        xor %s, %s, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v1), getIntegerRegisterName(v2));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        sltu %s, $0, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(newReg));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    len = sprintf(buf, "        xori %s, %s, 1\n", getIntegerRegisterName(newReg), getIntegerRegisterName(newReg));
    Stmt *s3 = (Stmt*)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// !=
int ne(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        xor %s, %s, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v1), getIntegerRegisterName(v2));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        sltu %s, $0, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(newReg));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// <=
int le(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        slt %s, %s, %s\n", getIntegerRegisterName(v2), getIntegerRegisterName(v1), getIntegerRegisterName(newReg));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        xori %s, %s, 1\n", getIntegerRegisterName(newReg), getIntegerRegisterName(newReg));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// <
int lt(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        slt %s, %s, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v1), getIntegerRegisterName(v2));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// >=
int ge(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        slt %s, %s, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v1), getIntegerRegisterName(v2));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        xori %s, %s, 1\n", getIntegerRegisterName(newReg), getIntegerRegisterName(newReg));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// >
int gt(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        slt %s, %s, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v2), getIntegerRegisterName(v1));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    return newReg;
}

// ||
int or(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg1 = allocateIntegerRegister();
    len = sprintf(buf, "        sltu %s, $0, %s", getIntegerRegisterName(newReg1), getIntegerRegisterName(v1));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    int newReg2 = allocateIntegerRegister();
    len = sprintf(buf, "        sltu %s, $0, %s", getIntegerRegisterName(newReg2), getIntegerRegisterName(v2));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    len = sprintf(buf, "        or %s, %s, %s\n", getIntegerRegisterName(allocateIntegerRegister()), getIntegerRegisterName(newReg1), getIntegerRegisterName(newReg2));
    Stmt *s3 = (Stmt*)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    freeIntegerRegister(newReg1);
    return newReg2;
}

// &&
int and(int v1, int v2) {
    char buf[256];
    int len;
    
    int newReg1 = allocateIntegerRegister();
    len = sprintf(buf, "        sltu %s, $0, %s", getIntegerRegisterName(newReg1), getIntegerRegisterName(v1));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    int newReg2 = allocateIntegerRegister();
    len = sprintf(buf, "        sltu %s, $0, %s", getIntegerRegisterName(newReg2), getIntegerRegisterName(v2));
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;

    len = sprintf(buf, "        and %s, %s, %s\n", getIntegerRegisterName(allocateIntegerRegister()), getIntegerRegisterName(newReg1), getIntegerRegisterName(newReg2));
    Stmt *s3 = (Stmt*)malloc(sizeof(Stmt));
    s3->num = stmtCount;
    s3->str = strdup(buf);
    s3->next = stmtHead;
    stmtHead = s3;
    stmtCount++;

    freeIntegerRegister(v1);
    freeIntegerRegister(v2);
    freeIntegerRegister(newReg1);
    return newReg2;
}

// !
int not(int v) {
    char buf[256];
    int len;
    
    int newReg = allocateIntegerRegister();
    len = sprintf(buf, "        sltu %s, $0, %s\n", getIntegerRegisterName(newReg), getIntegerRegisterName(v));
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;
    freeIntegerRegister(v);
    return newReg;
}

// Print reversed stmt list
void printReversedStmtList(Stmt *n) {
    if (n == NULL) return;
    printReversedStmtList(n->next);
    printf("%s", n->str);
    free(n->str);
    free(n);
}

// Write text section
void writeTextSection() {
    printf("        .text\n");
    printf("        .globl main\n");
    printf("main:   nop\n");
    printf("        la $gp, globals\n");

    // Stmts are in reverse order so need to print in reverse
    printReversedStmtList(stmtHead);
}

// Write data section
void writeDataSection() {
    printf("        .data\n");
    printf("globals: .space %d\n", varOffset);
    printf(".newline: .asciiz \"\\n\"\n");

    while (strHead != NULL) {
        StrConst *current = strHead;
        printf(".string%d: .asciiz \"%s\"\n", current->num, current->str);
        strHead = strHead->next;
        free(current->str);
        free(current);
    }
}

// Write exit
void writeExit() {
    char buf[256];
    int len;

    len = sprintf(buf, "        li $v0, 10");
    Stmt *s1 = (Stmt*)malloc(sizeof(Stmt));
    s1->num = stmtCount;
    s1->str = strdup(buf);
    s1->next = stmtHead;
    stmtHead = s1;
    stmtCount++;

    len = sprintf(buf, "        syscall");
    Stmt *s2 = (Stmt*)malloc(sizeof(Stmt));
    s2->num = stmtCount;
    s2->str = strdup(buf);
    s2->next = stmtHead;
    stmtHead = s2;
    stmtCount++;
}

int getVal(int i) {
    return loadVar(currentSymTab, i);
}