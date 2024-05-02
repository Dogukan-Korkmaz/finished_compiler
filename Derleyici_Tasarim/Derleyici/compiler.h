#ifndef PEACHCOMPILER_H
#define PEACHCOMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Ardarda yazılan operatörleri algılamak için makro oluşturuyorum.
#define S_EQ(str, str2) \
        (str && str2 && (strcmp(str, str2) == 0))// strcmp: str ve str2 değerleri aynı ise True döndürür.


// Derlenecek karakterlerin pozisyon değerleri burada tutuluyor(satır, sütun ve dosya).
struct pos
{
    int line;
    int col;
    const char* filename;
};

// Numara karakterlerinin tanımlanması.
#define NUMERIC_CASE \
    case '0':       \
    case '1':       \
    case '2':       \
    case '3':       \
    case '4':       \
    case '5':       \
    case '6':       \
    case '7':       \
    case '8':       \
    case '9' 

// Operatör karakterlerinin tanımlanması.
#define OPERATOR_CASE_EXCLUDING_DIVISION \
    case '+':                           \
    case '-':                           \
    case '*':                           \
    case '>':                           \
    case '<':                           \
    case '^':                           \
    case '%':                           \
    case '!':                           \
    case '=':                           \
    case '~':                           \
    case '|':                           \
    case '&':                           \
    case '(':                           \
    case '[':                           \
    case ',':                           \
    case '.':                           \
    case '?'                          

// Sembol karakterlerinin tanımalnması.
#define SYMBOL_CASE \
    case '{':       \
    case '}':       \
    case ':':       \
    case ';':       \
    case '#':       \
    case '\\':      \
    case ')':       \
    case ']'  

// Analiz işlemi sonrası geri bildirim mesajları burada tutuluyor..      
enum
{
    LEXICAL_ANALYSIS_ALL_OK,
    LEXICAL_ANALYSIS_INPUT_ERROR
};

// Token tipleri için.(yeni satır,sembol,string,yorum satırı...vb)
enum
{
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_SYMBOL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_NEWLINE
};

// Sayı tipleri burada tutuluyor.
enum
{
    NUMBER_TYPE_NORMAL,
    NUMBER_TYPE_LONG,
    NUMBER_TYPE_FLOAT,
    NUMBER_TYPE_DOUBLE
};

// Sözcüksel analiz[1] için token yapıları tanımlanması.
struct token
{
    int type;
    int flags;
    struct pos pos; // Token dosyanın neresinde olduğunu anlamak için değişken ataması.

    // Union içine yazılan herşey aynı belleği paylaşır.
    union
    {
        char cval;
        const char* sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void* any;
    };
    
    struct token_number
    {
        int type;
    } num;

    // Eğer tokenler arası boşluk varsa True.
    bool whitespace;

    const char* between_brackets;

};

// Sözcükler arası geçiş için gerekli fonksyonlar.
struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process* process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process* process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process* process, char c);

struct lex_process_functions
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

// Sözcüksel analiz işlemi.(Sözcük derken string anlamında değil her şey olabilir.Örn. 3, / ,  , ü)
struct lex_process
{
    struct pos pos;
    struct vector* token_vec;
    struct compile_process* compiler;

    // Lex(sözcük) kaç satır arasında olduğunu tutan değişken.örn. ((50))
    int current_expression_count;
    struct buffer* parentheses_buffer;
    struct lex_process_functions* function;

    /* Derleyicinin anlayamadığı ama kullanıcının anlayabildiği veriler için.*/
    void* private;
};

// Hata mesajları burada tutuluyor.
enum
{
    COMPILER_FILE_COMPILED_OK,
    COMPILER_FAILED_WITH_ERRORS
};

struct compile_process
{
    /*Bu dosyanın nasıl derlenmesi gerektiğine ilişkin bayraklar.*/
    int flags;

    struct pos pos;  // Sözcüksel analiz[3], sözcüğün pozisyonunu(satır,sütun) öğrenmek için değişken tanımlanması.
    struct compile_process_input_file
    {
        FILE* fp;
        const char* abs_path;
    } cfile;

    struct vector* token_vec;
    FILE* ofile;
};

/*Derleyici, derleyicimiz boyunca erişilmesi gereken işlevler için tüm prototipleri içerecektir.*/
int compile_file(const char* filename, const char* out_filename, int flags);
struct compile_process *compile_process_create(const char *filename, const char *filename_out, int flags);


char compile_process_next_char(struct lex_process* lex_process);
char compile_process_peek_char(struct lex_process* lex_process);
void compile_process_push_char(struct lex_process* lex_process, char c);


void compiler_error(struct compile_process* compiler, const char* msg, ...);
void compiler_warning(struct compile_process* compiler, const char* msg, ...);

struct lex_process* lex_process_create(struct compile_process* compiler, struct lex_process_functions* functions, void* private);
void lex_process_free(struct lex_process* process);
void* lex_process_private(struct lex_process* process);
struct vector* lex_process_tokens(struct lex_process* process);
int lex(struct lex_process* process);

/**
 * @brief
 * 
 * @param compiler 
 * @param str 
 * @return struct lex_process* 
 */
struct lex_process* tokens_build_for_string(struct compile_process* compiler, const char* str);

bool token_is_keyword(struct token* token, const char* value);

#endif