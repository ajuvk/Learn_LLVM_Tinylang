#ifndef TINYLANG_LEXER_TOKEN_H
#define TINYLANG_LEXER_TOKEN_H

#include "tinylang/Basic/LLVM.h"
#include "tinylang/Basic/TokenKinds.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SMLoc.h"

namespace tinylang{
    class Lexer;

    class Token{
        friend class Lexer;

        const char* Ptr;

        size_t Length;

        tok::TokenKind Kind;
    public:
        tok::TokenKind getKind() const {return Kind;}

        void setKind(tok::TokenKind K) {Kind  = K;}

        bool is(tok::TokenKind K) const {return Kind == K;}
        bool isNot(tok::TokenKind K) const {return Kind != K;}

        template<typename ...Tokens>
        bool isOneOf(Tokens && ...Toks) const{
            return (... || is(Toks));
        }

        const char* getName() const {
            return tok::getTokenName(Kind);
        }

        SMLoc getLocation() const {
            return SMLoc::getFromPointer(Ptr);
        }

        size_t getLength() const {return Length;}

        StringRef getIdentifier(){
            assert(is(tok::identifier) && "cannot get identifier on non-identifier");
            return StringRef(Ptr, Length);
        }

        StringRef getLiteralData() {
            assert(isOneOf(tok::integer_literal, tok::string_literal) && "cannot get literal data for non-literal");
            return StringRef(Ptr, Length);
        }

    };
}

#endif