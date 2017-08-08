#include<wctype.h>
#include<ctype.h>
#include<string.h>

#include<vm/cpu.h>
#include<parse/lex.h>
#include<parse/rbuff.h>
#include<alloc.h>
#include<log.h>

RBUFF_IMPL(lex_la_buff, wchar_t, MAX_LEX_LOOK)

/* inform user of fatal error
 * and jump to exception handler */
#define lex_err(state, fmt, ...) \
{ \
    printf("[  error  ] :"); \
    printf(" " fmt "\n", ##__VA_ARGS__); \
    longjmp(*(state)->err, 1); \
}

static void lex_advance(struct lex_state *);
static wchar_t la(struct lex_state *);

static inline wchar_t readwc(FILE *f)
{
    wint_t c = fgetwc(f);
    return c == WEOF ? L'\0' : (wchar_t)c;
}

void lex_init(struct lex_state *state, FILE *f, jmp_buf *b)
{
    state->file = f;
    state->line_no = 1;
    state->col_no = 1;
    lex_la_buff_init(&state->la_chars);
    for(size_t i = 0; i < MAX_LEX_LOOK; ++i){
        *lex_la_buff_push_back(&state->la_chars) = readwc(state->file);
    }
    state->err = b;
}

static inline wchar_t la(struct lex_state *state)
{
    return *lex_la_buff_front(&state->la_chars);
}

static void lex_advance(struct lex_state *state)
{
    wchar_t popped = *lex_la_buff_pop_front(&state->la_chars);
    ++state->col_no;
    if(popped == L'\n'){
        ++state->line_no;
        state->col_no = 1;
    }
    *lex_la_buff_push_back(&state->la_chars) = readwc(state->file);
}

static inline int is_id_start(wint_t c)
{
    return iswalpha(c) || c == L'_';
}

static inline int is_id_body(wint_t c)
{
    return iswalpha(c) || iswdigit(c) || c == '_';
}

static inline int iswbin(wint_t c)
{
    return c == L'1' || c == L'0';
}

static inline int lex_number(
    struct lex_state *state,
    struct token *res,
    size_t base)
{
    int(*test)(wint_t) = NULL;
    switch(base){
    case 0:
    case 10:
        test = iswdigit;
        break;
    case 2:
        test = iswbin;
        break;
    case 16:
        test = iswxdigit;
        break;
    default:
        return 1;
    };
    size_t i = 0;
    wchar_t c = 0;
    while(test((c = la(state)))){
        if(i == MAX_LEXEME - 1){
            lex_err(state, "number does not fit into max lexeme size of %d", MAX_LEXEME);
            break;
        }
        res->lexeme[i++] = c;
        lex_advance(state);
    }
    res->data = wcstoll(res->lexeme, NULL, base);
    return 0;
}

static inline int is_reg(const wchar_t *lexeme)
{
    if(lexeme[0] != L'r') return 0;
    wchar_t next = L'\0';
    while((next = *++lexeme)){
        if(!iswdigit(next)){
            return 0;
        }
    }
    return 1;
}

/* test id for possible other lexemes */
static inline int check_id(struct lex_state *state, struct token *tok)
{
    if(tok->type != TOK_ID){
        return 1;
    }
    if(is_reg(tok->lexeme)){
        tok->type = TOK_REG;
        tok->data = wcstoll(tok->lexeme + 1, NULL, 10);
        if(tok->data >= GENERAL_REGS || tok->data < 0){
            lex_err(state, "invalid register constant at l:%u c:%u: %ls",
                           state->line_no, state->col_no, tok->lexeme);
        }
    }
    return 0;
}

int lex_next(struct lex_state *state, struct token *res)
{
    err_on(!res, "Token not allocated");
    while(la(state) == L'\t' || la(state) == L' '){
        lex_advance(state);
    }
    /* remove comments */
    if(la(state) == L';'){
        while(la(state) && la(state) != L'\n'){
            lex_advance(state);
        }
    }

    #define CASES(c, t, str) \
        case c: \
            res->type = t; \
            wcsncpy(res->lexeme, str, MAX_LEXEME); \
            lex_advance(state); \
            break

    int err = 0;
    switch(la(state)){
    /* TODO remove or implement fully
    case L'.':
        lex_advance(state);
        if(iswalpha(la(state))){
            res->type = TOK_DOT_CHAR;
            res->lexeme[0] = L'.';
            res->lexeme[1] = la(state);
            lex_advance(state);
            break;
        }else{
            lex_err(state, "expected letter after '.'");
        }
        break;
    */
    case L'#':
        res->type = TOK_NUM;
        lex_advance(state);
        switch(la(state)){
        case L'x': lex_advance(state); lex_number(state, res, 16); break;
        case L'b': lex_advance(state); lex_number(state, res, 2); break;
        default:
            if(iswdigit(la(state))){
                lex_number(state, res, 10);
            }else{
                lex_err(state, "expected number, got '%lc'", la(state));
            }
        };
        break;
    /* Address literal */
    case L'@':
        res->type = TOK_ADDR;
        lex_advance(state);
        lex_number(state, res, 16);
        break;
    CASES(L':', TOK_COLON, L":");
    CASES(L',', TOK_COMMA, L",");
    CASES(L'\n', TOK_EOL,  L"EOL");
    CASES(L'\0', TOK_EOS,  L"EOS");
    default:
        if(is_id_start(la(state))){
            res->type = TOK_ID;
            wchar_t c = 0;
            size_t index = 0;
            while(is_id_body((c = la(state)))){
                if(index >= MAX_LEXEME - 1){
                    lex_err(state, "id does not fit into max lexeme size of %d", MAX_LEXEME);
                    break;
                }
                res->lexeme[index++] = c;
                lex_advance(state);
            }
            err = check_id(state, res);
        }else{
            lex_err(state, "couldn't match character %lc", la(state));
        }
        break;
    };

    res->line_no = state->line_no;
    res->col_no = state->col_no;

    #undef CASES

    return err;
}
