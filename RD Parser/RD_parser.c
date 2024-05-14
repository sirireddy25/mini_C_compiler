#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

struct token token2;
FILE *fp;

char *relops[6] = {"==", "!=", "<=", ">=", "<", ">"};
char *addops[2] = {"+", "-"};
char *mulops[3] = {"*", "/", "%"};

void Program();
void declarations();
int data_type();
void identifier_list();
void identifier_listprime();
void identifier_listprime1();
void assign_stat();
void assign_statprime();
void statement_list();
void statement();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
int relop();
int addop();
int mulop();
void valid();
void invalid();
int descision_stat();
int looping_stat();
void dprime();

void valid(){
	token2 = getNextToken(fp);
	if(strcmp(token2.type, "id") == 0 || (token2.type, "num") == 0 || (token2.type, "keyword") == 0 || (token2.type, "function") == 0 ||(token2.type, "dtype") == 0){
		invalid("$");
	}
	else{
		printf("\n-------SUCCESS!-------\n\n");
	}
	exit(0);
}

void invalid(char *follow)
{
	printf("\n----------ERROR!----------\n\n");
	printf("error reported at row no. %d column no. %d\n", token2.row, token2.col);
	printf("expected %s\n", follow);
	exit(0);
}

void Program(){
	token2 = getNextToken(fp);
	if (strcmp(token2.lexeme, "main") == 0){
		token2 = getNextToken(fp);
		if (strcmp(token2.lexeme, "(") == 0){
			token2 = getNextToken(fp);
			if (strcmp(token2.lexeme, ")") == 0){
				token2 = getNextToken(fp);
				if (strcmp(token2.lexeme, "{") == 0){
					token2 = getNextToken(fp);
					declarations();
					statement_list();
					if (strcmp(token2.lexeme, "}") == 0){
						valid();
					}
					else{
						invalid("}");
					}
				}
				else{
					invalid("{");				
				}
			}
			else{
				invalid(")");
			}
		}
		else{
			invalid("(");
		}
	}
	else{
		invalid("main");
	}
}

void declarations(){
	int d;
	if (data_type()){ 
		identifier_list();
		if (strcmp(token2.lexeme, ";") == 0){
			token2 = getNextToken(fp);
			declarations();
		}
		else{
			invalid(";");
		}
	}
	else
		return;
}

int data_type(){
	if (strcmp(token2.type, "dtype") == 0){
		if(strcmp(token2.lexeme, "char") == 0 || strcmp(token2.lexeme, "int") == 0){
			token2 = getNextToken(fp);
			return 1;
		}
		else
			invalid("int or char");
	}
	else
		return 0;
}


void identifier_list(){
    if (strcmp(token2.type, "id") == 0){
        token2 = getNextToken(fp);
        identifier_listprime();
    }
    else{
        invalid("id");
    }
}

void identifier_listprime(){
    if (strcmp(token2.lexeme, ",") == 0){
        token2 = getNextToken(fp);
        identifier_list();
    }
    else if(strcmp(token2.lexeme, "[") == 0){
        token2 = getNextToken(fp);
        if(strcmp(token2.type, "num") == 0){
            token2 = getNextToken(fp);
            if(strcmp(token2.lexeme, "]") == 0){
                token2 = getNextToken(fp);
                identifier_listprime1();
            }
            else
                invalid("]");
        }
        else
            invalid("num");
    }
    else if(strcmp(token2.type, "id") == 0)
		invalid(",");
	else
		return;
}

void identifier_listprime1(){
    if (strcmp(token2.lexeme, ",") == 0){
        token2 = getNextToken(fp);
        identifier_list();
    }
    else if(strcmp(token2.type, "id") == 0)
		invalid(",");
	else
		return;
}

void assign_stat(){
    if (strcmp(token2.type, "id") == 0){
        token2 = getNextToken(fp);
        if (strcmp(token2.lexeme, "=") == 0){
            token2 = getNextToken(fp);
            expn();
        }
        else{
            invalid("=");
        }
    }
    else{
        invalid("id");
    }
}

void statement_list(){
	if(strcmp(token2.type, "id") == 0 || strcmp(token2.lexeme, "if") == 0 || strcmp(token2.lexeme, "while") == 0|| strcmp(token2.lexeme, "for") == 0){
		statement();
		statement_list();
	}
	else
		return;
}

void statement(){
	if(strcmp(token2.type, "id") == 0){
    	assign_stat();
    	if(strcmp(token2.lexeme, ";") == 0){
    		token2 = getNextToken(fp);
        	return;
    	}	
    	else{
        	invalid(";");
    	}
    }
    else if(descision_stat()){
    }
    else if(looping_stat()){
    }
    else{
    	invalid("id or if or while or for");
    }
}

void dprime(){
	if(strcmp(token2.lexeme, "else") == 0){
		token2 = getNextToken(fp);
		if(strcmp(token2.lexeme,"{") == 0){
			token2 = getNextToken(fp);
			statement_list();
			//may have to getNextToken
			if(strcmp(token2.lexeme, "}") == 0){
				token2 = getNextToken(fp);
				return;
			}
			else{
				invalid("}");
			}
		}
		else{
			invalid("{");
		}
	}
}

int descision_stat(){
	if(strcmp(token2.lexeme, "if") == 0){
		token2 = getNextToken(fp);
		if(strcmp(token2.lexeme, "(") == 0){
			token2 = getNextToken(fp);
			expn();
			if(strcmp(token2.lexeme, ")") == 0){
				token2 = getNextToken(fp);
				if(strcmp(token2.lexeme,"{") == 0){
					token2 = getNextToken(fp);
					statement_list();
					//may have to getNextToken
					if(strcmp(token2.lexeme, "}") == 0){
						token2 = getNextToken(fp);
						dprime();
						return 1;
					}
					else{
						invalid("}");
					}
				}
				else{
					invalid("{");
				}
			}
			else{
				invalid(")");
			}
		}
		else{
			invalid("(");
		}
	}
	else{
		return 0;
	}
}

int looping_stat(){
	if(strcmp(token2.lexeme,"while") == 0){
		token2 = getNextToken(fp);
		if(strcmp(token2.lexeme, "(") == 0){
			token2 = getNextToken(fp);
			expn();
			if(strcmp(token2.lexeme, ")") == 0){
				token2 = getNextToken(fp);
				if(strcmp(token2.lexeme,"{") == 0){
					token2 = getNextToken(fp);
					statement_list();
					//may have to getNextToken
					if(strcmp(token2.lexeme, "}") == 0){
						token2 = getNextToken(fp);
						return 1;
					}
					else{
						invalid("}");
					}
				}
				else{
					invalid("{");
				}
			}
			else{
				invalid(")");
			}
		}
		else{
			invalid("(");
		}
	}
	else if(strcmp(token2.lexeme,"for") == 0){
		token2 = getNextToken(fp);
		if(strcmp(token2.lexeme,"(") == 0){
			token2 = getNextToken(fp);
			assign_stat();
			if(strcmp(token2.lexeme, ";") == 0){
				token2 = getNextToken(fp);
				expn();
				if(strcmp(token2.lexeme, ";") == 0){
					token2 = getNextToken(fp);
					assign_stat();
					if(strcmp(token2.lexeme, ")") == 0){
						token2 = getNextToken(fp);
						if(strcmp(token2.lexeme, "{") == 0){
							token2 = getNextToken(fp);
							statement_list();
							if(strcmp(token2.lexeme, "}") == 0){
								token2 = getNextToken(fp);
								return 1;
							}
							else{
								invalid("}");
							}
						}
						else{
							invalid("{");
						}
					}
					else{
						invalid(")");
					}
				}
				else{
					invalid(";");
				}
			}
			else{
				invalid(";");
			}
		}
		else{
			invalid("(");
		}
	}
	else{
		return 0;
	}
}

void expn(){
    simple_expn();
    eprime();
}

void eprime(){
    if (relop()){
        simple_expn();
    }
    else
        return;
}


void simple_expn(){
    term();
    seprime();
}

void term(){
    factor();
    tprime();
}

void seprime(){
    if(addop()){
        term();
        seprime();
    }
    else
        return;
}


void factor(){
    if(strcmp(token2.type, "id") == 0 || strcmp(token2.type, "num") == 0){
        token2 = getNextToken(fp);
        return;
    }
    else
        invalid("id or num");
}

void tprime(){
    if(mulop()){
        factor();
        tprime();
    }
    else
        return;
}

int relop(){
    for(int i = 0; i < 6; i++){
        if(strcmp(token2.lexeme, relops[i]) == 0){
            token2 = getNextToken(fp);
            return 1;
        }
    }
    return 0;
}

int addop(){
    for(int i = 0; i < 2; i++){
        if(strcmp(token2.lexeme, addops[i]) == 0){
            token2 = getNextToken(fp);
            return 1;
        }
    }
    return 0;
}

int mulop(){
    for(int i = 0; i < 3; i++){
        if(strcmp(token2.lexeme, mulops[i]) == 0){
            token2 = getNextToken(fp);
            return 1;
        }
    }
    return 0;
}


int main(){
	char filename[100];
	printf("Enter file name\n");
	scanf("%s",filename);
	fp = fopen(filename,"r");
	if (fp == NULL){
		printf("Cannot open file \n");
		exit(0);
	}
	Program();
	valid();
}