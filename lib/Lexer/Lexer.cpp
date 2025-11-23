#include "tinylang/Lexer/Lexer.h"

using namespace tinylang;

void KeywordFilter::addKeyword(StringRef Keyword, tok::TokenKind TokenCode){
    HashTable.insert(std::make_pair(Keyword,TokenCode));
}

void KeywordFilter::addKeywords(){
    #define KEYWORD(NAME, FLAGS) addKeyword(StringRef(#NAME), tok::kw_##NAME);
    #include "tinylang/Basic/TokenKinds.def"
}

namespace charinfo{
    LLVM_READNONE inline bool isASCII(char ch){
        return static_cast<unsigned char>(ch) <= 127;
    }

    LLVM_READNONE inline bool isVerticalWhitespace(char ch){
        return isASCII(ch) && (ch == '\r' || ch == '\n');
    }

    LLVM_READNONE inline bool isHorizontalWhitespace(char ch){
        return isASCII(ch) && (ch == ' ' || ch == '\t' || ch == '\f' || ch == '\v');
    }

    LLVM_READNONE inline bool isWhitespace(char ch){
        return isHorizontalWhitespace(ch) || isVerticalWhitespace(ch);
        
    }

    LLVM_READNONE inline bool isDigit(char ch){
        return isASCII(ch) && (ch >= '0' && ch <='9');
    }

    LLVM_READNONE inline bool isHexDigit(char ch){
        return isASCII(ch) && (isDigit(ch) || (ch >= 'A' && ch <= 'F'));
    }

    LLVM_READNONE inline bool isIdentifierHead(char ch){
        return isASCII(ch) && (ch == '_' || (ch >= 'A' && ch <= 'Z') ||
         (ch >= 'a' && ch <='z'));
    }

    LLVM_READNONE inline bool isIdentifierBody(char ch){
        return isIdentifierHead(ch) || isDigit(ch);
    }
}

void Lexer::identifier(Token& Result){
    const char* Start = CurPtr;
    const char* End = CurPtr + 1;
    while(charinfo::isIdentifierBody(*End)){
        ++End;
    }

    StringRef Name(Start, End-Start);
    formToken(Result, End, Keywords.getKeyword(Name, tok::identifier));
}

void Lexer::number(Token& Result){
    const char* End = CurPtr + 1;
    tok::TokenKind Kind = tok::unknown;
    bool IsHex = false;
    while(*End){
        if(!charinfo::isHexDigit(*End)){
            break;
        }

        if(!charinfo::isDigit(*End)){
            IsHex = true;
        }
        ++End;
    }

    switch(*End){
        case 'H':
            Kind = tok::integer_literal;
            ++End;
            break;
        default:
            if (IsHex){
                Diags.report(getLoc(), diag::err_hex_digit_in_decimal);
            }
            Kind = tok::integer_literal;
            break;
    }
    formToken(Result, End, Kind);
}

void Lexer::string(Token& Result){
    const char* Start = CurPtr;
    const char* End = CurPtr + 1;
    while(*End && *End != *Start && !charinfo::isVerticalWhitespace(*End)){
        ++End;
    }

    if(charinfo::isVerticalWhitespace(*End)){
        Diags.report(getLoc(),diag::err_unterminated_char_or_string);
    }

    formToken(Result, End+1, tok::string_literal);
}

void Lexer::comment(){
    const char* End = CurPtr+2;
    int Level = 1;
    while(*End && Level){

        if(*End == '(' && *(End+1) == '*'){
            End += 2;
            Level++;
        }else if (*End == '*' && *(End+1) == ')'){
            End += 2;
            --Level;
        }else{
            ++End;
        }
    }

    if(!*End){
        Diags.report(getLoc(),diag::err_unterminated_block_comment);
    }
    CurPtr = End;
}

void Lexer::formToken(Token& Result, const char* TokEnd, tok::TokenKind Kind){
    size_t TokLen = TokEnd - CurPtr;
    Result.Ptr = CurPtr;
    Result.Length = TokLen;
    Result.Kind = Kind;
    
    CurPtr = TokEnd;
}

void Lexer::next(Token& Result){
    while(*CurPtr && charinfo::isWhitespace(*CurPtr)){
        ++CurPtr;
    }

    if(!*CurPtr){
        Result.setKind(tok::eof);
        return ;
    }

    if(charinfo::isIdentifierHead(*CurPtr)){
        identifier(Result);
    }else if(charinfo::isDigit(*CurPtr)){
        number(Result);
    }else if (*CurPtr == '"' || *CurPtr == '\''){
        string(Result);
    }else{
        switch(*CurPtr){
            #define CASE(ch, tok) \
                case ch:          \
                    formToken(Result,CurPtr+1, tok);\
                    break
            CASE('=', tok::equal);
            CASE('#', tok::hash);
            CASE('+', tok::plus);
            CASE('-', tok::minus);
            CASE('*', tok::star);
            CASE('/', tok::slash);
            CASE(',', tok::comma);
            CASE('.', tok::period);
            CASE(';', tok::semi);            
            CASE(')', tok::r_paren);
            #undef CASE

            case '(':
                if (*(CurPtr+1) == '*'){
                    comment();
                    next(Result);
                }else{
                    formToken(Result, CurPtr + 1, tok::l_paren);                   
                }
                break;
            
            case ':' :
                if(*(CurPtr+1) == '='){
                    formToken(Result, CurPtr + 2, tok::colonequal);
                }else{
                     formToken(Result, CurPtr + 1, tok::colon);
                }
                break;
            
            case '<' :
                if (*(CurPtr+1) == '='){
                   formToken(Result, CurPtr + 2, tok::lessequal); 
                }else{
                    formToken(Result, CurPtr + 1, tok::less);
                }
                break;
            
            case '>' :
                if (*(CurPtr+1) == '='){
                   formToken(Result, CurPtr + 2, tok::greaterequal); 
                }else{
                    formToken(Result, CurPtr + 1, tok::greater);
                }
                break;
            default:
                Result.setKind(tok::unknown);
             
        }
    }
    
}

