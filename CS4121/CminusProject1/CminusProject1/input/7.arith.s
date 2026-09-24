<IdentifierList> -> <VarDecl>
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
15
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
4
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<ProcedureBody> -> <StatementList> <RBR>
<Procedures> -> epsilon
<Procedures> -> <ProcedureDecl> <Procedures>
<Program> -> <DeclList> <Procedures>
