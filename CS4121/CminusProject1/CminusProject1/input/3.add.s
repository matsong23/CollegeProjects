<IdentifierList> -> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<IdentifierList> -> <IdentifierList> <CM> <VarDecl>
<DeclList> -> <Type> <IdentifierList> <SC>
<ProcedureHead> -> <FunctionDecl>
30
<Statement> -> <IOStatement>
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
<ProcedureBody> -> <StatementList> <RBR>
<Procedures> -> epsilon
<Procedures> -> <ProcedureDecl> <Procedures>
<Program> -> <DeclList> <Procedures>
