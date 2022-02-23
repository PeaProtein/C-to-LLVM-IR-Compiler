#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CODE_LENGTH 1000
#define MAX_LEXEME_LENGTH 11
#define MAX_SYMBOL_TABLE_SIZE 100

char sourceCode[MAX_CODE_LENGTH] = "";

enum tokenTypes
{
    printsym = 1, readsym, identsym, elsesym, numbersym, assignsym, plussym, minussym, lcurlysym,
    notsym, eqsym, noteqsym, gtsym, ltsym, andsym, orsym, multsym, divsym, modsym, ifsym, whilesym,
    lparensym, rparensym, semicolonsym, intsym, rcurlysym
};

typedef struct lexeme
{
    char lexeme[MAX_LEXEME_LENGTH + 1];
    int tokenType;
} Lexeme;

Lexeme lexemes[MAX_CODE_LENGTH];

char symbolTable[MAX_SYMBOL_TABLE_SIZE][MAX_LEXEME_LENGTH + 1];

Lexeme token;
int regCount = 1, labelCount = 1, tokenIdx = 0, tableIdx = 0;

void readSourceCode(char*);
void lexicalAnalyzer();
void printBoilerplate();
void program();
void declaration();
void statement();
char* expression(char*);
char* expressionPrime(char*);
char* andEx(char*);
char* andPrime(char*);
char* equality(char*);
char* equalityPrime(char*);
char* relOp(char*);
char* relOpPrime(char*);
char* mathEx(char*);
char* mathExPrime(char*);
char* term(char*);
char* termPrime(char*);
char* factor(char*);
void getToken();
void enter();
int lookup();
void error(int);

int main(int argc, char** argv)
{
    readSourceCode(argv[1]);

    lexicalAnalyzer();

    printBoilerplate();

	program();

    printf("ret i32 0\n}\n");

    return 0;
}

void readSourceCode(char* fileName)
{
    FILE* in = fopen(fileName, "r");
    fscanf(in, "%[^EOF]s", sourceCode);
    fclose(in);
}

void lexicalAnalyzer()
{
    int sx, lx;

    for (sx = 0, lx = 0; sourceCode[sx]; sx++, lx++)
    {
        // Whitespace
        if (isspace(sourceCode[sx]))
        {
            lx--;
            continue;
        }
        // print
        if (sourceCode[sx] == 'p'
            && sourceCode[sx + 1] == 'r'
            && sourceCode[sx + 2] == 'i'
            && sourceCode[sx + 3] == 'n'
            && sourceCode[sx + 4] == 't')
        {
            lexemes[lx].tokenType = printsym;
            strcpy(lexemes[lx].lexeme, "print");
            sx += 4;
            continue;
        }
        // read
        if (sourceCode[sx] == 'r'
            && sourceCode[sx + 1] == 'e'
            && sourceCode[sx + 2] == 'a'
            && sourceCode[sx + 3] == 'd')
        {
            lexemes[lx].tokenType = readsym;
            strcpy(lexemes[lx].lexeme, "read");
            sx += 3;
            continue;
        }
        // int
        if (sourceCode[sx] == 'i'
            && sourceCode[sx + 1] == 'n'
            && sourceCode[sx + 2] == 't')
        {
            lexemes[lx].tokenType = intsym;
            strcpy(lexemes[lx].lexeme, "int");
            sx += 2;
            continue;
        }
        // if
        if (sourceCode[sx] == 'i'
            && sourceCode[sx + 1] == 'f')
        {
            lexemes[lx].tokenType = ifsym;
            strcpy(lexemes[lx].lexeme, "if");
            sx += 1;
            continue;
        }
        // else
        if (sourceCode[sx] == 'e'
            && sourceCode[sx + 1] == 'l'
            && sourceCode[sx + 2] == 's'
            && sourceCode[sx + 3] == 'e')
        {
            lexemes[lx].tokenType = elsesym;
            strcpy(lexemes[lx].lexeme, "else");
            sx += 3;
            continue;
        }
        // while
        if (sourceCode[sx] == 'w'
            && sourceCode[sx + 1] == 'h'
            && sourceCode[sx + 2] == 'i'
            && sourceCode[sx + 3] == 'l'
            && sourceCode[sx + 4] == 'e')
        {
            lexemes[lx].tokenType = whilesym;
            strcpy(lexemes[lx].lexeme, "while");
            sx += 4;
            continue;
        }
        // &&
        if (sourceCode[sx] == '&'
            && sourceCode[sx + 1] == '&')
        {
            lexemes[lx].tokenType = andsym;
            strcpy(lexemes[lx].lexeme, "and");
            sx += 1;
            continue;
        }
        // ||
        if (sourceCode[sx] == '|'
            && sourceCode[sx + 1] == '|')
        {
            lexemes[lx].tokenType = orsym;
            strcpy(lexemes[lx].lexeme, "or");
            sx += 1;
            continue;
        }
        // ==
        if (sourceCode[sx] == '='
            && sourceCode[sx + 1] == '=')
        {
            lexemes[lx].tokenType = eqsym;
            strcpy(lexemes[lx].lexeme, "icmp eq");
            sx += 1;
            continue;
        }
        // !=
        if (sourceCode[sx] == '!'
            && sourceCode[sx + 1] == '=')
        {
            lexemes[lx].tokenType = noteqsym;
            strcpy(lexemes[lx].lexeme, "icmp ne");
            sx += 1;
            continue;
        }
        // !
        if (sourceCode[sx] == '!')
        {
            lexemes[lx].tokenType = notsym;
            strcpy(lexemes[lx].lexeme, "!");
            continue;
        }
        // >
        if (sourceCode[sx] == '>')
        {
            lexemes[lx].tokenType = gtsym;
            strcpy(lexemes[lx].lexeme, "icmp sgt");
            continue;
        }
        // <
        if (sourceCode[sx] == '<')
        {
            lexemes[lx].tokenType = ltsym;
            strcpy(lexemes[lx].lexeme, "icmp slt");
            continue;
        }
        // {
        if (sourceCode[sx] == '{')
        {
            lexemes[lx].tokenType = lcurlysym;
            strcpy(lexemes[lx].lexeme, "{");
            continue;
        }
        // }
        if (sourceCode[sx] == '}')
        {
            lexemes[lx].tokenType = rcurlysym;
            strcpy(lexemes[lx].lexeme, "}");
            continue;
        }
        // +
        if (sourceCode[sx] == '+')
        {
            lexemes[lx].tokenType = plussym;
            strcpy(lexemes[lx].lexeme, "add nsw");
            continue;
        }
        // Minus
        if (sourceCode[sx] == '-' &&
            (lexemes[lx - 1].tokenType == identsym ||
            lexemes[lx - 1].tokenType == numbersym ||
            lexemes[lx - 1].tokenType == rparensym))
        {
            lexemes[lx].tokenType = minussym;
            strcpy(lexemes[lx].lexeme, "sub nsw");
            continue;
        }
        // *
        if (sourceCode[sx] == '*')
        {
            lexemes[lx].tokenType = multsym;
            strcpy(lexemes[lx].lexeme, "mul nsw");
            continue;
        }
        // /
        if (sourceCode[sx] == '/')
        {
            lexemes[lx].tokenType = divsym;
            strcpy(lexemes[lx].lexeme, "sdiv");
            continue;
        }
        // %
        if (sourceCode[sx] == '%')
        {
            lexemes[lx].tokenType = modsym;
            strcpy(lexemes[lx].lexeme, "srem");
            continue;
        }
        // =
        if (sourceCode[sx] == '=')
        {
            lexemes[lx].tokenType = assignsym;
            strcpy(lexemes[lx].lexeme, "=");
            continue;
        }
        // (
        if (sourceCode[sx] == '(')
        {
            lexemes[lx].tokenType = lparensym;
            strcpy(lexemes[lx].lexeme, "(");
            continue;
        }
        // )
        if (sourceCode[sx] == ')')
        {
            lexemes[lx].tokenType = rparensym;
            strcpy(lexemes[lx].lexeme, ")");
            continue;
        }
        // ;
        if (sourceCode[sx] == ';')
        {
            lexemes[lx].tokenType = semicolonsym;
            strcpy(lexemes[lx].lexeme, ";");
            continue;
        }
        // Identifiers
        if (isalpha(sourceCode[sx]))
        {
            char identifier[MAX_LEXEME_LENGTH + 1] = {sourceCode[sx++]};
            int identIndex;

            for (identIndex = 1; isalnum(sourceCode[sx]); identIndex++, sx++)
                identifier[identIndex] = sourceCode[sx];

            sx--;
            lexemes[lx].tokenType = identsym;
            strcpy(lexemes[lx].lexeme, identifier);
            continue;
        }
        // Negative and non-negative numbers. If minus condition failed we know its a negative.
        if (isdigit(sourceCode[sx]) || sourceCode[sx] == '-')
        {
            char number[MAX_LEXEME_LENGTH + 1] = {sourceCode[sx++]};
            int numIndex;

            for (numIndex = 1; isdigit(sourceCode[sx]); numIndex++, sx++)
                number[numIndex] = sourceCode[sx];

            sx--;
            lexemes[lx].tokenType = numbersym;
            strcpy(lexemes[lx].lexeme, number);
            continue;
        }
    }
}

void printBoilerplate()
{
    printf("target triple = \"x86_64-pc-linux-gnu\"\n\ndeclare i32 @printf(i8*, ...) #1\n@.str = "
        "private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", align 1\n\ndefine void "
        "@print_integer(i32) #0 {\n\t%%2 = alloca i32, align 4\n\tstore i32 %%0, i32* %%2, align "
        "4\n\t%%3 = load i32, i32* %%2, align 4\n\t%%4 = call i32 (i8*, ...) @printf(i8* "
        "getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %%3)\n\tret void\n}"
        "\n\n%%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*"
        ", %%struct._IO_marker*, %%struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, "
        "%%struct._IO_codecvt*, %%struct._IO_wide_data*, %%struct._IO_FILE*, i8*, i64, i32, [20 x "
        "i8] }\n%%struct._IO_marker = type opaque\n%%struct._IO_codecvt = type opaque\n%%struct._"
        "IO_wide_data = type opaque\n\n@stderr = external dso_local global %%struct._IO_FILE*, "
        "align 8\n@.str.1 = private unnamed_addr constant [25 x i8] c\"please enter an integer\\0A"
        "\\00\", align 1\n@.str.2 = private unnamed_addr constant [3 x i8] c\"%%d\\00\", align 1\n"
        "@.str.3 = private unnamed_addr constant [6 x i8] c\"scanf\\00\", align 1\n@.str.4 = private "
        "unnamed_addr constant [24 x i8]\nc\"no matching characters\\0A\\00\", align 1\n\ndeclare i32* @_"
        "_errno_location() #2\ndeclare i32 @__isoc99_scanf(i8*, ...) #1\ndeclare void @perror(i8*) #1\n"
        "declare void @exit(i32) #3\ndeclare i32 @fprintf(%%struct._IO_FILE*, i8*, ...) #1\n\n"
        "define i32 @read_integer() #0 {\n\t%%1 = alloca i32, align 4\n\t%%2 = alloca i32, align 4"
        "\n\t%%3 = call i32* @__errno_location() #4\n\tstore i32 0, i32* %%3, align 4\n\t%%4 = "
        "load %%struct._IO_FILE*, %%struct._IO_FILE** @stderr, align 8\n\t%%5 = call i32 "
        "(%%struct._IO_FILE*, i8*, ...) @fprintf(%%struct._IO_FILE* %%4, i8* getelementptr "
        "inbounds ([25 x i8], [25 x i8]* @.str.1, i32 0, i32 0))\n\t%%6 = call i32 (i8*, ...) "
        "@__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i32 0, i32 0), "
        "i32* %%1)\n\tstore i32 %%6, i32* %%2, align 4\n\t%%7 = load i32, i32* %%2, align 4\n\t"
        "%%8 = icmp eq i32 %%7, 1\n\tbr i1 %%8, label %%9, label %%11\n\n; <label>:9:             "
        "                         ; preds = %%0\n\t%%10 = load i32, i32* %%1, align 4\n\tret i32 "
        "%%10\n\n; <label>:11:                                     ; preds = %%0\n\t%%12 = call "
        "i32* @__errno_location() #4\n\t%%13 = load i32, i32* %%12, align 4\n\t%%14 = icmp ne i32 "
        "%%13, 0\n\tbr i1 %%14, label %%15, label %%16\n\n; <label>:15:                           "
        "          ; preds = %%11\n\tcall void @perror(i8* getelementptr inbounds ([6 x i8], [6 x "
        "i8]* @.str.3, i32 0, i32 0))\n\tcall void @exit(i32 1) #5\n\tunreachable\n\n; <label>:16:"
        "                                     ; preds = %%11\n\t%%17 = load %%struct._IO_FILE*, "
        "%%struct._IO_FILE** @stderr, align 8\n\t%%18 = call i32 (%%struct._IO_FILE*, i8*, ...) "
        "@fprintf(%%struct._IO_FILE* %%17, i8* getelementptr inbounds ([24 x i8], [24 x i8]* "
        "@.str.4, i32 0, i32 0))\n\tcall void @exit(i32 1) #5\n\tunreachable\n}\n\ndefine i32 "
        "@main() #0 {\n\t");
}

void program()
{
    do
    {
        getToken();
        declaration();
    }
    while (token.tokenType == semicolonsym);

    tokenIdx--;

    do
    {
        getToken();
        statement();
    }
    while (token.tokenType != 0);
}

void declaration()
{
    if (token.tokenType == intsym)
    {
        getToken();
        if (lookup() != -1)
            error(2);
        enter();
        printf("%%t%d = alloca i32\n\t", regCount++);
        getToken();
    }
}

void statement()
{
    if (token.tokenType == printsym)
    {
		char mathExPrimeRes[MAX_LEXEME_LENGTH + 1];
        getToken();
        printf("call void @print_integer(i32 %s)\n\t", mathEx(mathExPrimeRes));
    }

    else if (token.tokenType == readsym)
    {
        printf("%%t%d = call i32 @read_integer()\n\t", regCount);
        getToken();

        int index = lookup();
        if (index == -1)
            error(1);

        printf("store i32 %%t%d, i32* %%t%d\n\t", regCount++, index);
        getToken();
        tokenIdx--;
    }

    else if (token.tokenType == identsym)
    {
        int index = lookup();
        if (index == -1)
            error(1);

        getToken();
        getToken();
        char mathExPrimeRes[MAX_LEXEME_LENGTH + 1];
        printf("store i32 %s, i32* %%t%d\n\t", mathEx(mathExPrimeRes), index);
    }

    else if (token.tokenType == ifsym)
    {
        char exPrimeRes[MAX_LEXEME_LENGTH + 1];
        int ifLabel = labelCount++;
        int elseOrEndLabel = labelCount++;

        getToken();
        getToken();
        printf("br i1 %s, label %%label%d, label %%label%d\nlabel%d:\n\t", expression(exPrimeRes),
            ifLabel, elseOrEndLabel, ifLabel);

        getToken();
        getToken();
        statement();
        getToken();

        if (token.tokenType == semicolonsym)
            getToken();

        if (token.tokenType == elsesym)
        {
            printf("br label %%label%d\nlabel%d:\n\t", labelCount, elseOrEndLabel);
            getToken();
            statement();
            printf("br label %%label%d\nlabel%d:\n\t", labelCount, labelCount);
            labelCount++;
        }

        else
        {
            printf("br label %%label%d\nlabel%d:\n\t", elseOrEndLabel, elseOrEndLabel);
            statement();
        }
    }

    else if (token.tokenType == whilesym)
    {
        char exPrimeRes[MAX_LEXEME_LENGTH + 1];
        int headLabel = labelCount++;
        int bodyLabel = labelCount++;
        int endLabel = labelCount++;

        getToken();
        getToken();
        printf("br label %%label%d\nlabel%d:\n\t", headLabel, headLabel);
        printf("br i1 %s, label %%label%d, label %%label%d\nlabel%d:\n\t", expression(exPrimeRes),
            bodyLabel, endLabel, bodyLabel);

        getToken();
        getToken();
        statement();

        printf("br label %%label%d\nlabel%d:\n\t", headLabel, endLabel);
    }

    else if (token.tokenType == lcurlysym)
    {
        while (token.tokenType != rcurlysym)
        {
            getToken();
            statement();
        }
    }
}

char* expression(char* exPrimeRes)
{
    char left[MAX_LEXEME_LENGTH + 1];
	char andPrimeRes[MAX_LEXEME_LENGTH + 1];
	
	strcpy(left, andEx(andPrimeRes));
	getToken();
	strcpy(exPrimeRes, expressionPrime(left));

    return exPrimeRes;
}

char* expressionPrime(char* left)
{
	if (token.tokenType != orsym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char andPrimeRes[MAX_LEXEME_LENGTH + 1];
	char opStr[3];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, andEx(andPrimeRes));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i1 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	expressionPrime(regist);
}

char* andEx(char* andPrimeRes)
{
    char left[MAX_LEXEME_LENGTH + 1];
	char equalityPrimeRes[MAX_LEXEME_LENGTH + 1];
	
	strcpy(left, equality(equalityPrimeRes));
	getToken();
	strcpy(andPrimeRes, andPrime(left));

    return andPrimeRes;
}

char* andPrime(char* left)
{
	if (token.tokenType != andsym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char equalityPrimeRes[MAX_LEXEME_LENGTH + 1];
	char opStr[4];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, equality(equalityPrimeRes));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i1 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	andPrime(regist);
}

char* equality(char* equalityPrimeRes)
{
    char left[MAX_LEXEME_LENGTH + 1];
	char relOpPrimeRes[MAX_LEXEME_LENGTH + 1];
	
	strcpy(left, relOp(relOpPrimeRes));
	getToken();
	strcpy(equalityPrimeRes, equalityPrime(left));

    return equalityPrimeRes;
}

char* equalityPrime(char* left)
{
	if (token.tokenType != eqsym && token.tokenType != noteqsym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char relOpPrimeRes[MAX_LEXEME_LENGTH + 1];
	char opStr[8];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, relOp(relOpPrimeRes));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i32 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	equalityPrime(regist);
}

char* relOp(char* relOpPrimeRes)
{
    char left[MAX_LEXEME_LENGTH + 1];
	char mathExPrimeRes[MAX_LEXEME_LENGTH + 1];
	
	strcpy(left, mathEx(mathExPrimeRes));
	getToken();
	strcpy(relOpPrimeRes, relOpPrime(left));

    return relOpPrimeRes;
}

char* relOpPrime(char* left)
{
	if (token.tokenType != ltsym && token.tokenType != gtsym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char mathExPrimeRes[MAX_LEXEME_LENGTH + 1];
	char opStr[9];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, mathEx(mathExPrimeRes));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i32 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	relOpPrime(regist);
}

char* mathEx(char* mathExPrimeRes)
{
    char left[MAX_LEXEME_LENGTH + 1];
	char termPrimeResult[MAX_LEXEME_LENGTH + 1];
	
	strcpy(left, term(termPrimeResult));
	getToken();
	strcpy(mathExPrimeRes, mathExPrime(left));

    return mathExPrimeRes;
}

char* mathExPrime(char* left)
{
	if (token.tokenType != plussym && token.tokenType != minussym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char termPrimeResult[MAX_LEXEME_LENGTH + 1];
	char opStr[8];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, term(termPrimeResult));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i32 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	mathExPrime(regist);
}

char* term(char* termPrimeResult)
{
	char left[MAX_LEXEME_LENGTH + 1];
	char result[MAX_LEXEME_LENGTH + 1];

	strcpy(left, factor(result));
	getToken();
	strcpy(termPrimeResult, termPrime(left));

	return termPrimeResult;
}

char* termPrime(char* left)
{
	if (token.tokenType != multsym && token.tokenType != divsym && token.tokenType != modsym)
	{
		tokenIdx--;
		return left;
	}

	char right[MAX_LEXEME_LENGTH + 1];
	char regist[MAX_LEXEME_LENGTH + 1];
	char result[MAX_LEXEME_LENGTH + 1];
	char opStr[8];
	strcpy(opStr, token.lexeme);

	getToken();
	strcpy(right, factor(result));
	sprintf(regist, "%%t%d", regCount++);

	printf("%s = %s i32 %s, %s\n\t", regist, opStr, left, right);

	getToken();
	termPrime(regist);
}

char* factor(char* result)
{
	if (token.tokenType == numbersym)
		return token.lexeme;

    else if (token.tokenType == identsym)
    {
        int index = lookup();
        if (index == -1)
            error(1);

        sprintf(result, "%%t%d", regCount++);
        printf("%s = load i32, i32* %%t%d\n\t", result, index);
        return result;
    }

    else if (token.tokenType == notsym)
    {
        char exPrimeRes[MAX_LEXEME_LENGTH + 1];
        getToken();
        getToken();
        sprintf(result, "%%t%d", regCount++);
        printf("%s = xor i1 %s, 1\n\t", result, expression(exPrimeRes));
        getToken();
        return result;
    }

    else if (token.tokenType == lparensym)
	{
		char exPrimeRes[MAX_LEXEME_LENGTH + 1];
		getToken();
		strcpy(result, expression(exPrimeRes));
		getToken();
		return result;
	}
}

void getToken()
{
    token = lexemes[tokenIdx++];
}

void enter()
{
    strcpy(symbolTable[tableIdx++], token.lexeme);
}

int lookup()
{
    int i;

    for (i = 0; i < tableIdx; i++)
    {
        if (!strcmp(token.lexeme, symbolTable[i]))
            return i + 1;
    }

    return -1;
}

void error(int errorCode)
{
    switch (errorCode)
    {
        case 1:
            fprintf(stderr, "error: use of undeclared variable %s", token.lexeme);
            break;
        case 2:
            fprintf(stderr, "error: multiple definitions of %s", token.lexeme);
            break;
    }
    exit(EXIT_FAILURE);
}
