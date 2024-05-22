
# Miniature C compiler

This project assesses the syntactical correctness of a C program by performing lexical analysis and recursive descent parsing according to the given grammar.


## Grammar
Program -> main ( ) { declarations statement_list }

declarations -> data_type identifier_list; declarations | € 

data_type -> int | char

identifier_list -> id identifier_listprime

identifier_listprime -> € | , identifier_list | [number] identifier_listprime1

identifier_listprime1 -> € | ,identifier_list

statement_list → statement statement _list | € 

statement -> assign_stat; | decision_stat | looping_stat 

decision_stat -> if (expn) {statement_list} dprime

dprime -> else{statemnt_list} | € 

looping_stat → while (expn) {statement_list} | for (assign_stat; expn; assign_stat){statement_list}

assign _stat -> id = expn

expn -> simple_expn eprime 

eprime -> relop simple_expn | € 

simple_exp -> term seprime 

seprime -> addop term seprime | € 

term -> factor tprime

tprime -> mulop factor tprime | € 

factor -> id| num

relop -> == | != |<= |>= | > |< 

addop -> + | -

mulop -> * | / | %
## Lexical Analyzer
This is the first step in the compilation process.

In this section, characters are grouped into meaningful sequences called lexemes. The Lexical analyzer outputs a stream of tokens for all the lexemes identified. 

Additionaly, a symbol table is created and each identifier generated during lexical analysis is stored in the symbol table. Only one entry is made in the symbol table for each identifier. 

The lexical analyzer also discards any white spaces and comments in the input file. Tokens are not generated for preprocessor directives. 

Structure of the tokens:
```bash
  <token type, row number, column number>
```

Output of Lexical analyzer (token stream + symbol table:

![LA](https://github.com/sirireddy25/mini_C_compiler/assets/79968911/4bb18922-f763-4c19-8f57-277535e645ff)

## Recursive Descent Parser
This is the second step in the compilation process. The output of the lexical analyser - stream of tokens + symbol table is passed to the parser. The parser checks that the source code is according to the given grammar. 

Recursive descent is a top-down parsing technique that constructs the parse tree from the top (non-terminals are expanded) and the input is read from left to right. For every non-terminal, there is a procedure call.

Output for error free code:

![success](https://github.com/sirireddy25/mini_C_compiler/assets/79968911/7f116a87-9252-4ca8-bc90-07caf1f4ced7)

Output for code with error:

![error](https://github.com/sirireddy25/mini_C_compiler/assets/79968911/05b67e43-90a4-43aa-99ee-aaa1a790dfdf)

