<Type> -> <INTEGER>
<FunctionDecl> ->  <Type> <IDENTIFIER> <LP> <RP> <LBR>
<ProcedureHead> -> <FunctionDecl>
<StringConstant> -> <STRING>
Your grader
<Statement> -> <IOStatement>
<StatementList> -> <Statement>
<StringConstant> -> <STRING>
will put
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
a random
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<StringConstant> -> <STRING>
string here
<Statement> -> <IOStatement>
<StatementList> -> <StatementList> <Statement>
<ProcedureBody> -> <StatementList> <RBR>
<ProcedureDecl> -> <ProcedureHead> <ProcedureBody>
<Procedures> -> epsilon
<Procedures> -> <ProcedureDecl> <Procedures>
<Program> -> <Procedures>
