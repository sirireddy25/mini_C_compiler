#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token {
    char lexeme[64];
    int row, col;
    char type[20];
};

static int row = 1, col = 1;
char buf[1024];
const char specialsymbols[] = {'?', ';', ':', ',','(', ')', '{', '}'};
const char *Keywords[] = {"for", "if", "else", "while", "do", "break", "continue", "return", "int", "double", "float", "char", "long", "short", "sizeof", "typedef", "switch", "case", "struct", "const", "void", "exit"};
const char arithmeticsymbols[] = {'*','+','-', '%'};

//********************************************************************************************************************************************************
struct table_entry{
    int sno;
    char lexeme_name[100];
    char dtype[100],retype[100],tname[100];
    int num_args;
}Table[100],globTable[100];

char dbuf[200];
static int ind=1,globind=1;

const char *datatypes[]={"int","char","float","double"};

int searchTable(const char* lexeme_name){
    if(ind==1)
        return 0;
    int i=0;
    while(i<ind){
        if(strcmp(Table[i].lexeme_name,lexeme_name)==0)
            return 1;
        i++;
    }
    return 0;
}
void insertIntoTable(struct token* tkn,int n_args,const char* type){
    if(!strcmp(type,"variable")){
        Table[ind-1].sno=ind;
        strcpy(Table[ind-1].lexeme_name,tkn->lexeme);
        strcpy(Table[ind-1].dtype,dbuf);
        strcpy(Table[ind-1].tname,"var");
        ++ind;
    }
    else if(!strcmp(type,"function")){
        Table[ind-1].sno=ind;
        strcpy(Table[ind-1].lexeme_name,tkn->lexeme);
        strcpy(Table[ind-1].retype,dbuf);
        strcpy(Table[ind-1].tname,"func");
        Table[ind-1].num_args=n_args;
        ++ind;
    }
    else
        printf("Token cannot be inserted\n");
}

void printTable(int n){
    for(int i=0;i<n;i++){
        if(!strcmp(Table[i].tname,"var"))
            printf("%d\t\t\t%s\t\t\t%s\t\t\t-\t\t\t-\t\t%s\n",Table[i].sno,Table[i].lexeme_name,Table[i].dtype,Table[i].tname);
        if(!strcmp(Table[i].tname,"func"))
            printf("%d\t\t\t%s\t\t\t-\t\t\t%s\t\t\t%d\t\t%s\n",Table[i].sno,Table[i].lexeme_name,Table[i].retype,Table[i].num_args,Table[i].tname);
    }
}

int isFuncName(FILE* f,char c){
    int n_args=0;
    if(c=='('){
        int chars=0;
        while((c=fgetc(f))!=EOF && c !=')'){
            if(c ==',')
                n_args++;
            chars++;
        }
        if(n_args!=0 || chars > 0)
            n_args++;
        fseek(f,(0-chars-1),SEEK_CUR);
    }
    else{
        n_args= -1;
    }
    return n_args;
}

int isDtype(const char* lexeme){
    for(int i=0;i<sizeof(datatypes)/sizeof(char*);i++){
        if(strcmp(lexeme,datatypes[i])==0)
            return 1;
    }
    return 0;
}
 

//*********************************************************************************************************************************************************

int isKeyword(const char *str) {
    for (int i = 0; i < sizeof(Keywords) / sizeof(char *); i++) {
        if (strcmp(str, Keywords[i]) == 0)
            return 1;
    }
    return 0;
}
int charBelongsTo(int c, const char *arr){
    int len;
    if (arr == specialsymbols)
        len = sizeof(specialsymbols) / sizeof(char);
    else if (arr == arithmeticsymbols)
        len = sizeof(arithmeticsymbols) / sizeof(char);
    for (int i = 0; i < len; i++) {
        if (c == arr[i])
            return 1;
    }
    return 0;
}
void fillToken(struct token *tkn, char c, int row, int col, char *type) {
    tkn->row = row;
    tkn->col = col;
    strcpy(tkn->type, type);
    tkn->lexeme[0] = c;
    tkn->lexeme[1] = '\0';
}
void newLine() {
    ++row;
    col = 1;
}
struct token getNextToken(FILE *fin) {
    int c, d;
    struct token tkn = {.row = -1};
    int gotToken = 0;
    while (!gotToken && (c = fgetc(fin)) != EOF) {
        if (charBelongsTo(c, specialsymbols)) {
            switch(c) {
                case ';':
                    fillToken(&tkn, c, row, col, ";");
                    break;
                case ':':
                    fillToken(&tkn, c, row, col, ":");
                    break;
                case '?':
                    fillToken(&tkn, c, row, col, "?");
                    break;
                case ',':
                    fillToken(&tkn, c, row, col, ",");
                    break;
                case '(':
                	fillToken(&tkn, c, row, col, "(");
                	break;
                case ')':
                	fillToken(&tkn, c, row, col, ")");
                	break;
                case '{':
                	fillToken(&tkn, c, row, col, "{");
                	break;
                case '}':
                	fillToken(&tkn, c, row, col, "}");
                	break;
                default : break;
                
            }
            gotToken = 1;
            col++;
        }

        else if (charBelongsTo(c, arithmeticsymbols)) { 
            switch(c) {
                case '+':
                    d = fgetc(fin);
		            if (d == '+') {
		                fillToken(&tkn, c, row, col, "++");
		                strcpy(tkn.lexeme, "++");
		                gotToken = 1;
		                col += 2;
		            }
		            else if (d == '=') {
		                fillToken(&tkn, c, row, col, "+=");
		                strcpy(tkn.lexeme, "+=");
		                gotToken = 1;
		                col += 2;
		            }
		            else {
		                fillToken(&tkn, c, row, col, "+");
		                gotToken = 1;
		                col++;
		                fseek(fin, -1, SEEK_CUR);
		            }
                    break;

                case '-':
                    d = fgetc(fin);
		            if (d == '-') {
		                fillToken(&tkn, c, row, col, "--");
		                strcpy(tkn.lexeme, "--");
		                gotToken = 1;
		                col += 2;
		            }
		            else if (d == '=') {
		                fillToken(&tkn, c, row, col, "-=");
		                strcpy(tkn.lexeme, "-=");
		                gotToken = 1;
		                col += 2;
		            }
		            else {
		                fillToken(&tkn, c, row, col, "-");
		                gotToken = 1;
		                col++;
		                fseek(fin, -1, SEEK_CUR);
		            }
                    break;

                case '*':
                    d = fgetc(fin);
		            if (d != '=') {
		                fillToken(&tkn, c, row, col, "*");
		                gotToken = 1;
		                col++;
		                fseek(fin, -1, SEEK_CUR);
		            }
		            else {
		                fillToken(&tkn, c, row, col, "*=");
		                strcpy(tkn.lexeme, "*=");
		                gotToken = 1;
		                col += 2;
		            }
                    break;
                case '%':
                    d = fgetc(fin);
		            if (d != '=') {
		                fillToken(&tkn, c, row, col, "%");
		                gotToken = 1;
		                col++;
		                fseek(fin, -1, SEEK_CUR);
		            }
		            else {
		                fillToken(&tkn, c, row, col, "%=");
		                strcpy(tkn.lexeme, "%=");
		                gotToken = 1;
		                col += 2;
		            }
                    break;
                default : break;
                
            }
            gotToken = 1;
            col++;
        }

        else if (c == '=') {
            int d = fgetc(fin);
            if (d != '=') {
                fillToken(&tkn, c, row, col, "=");
                gotToken = 1;
                col++;
                fseek(fin, -1, SEEK_CUR);
            }
            else {
                fillToken(&tkn, c, row, col, "==");
                strcpy(tkn.lexeme, "==");
                gotToken = 1;
                col += 2;
            }
        }

        else if (c == '<' || c == '>' || c == '!') {
            switch(c) {
                case '>':
                    fillToken(&tkn, c, row, col, ">");
                    break;
                case '<':
                    fillToken(&tkn, c, row, col, "<");
                    break;
                case '!':
                    fillToken(&tkn, c, row, col, "!");
                    break;
                default : break;
            }
            col++;
            int d = fgetc(fin);
            if (d == '=') {
                col++;
                strcat(tkn.lexeme, "=");
            }
            else {
                if (c == '!')
                    strcpy(tkn.type, "!=");
                fseek(fin, -1, SEEK_CUR);
            }
            gotToken = 1;
        }

        else if (c == '&' || c == '|') {
            int d = fgetc(fin);
            if (c == d) {
                tkn.lexeme[0] = tkn.lexeme[1] = c;
                tkn.lexeme[2] = '\0';
                tkn.row = row;
                tkn.col = col;
                col++;
                gotToken = 1;
                switch(c) {
                case '&':
                    fillToken(&tkn, c, row, col, "&&");
                    break;
                case '|':
                    fillToken(&tkn, c, row, col, "||");
                    break;
                default : break;
                
            	}
            }
            else
                fseek(fin, -1, SEEK_CUR);
            col++;
        }

        else if (isdigit(c)) {
            tkn.row = row;
            tkn.col = col++;
            tkn.lexeme[0] = c;
            int k = 1;
            while ((c = fgetc(fin)) != EOF && isdigit(c)) {
                tkn.lexeme[k++] = c;
                col++;
            }
            tkn.lexeme[k] = '\0';
            strcpy(tkn.type, "Number");
            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }

        else if (c == '#') {
            while ((c = fgetc(fin)) != EOF && c != '\n');
            newLine();
        }

        else if (c == '\n') {
            newLine();
            c = fgetc(fin);
            if (c == '#') {
                while ((c = fgetc(fin)) != EOF && c != '\n');
                newLine();
            }
            else if (c != EOF)
                fseek(fin, -1, SEEK_CUR);
        }

        else if (isspace(c))
            col++;

        else if (isalpha(c) || c == '_') {
            tkn.row = row;
            tkn.col = col;
            int k = 0;
            while (c != EOF && isalnum(c)){
                tkn.lexeme[k++] = c;
                c = fgetc(fin);
                col++;
            }
            tkn.lexeme[k] = '\0';
            if(!isKeyword(tkn.lexeme)){
                if(!searchTable(tkn.lexeme)){
                    int n_args = isFuncName(fin, c);
                    if(n_args == -1){
                        insertIntoTable(&tkn, n_args, "variable");
                    }
                    else{
                        if(strcmp(tkn.lexeme, "printf") == 0){
                            strcpy(dbuf, "int");
                        }
                        insertIntoTable(&tkn, n_args, "function");
                    }
                }
                strcpy(tkn.type,"identifier");
            }
            else if(isKeyword(tkn.lexeme))
                strcpy(tkn.type, "keyword");

            if(isDtype(tkn.lexeme))
                strcpy(dbuf, tkn.lexeme);

            gotToken = 1;
            fseek(fin, -1, SEEK_CUR);
        }
        else if (c == '/'){
            c = fgetc(fin);
            col++;
            if (c == '/'){
                while (c != EOF && c != '\n'){
                    c = fgetc(fin);
                    col++;
                }
                if (c == '\n')
                    newLine();
            }
            else if(c =='*'){
                do{
                    if (c == '\n')
                        newLine();
                    while (c != EOF && c != '*'){
                        c = fgetc(fin);
                        ++col;
                        if (c == '\n')
                            newLine();
                    }
                    c = fgetc(fin);
                    col++;
                } while (c != EOF && c != '/' && (++col));
                col++;
            }
            else{
                fillToken(&tkn, c, row, --col, "/");
                gotToken = 1;
                fseek(fin, -1, SEEK_CUR);
            }
        }
        else if (c == '"') {
            tkn.row = row;
            tkn.col = col;
            strcpy(tkn.type, "string literal");
            int k = 1;
            tkn.lexeme[0] = '"';
            while ((c = fgetc(fin)) != EOF && c != '"') {
                tkn.lexeme[k++] = c;
                col++;
            }
            tkn.lexeme[k] = '"';
            gotToken = 1;
        }
    }
    return tkn;
}
int main() {
    FILE *fin = fopen("input.c", "r");
    if (!fin) {
        printf("Error! File cannot be opened!\n");
        return 0;
    }
    struct token tkn;
    while ((tkn = getNextToken(fin)).row != -1)
        printf( "< %s , %d , %d >\n", tkn.type, tkn.row, tkn.col);
    printf("\n\nS.No.\t\tLexemeName\t\tDataType\t\tReturnType\tNumArgs\t\tTokenName\n");
    int num_entries=ind;
    printTable(num_entries);
    fclose(fin);
}