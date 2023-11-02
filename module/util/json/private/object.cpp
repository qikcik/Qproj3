#include "json/object.hpp"
#include "log/log.hpp"
using namespace json;

std::string Array::stringify() const
{
    if(values.empty()) return "[]";

    std::string result = "[";

    for(const auto& it : values)
    {
        std::visit(overloaded{
                [&](const Value& val)
                {
                    result += val.stringify()+",";
                },
                [&](const Object& obj)
                {
                    result += obj.stringify()+",";
                },
                [&](const Array& arr)
                {
                    result += arr.stringify()+",";
                },
        },it);
    }

    result[result.size()-1] = ']';
    return result;
}

std::string Object::stringify() const
{
    std::string result = "{";

    for(const auto& entryIt : entries)
    {
        result += "\""+entryIt.first+"\":";

        std::visit(overloaded {
            [&](const Value& val)
            {
                result += val.stringify()+",";
            },
            [&](const Object& obj)
            {
                result += obj.stringify()+",";
            },
            [&](const Array& arr)
            {
                result += arr.stringify()+",";
            },
        }, entryIt.second);
    }

    result[result.size()-1] = '}';
    return result;
}

void Object::set(const std::string& key, const innerType& value)
{
    entries[key] = value;
}
