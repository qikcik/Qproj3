#include "json/parser.hpp"
#include "json/tokenIterator.hpp"
#include "log/log.hpp"

using namespace json;

Object parseObject(TokenIterator& it);

Array parseArray(TokenIterator& it)
{
    Array result;
    it.next();
    if (auto* asKeyword = std::get_if<Token::Keyword>(&it.current()); asKeyword && asKeyword->value == ']')
        return result;


    while(!std::holds_alternative<Token::End>(it.current()))
    {
        std::visit(overloaded{
                [&](const Token::Keyword& keyword) {
                    if (keyword.value == '{') {
                        result.values.emplace_back(parseObject(it));
                        return;
                    }
                    if (keyword.value == '[') {
                        result.values.emplace_back(parseArray(it));
                        return;
                    }
                    LOG_WRN("unexpected keyword, expected {}, at:{}, hint:({})","'[' or '{'", it.getPosition(), it.getHint());
                    throw std::runtime_error("unexpected keyword, expected '[' or '{'");
                },
                [&](const Token::String& value) {
                    result.values.emplace_back(Value{value.value});
                },
                [&](const Token::Float& value) {
                    result.values.emplace_back(Value{value.value});
                },
                [&](const Token::End& end) {
                    LOG_WRN("unexpected end of file, expected value for entry, at:{}, hint:({})",it.getPosition(), it.getHint());
                    throw std::runtime_error("unexpected end of file, expected value for entry");
                },
        }, it.current());

        it.next();
        if (auto* asKeyword = std::get_if<Token::Keyword>(&it.current()); asKeyword && asKeyword->value == ',')
        {
            it.next();
            continue;
        }
        if (auto* asKeyword = std::get_if<Token::Keyword>(&it.current()); asKeyword && asKeyword->value == ']')
            return result;
        else
        {
            LOG_WRN("unexpected keyword, expected {}, at:{}, hint:({})", "',' or ']'", it.getPosition(), it.getHint());
            throw std::runtime_error("unexpected token, expected ',' or ']'");
        }
    }
    LOG_WRN("unexpected end of file, expected value for entry, at:{}, hint:({})",it.getPosition(), it.getHint());
    throw std::runtime_error("unexpected end of file, expected value for entry");
}

Object parseObject(TokenIterator& it)
{
    Object result;
    while(!std::holds_alternative<Token::End>(it.current()))
    {
        auto tokenKeyPtr = std::get_if<Token::String>(&it.next());

        if (!tokenKeyPtr) {
            LOG_WRN("unexpected keyword, expected string type key, at:{}, hint:({})", it.getPosition(), it.getHint());
            throw std::runtime_error("expected string type key");
        }

        std::string entryKey = tokenKeyPtr->value;

        if (auto* asKeyword = std::get_if<Token::Keyword>(&it.next()); !asKeyword or asKeyword->value != ':')
        {
            LOG_WRN("unexpected keyword, expected ':' as separator, at:{}, hint:({})", it.getPosition(), it.getHint());
            throw std::runtime_error("expected ':' as separator");
        }

        std::visit(overloaded {
                [&](const Token::Keyword& keyword)  {
                    if(keyword.value == '{')
                    {
                        result.entries[entryKey] = parseObject(it);
                        return;
                    }
                    if(keyword.value == '[')
                    {
                        result.entries[entryKey] = parseArray(it);
                        return;
                    }
                    LOG_WRN("unexpected keyword, expected {}, at:{}, hint:({})","'[' or '{'", it.getPosition(), it.getHint());
                    throw std::runtime_error("unexpected keyword, expected '[' or '{'");
                },
                [&](const Token::String& value)   {
                    result.entries[entryKey] = Value{value.value};
                },
                [&](const Token::Float& value)   {
                    result.entries[entryKey] = Value{value.value};
                },
                [&](const Token::End& end) {
                    LOG_WRN("unexpected end of file, expected value for entry, at:{}, hint:({})",it.getPosition(), it.getHint());
                    throw std::runtime_error("unexpected end of file, expected value for entry");
                },
        },it.next());

        it.next();
        if (auto* asKeyword = std::get_if<Token::Keyword>(&it.current()); asKeyword && asKeyword->value == ',')
            continue;
        if (auto* asKeyword = std::get_if<Token::Keyword>(&it.current()); asKeyword && asKeyword->value == '}')
            return result;
        else
        {
            LOG_WRN("unexpected keyword, expected {}, at:{}, hint:({})", "',' or '}'", it.getPosition(), it.getHint());
            throw std::runtime_error("unexpected token, expected ',' or '}'");
        }
    }
    LOG_WRN("unexpected end of file, expected value for entry, at:{}, hint:({})",it.getPosition(), it.getHint());
    throw std::runtime_error("unexpected end of file, expected value for entry");
}

std::variant<Object,Parser::ParseError> Parser::parse(const std::string& source)
{
    TokenIterator tokenIterator(source);
    if(auto token = std::get_if<Token::Keyword>(&tokenIterator.next()); token && token->value == '{' )
    {
        try {
            return parseObject(tokenIterator);
        } catch (const std::runtime_error& err) {
            return Parser::ParseError{err.what(),tokenIterator.getPosition()};
        }
    }
    //TODO: Source didn't contains root parseObject
    LOG_WRN("{}","Source didn't contains root parseObject");
    return Parser::ParseError{"Source didn't contains root parseObject",0};
}