/** https://www.json.org/json-en.html
 *
 *
 *
 *
 *
 */
#pragma once

#include <string>
#include <vector>
#include <format>

#include "format.h"

struct Token
{
    struct Location
    {
        int start;
        int length;
    };
    enum Tag
    {
        END = 0,
        // JsonObject
        LEFT_BRACKET,
        RIGHT_BRACKET,
        // JsonArray
        LEFT_SQUARE_BRACKET,
        RIGHT_SQUARE_BRACKET,
        // Syntax
        COMMA,
        COLUMN,
        // Values
        STRING_LITERAL, // 7
        NUMBER_LIRETAL,
        NULLABLE,
        BOOLEAN_TRUE,
        BOOLEAN_FALSE,
        INVALID,
    };
    Tag tag;
    Location loc;
};

template<>                                                   
struct std::formatter<Token> {
  constexpr auto parse(std::format_parse_context& ct) {
    return ct.begin();
  }
  auto format(const Token& val, std::format_context& ct) const { 
    return std::format_to(ct.out(), "{{ Tag:{:3}, Location:{{{:3}, {:2} }} }}", static_cast<int>(val.tag), val.loc.start, val.loc.length);
  }
};

struct Lexer
{
    std::string str;
    int index = -1;
    int max = str.length();

    Token next()
    {
        Token token = {Token::Tag::INVALID, Token::Location{index, 0}};
        while (true) {
            index++;    
            if (index >= max) {
                token.tag = Token::Tag::END;
                return token;
            }
            switch (str[index]) {
                case '{':
                    token.tag = Token::Tag::LEFT_BRACKET;
                    token.loc.start = index;
                    return token;
                case '}':
                    token.tag = Token::Tag::RIGHT_BRACKET;
                    token.loc.start = index;
                    return token;
                case ':':
                    token.tag = Token::Tag::COLUMN;
                    token.loc.start = index;
                    return token;
                case ',':
                    token.tag = Token::Tag::COMMA;
                    token.loc.start = index;
                    return token;
                case '[':
                    token.tag = Token::Tag::LEFT_SQUARE_BRACKET;
                    token.loc.start = index;
                    return token;
                case ']':
                    token.tag = Token::Tag::RIGHT_SQUARE_BRACKET;
                    token.loc.start = index;
                    return token;
                case 't':
                    // can it be true?
                    if (index + 3 >= max) {
                        token.tag = Token::Tag::INVALID;
                        return token;
                    }
                    if (str[index+1] == 'r' && str[index+2] == 'u' && str[index+3] == 'e') {
                        token.tag = Token::Tag::BOOLEAN_TRUE;
                        token.loc.length = 3;
                        return token;
                    }
                    token.tag = Token::Tag::INVALID;
                    return token;
                case 'f':
                    // can it be false?
                    if (index + 4 >= max) {
                        token.tag = Token::Tag::INVALID;
                        return token;
                    }
                    if (str[index+1] == 'a' && str[index+2] == 'l' && str[index+3] == 's' && str[index+4] == 'e') {
                        token.tag = Token::Tag::BOOLEAN_FALSE;
                        token.loc.length = 4;
                        return token;
                    }
                    token.tag = Token::Tag::INVALID;
                    return token;
                case 'n':
                    // can it be null
                    if (index + 3 >= max) {
                        token.tag = Token::Tag::INVALID;
                        return token;
                    }
                    if (str[index+1] == 'u' && str[index+2] == 'l' && str[index+3] == 'l') {
                        token.tag = Token::Tag::NULLABLE;
                        token.loc.length = 3;
                        return token;
                    }
                    token.tag = Token::Tag::INVALID;
                    return token;
                case '"':
                    token.loc.start = index;
                    while (true) {
                        index++;
                        if (index >= max) {
                            token.tag = Token::Tag::INVALID;
                            return token;
                        }
                        switch (str[index]) {
                            case '\\':
                                if (index + 1 == max) {
                                    token.tag = Token::Tag::INVALID;
                                    return token;
                                }
                                if (str[index + 1] == '"') {
                                    index += 2;
                                }
                                continue;
                            case '"':
                                token.tag = Token::Tag::STRING_LITERAL;
                                token.loc.length = index - token.loc.start;
                                return token;
                            default:
                                continue;
                        }
                    }

                default:
                    continue;
            }
        }
    }
};

std::vector<Token> lex(const std::string &str)
{
    Lexer lexer = {str};
    std::vector<Token> tokens = {};

    while (true) {
        const Token token = lexer.next();
        if (token.tag == Token::Tag::END) {
            return tokens;
        }
        tokens.emplace_back(token);
    }
}
