#pragma once

#include <vector>
#include <variant>
#include <string>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include <memory>
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace json
{
    using string = std::string;
    using Float = float;

    struct Object;
    struct Array;
    struct Value;

    using innerType = std::variant<Value,Object,Array>;

    struct Value
    {
        Value() = default;

        explicit Value(string val) : value(std::move(val)) {};
        explicit Value(const char* val) : value(val) {};
        explicit Value(float val) : value(val) {};
        explicit Value(bool val) : value(val) {};

        std::string stringify() const {
            return std::visit(overloaded{
                    [&](const string& str) -> string{
                        return '"'+str+'"';
                    },
                    [&](const float& num) -> string{
                        return std::to_string(num);
                    },
                    [&](const bool& b) -> string {
                        return b ? R"("true")" : R"("false")";
                    }
            },value);
        };

        using innerType = std::variant<string,float,bool>;
        innerType value {};

        template<typename T>
        T* get() {return std::get_if<T>(&value); };

        template<typename T>
        const T* get() const {return std::get_if<T>(&value); };
    };

    struct Array
    {
        std::vector<innerType> values;

        template<typename T>
        T* get(size_t idx);

        std::string stringify() const;
    };

    struct Object
    {

        template<typename T>
        T* get(const std::string& key);
        void set(const std::string& key,const innerType& value);

        std::unordered_map<string,innerType> entries;
        std::string stringify() const;
    };


    template<typename T>
    T* Array::get(size_t idx) {return std::get_if<T>(&values[idx]); };

    template<typename T>
    T* Object::get(const std::string& key) { return std::get_if<T>(&entries[key]); };
}