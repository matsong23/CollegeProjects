<IdentifierList> -> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<DeclList> -> <Type> <IdentifierList> <SC>
<ProcedureHead> -> <FunctionDecl>
<Statement> -> <Assignment>
<StatementList> -> <Statement>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
8
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<Statement> -> <Assignment>
<StatementList> -> <StatementList> <Statement>
-1
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
24
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
2
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
15
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
5
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
-33
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<ProcedureBody> -> <StatementList> <RBR>
<Procedures> -> epsilon
<Procedures> -> <ProcedureDecl> <Procedures>
<Program> -> <DeclList> <Procedures>
