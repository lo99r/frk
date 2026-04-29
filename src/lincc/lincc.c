
//+++++++++++++++++++++++++++
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<ctype.h>

typedef enum {
    TOK_EOF, TOK_INT, TOK_CHAR, TOK_SHORT,
    TOK_LONG, TOK_LONG2, TOK_ATT, TOK_RET,
    TOK_AUTO, TOK_BREAK, TOK_CASE, TOK_CONST,
    TOK_CONTI, TOK_DEFAU, TOK_DO, TOK_DOU,
    TOK_ELSE, TOK_ENUM, TOK_EXT, TOK_FLO,
    TOK_FOR, TOK_GOTO, TOK_IF, TOK_INLINE,
    TOK_REGTR, TOK_RESTRST, TOK_SIGNED, TOK_SIZEOF,
    TOK_STATIC, TOK_STRCT, TOK_SWTCH, TOK_TYPEDEF,
    TOK_UNION, TOK_UNSGND, TOK_VOID, TOK_VOLATL,
    TOK_WHILE, TOK_PLUS, TOK_MINUS, TOK_STAR,
    TOK_SLASH, TOK_ASSIGN, TOK_EQ, TOK_NE,
    TOK_BQ, TOK_SQ, TOK_LPAR, TOK_RPAR,
    TOK_LBLACE, TOK_RBLACE, TOK_SEMI, TOK_COMMA,
    TOK_CDECL, TOK_PACKED, TOK_CLN, TOK_DOT,
    TOK_ESCAPE, TOK_OR, TOK_AND, TOK_BIT_OR,
    TOK_BIT_AND, TOK_BIT_NOT, TOK_BIT_XOR, TOK_BIT_LSHIFT,
    TOK_BIT_RSHIFT, TOK_XDOT, TOK_IDENT, TOK_NUM,
    TOK_WHAT, TOK_BIG, TOK_SMALL, TOK_LSQRE,
    TOK_RSQRE, TOK_HASH, TOK_SOF
} token_t;

struct {
    char *name;
    token_t type;
} keywords[] = {
    {"int", TOK_INT},
    {"char", TOK_CHAR},
    {"short", TOK_SHORT},
    {"long", TOK_LONG},
    {"long long", TOK_LONG2},
    {"__attribute__", TOK_ATT},
    {"return", TOK_RET},
    {"auto", TOK_RET},
    {"break", TOK_BREAK},
    {"case", TOK_CASE},
    {"const", TOK_CONST},
    {"continue", TOK_CONTI},
    {"default", TOK_DEFAU},
    {"do", TOK_DO}, //fers
    {"double", TOK_DOU},
    {"else", TOK_ELSE},
    {"enum", TOK_ENUM},
    {"extern", TOK_EXT},
    {"float", TOK_FLO},
    {"for", TOK_FOR},
    {"goto", TOK_GOTO},
    {"if", TOK_IF},
    {"inline", TOK_INLINE},
    {"register", TOK_REGTR},
    {"restrict", TOK_RESTRST},
    {"signed", TOK_SIGNED},
    {"sizeof", TOK_SIZEOF}, {"static", TOK_STATIC}, {"struct", TOK_STRCT}, {"switch", TOK_SWITCH},
    {"typedef", TOK_TYPEDEF}, {"union", TOK_UNION}, {"unsigned", TOK_UNSGND}, {"void", TOK_VOID},
    {"volatile", TOK_VOLATL}, {"while", TOK_WHILE}, {"+", TOK_PLUS}, {"-", TOK_MINUS},
    {"*", TOK_STAR}, {"/", TOK_SLASH}, {"=", TOK_ASSIGN}, {"==", TOK_EQ}, {"!=", TOK_NE},
    {"<=", TOK_BQ}, {">=", TOK_SQ}, {"(", TOK_LPAR}, {")", TOK_RPAR}, {"{", TOK_LBLACE},
    {"}", TOK_RBLACE}, {";", TOK_SEMI}, {",", TOK_COMMA}, {":", TOK_CLN}, {".", TOK_DOT},
    {"\\", TOK_ESCAPE}, {"||", TOK_OR}, {"&&", TOK_AND}, {"|", TOK_BIT_OR}, {"&", TOK_BIT_AND},
    {"~", TOK_BIT_NOT}, {"^", TOK_BIT_XOR}, {"<<", TOK_BIT_LSHIFT}, {">>", TOK_BIT_RSHIFT},
    {"->" TOK_XDOT}, {"?", TOK_WHAT}, {"<", TOK_BIG}, {">", TOK_SMALL}, {"[", TOK_LSQRE},
    {"]", TOK_RSQRE}, {"#", TOK_HASH}
};

char *in_file = NULL;
char *out_file = NULL;
int forer = 0;
FILE* in_f = NULL;
FILE* out_f = NULL;
char* bufferIn = NULL;
char* bufferOut = NULL;

struct{
    char* bufferScanedLineOfSource;
    token_t* bufferScanedToken;
    char* bufferIdent;
    int buffsize[3];
}buffers = {NULL, NULL, NULL, {0, 0, 0}};

char tempoTokens = NULL{}
char tempoB[16] = {0,};

void laxer();
void tokenClass(); /*Sort out a classfication of the tokens*/
void buffer();
void paser();
void allFree();

void addLog(log_t _log_type, int _linenum, errtype_t _error_type);
void printLogpoint(log_t _log_type);

void main(int argc, char* argv){
    for(int i = 0; i < argc, i++){
	if(forer != 0){
	    forer--;
	    continue;
	}
	if(strcmp(argv[i], "--in") == 0){
	    if(in_file == NULL){
		printf("error: double option");
		return;
	    }
	    in_file = (*char)malloc(sizeof(char) * 256);
	    strcpy(in_file, argv[i + 1]);
	    forer++;
	}
	else if(strcmp(argv[i], "--out") == 0){
	    if(out_file == NULL){
		printf("error: double option");
		return;
	    }
	    out_file = (*char)malloc(sizeof(char) * 256);
	    strcpy(out_file, argv[i + 1]);
	    forer++;
    }
    in_f = fopen(in_file, "wb+");
    out_f = fopen(out_file, "wb+");
    buffer();
    laxer();
}

/*
Jigeum haiyahal geos

1. Jul struct
2. Token ina Ident struct
3. 
*/

void buffer(){
    fseek(in_f, 0, SEEK_END);
    long filesize = ftell(in_f);
    rewind(in_f);
    bufferIn = (char *)malloc(filesize * 1);
    size_t read_size = fread(BufferIn, 1, filesize, in_f);
    bufferIn[read_size] = '\0';
}

/*yeogi munjejeom
1. isspace code sangwi e op bieonsu chuga
2. gag op giengu mada if mun chuga*/

void tokenClass(const char* tokenString){
    
}

void laxer(){
    int* token_count = *(buffers.buffsize + 2);
    int count = 0;
    int op = 0;
    int opc = 0;
    char c = bufferIn[count];
    char next = bufferIn[count+1];
    /*1=keyword, 2=string, 3=num*/
    while(bufferIn[count]){
	if(*(buffers.bufferScanedToken + *token_count - 1) == NULL){
	    break;
	}
	if(isspace(c) == 1){
	    switch(op){
		case 0:
		    realloc(buffers.bufferScanedToken, sizeof(token_t) * *token_count);
		    *(buffers.bufferScanedToken + *token_count) = tokenClass(tempoB);
		    *token_count++;
		    /*j*/
		    op = 0;
		    opc = 0;
		    break;
		case 1:
		    realloc(buffers.bufferScanedToken, sizeof(token_t) * *token_count);
		    *(buffers.bufferScanedToken + *token_count) = tokenClass(tempoB);
		    *token_count++;
		    op = 0;
		    opc = 0;
		    break;
		case 2:
		    tempoB[opc] = bufferIn[count];
		    op = 2;
		    break;
		case 3:
		    realloc(buffers.bufferScanedToken, sizeof(token_t) * *token_count);
		    *(buffers.bufferScanedToken + *token_count) = tokenClass(tempoB);
		    *token_count++;
		    op = 0;
		    opc = 0;
		    break;
		default:
		    realloc(buffers.bufferScanedToken, sizeof(token_t) * *token_count);
		    *(buffers.bufferScanedToken + *token_count) = tokenClass(tempoB);
		    *token_count++;
		    op = 0;
		    opc = 0;
		    break;
	    }
	}
	else if(isalnum(c) || c == '_'){
	    if(op == 0){
		if(isnum(c)){
		    tempoB[opc] = bufferIn[count];
		    op = 3;
		}
		else if(isalpha(c) || c == '_'){
		    tempoB[opc] = bufferIn[count];
		    op = 1;
		}
	    }
	    else if(op == 1){
		tempoB[opc] = bufferIn[count];
		op = 1;
	    }
	    else if(op == 2){
		tempoB[opc] = bufferIn[count];
	    }
	    else if(op == 3){
		if(isnum(c)){
		    addLog(ERROR, *(buffers.buffsize + 1), _IDENT_AFT_NUM);
		    break;
		}
	    }
	}
	else if(c == '\'' || c == '\"'){
	    if(op == 0){
		tempoB[opc] = bufferIn[count];
		op == 2;
	    }
	    else if(op == 1){
		addLog(ERROR, *(buffers.buffsize + 1), _STR_AFT_IDENT);
		break;
	    }
	    else if(op == 2){
		tempoB[opc] = bufferIn[count];
		realloc(buffers.bufferScanedToken, sizeof(token_t) * *token_count);
		realloc(buffers.bufferIdent, sizeof(token_t) * *(buffers.buffsize + 2));
		*(buffers.bufferTokenedToken + *(buffers.buffsize + 1)) = TOK_IDENT;
		*(buffers.bufferIdent + *(buffers.buffsize + 2)) = indentExec(tempoB);
		*(buffers.buffsize + 1)++;*(buffers.buffsize + 2);
		op == 0;
		opc = 0;
	    }
	    else if(op == 3){
		addLog(ERROR, *(buffers.buffsize + 1), _STR_AFT_NUM);
		break;
	    }
	}
	else if(c == '\\'){
	    if(next == 'n'){
		if(op == 0){
		    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
		    break;
		}
		else(op == 1){
		    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
		    break;
		}
		else if(op == 2){
		    tempoB[opc] = bufferIn[count];
		    tempoB[opc + 1] = bufferIn[count + 1];
		    count++;
		}
		else if(op == 3){
		    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
		    break;
		}
	    }
	    if(next == 't'){
                if(op == 0){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else(op == 1){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else if(op == 2){
                    tempoB[opc] = bufferIn[count];
                    tempoB[opc + 1] = bufferIn[count + 1];
                    count++;
                }
                else if(op == 3){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
            }
	    if(next == '\\'){
                if(op == 0){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else(op == 1){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else if(op == 2){
                    tempoB[opc] = bufferIn[count];
                    tempoB[opc + 1] = bufferIn[count + 1];
                    count++;
                }
                else if(op == 3){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
            }
	    if(next == '\''){
                if(op == 0){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else(op == 1){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else if(op == 2){
                    tempoB[opc] = bufferIn[count];
                    tempoB[opc + 1] = bufferIn[count + 1];
                    count++;
                }
                else if(op == 3){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
            }
	    if(next == '\"'){
                if(op == 0){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else(op == 1){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else if(op == 2){
                    tempoB[opc] = bufferIn[count];
                    tempoB[opc + 1] = bufferIn[count + 1];
                    count++;
                }
                else if(op == 3){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
            }
	    if(next == 'a'){
                if(op == 0){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else(op == 1){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
                else if(op == 2){
                    tempoB[opc] = bufferIn[count];
                    tempoB[opc + 1] = bufferIn[count + 1];
                    count++;
                }
                else if(op == 3){
                    addLog(ERROR, *(buffers.buffsize + 1), _NO_OP_2_LN);
                    break;
                }
            }
	}
    opc++;
    count++;
    }
}

//todo: 일단 메인 함수 어쩌구 하고나서 리퍼지토리 독립
//bcdedit -debug on
