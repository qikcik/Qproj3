#pragma once
#include "concepts"

// Using destructor and constructor to generate assigment operators, not optimized, but fast to use.

#define GEN_COPY_ASSIGMENT_OPERATOR(TType) TType& operator=(const TType& other) \
{ \
    if(this == &other) \
        return *this; \
    this->~TType(); \
    new (this) TType(other); \
    return *this; \
}

#define GEN_MOVE_ASSIGMENT_OPERATOR(TType) \
TType& operator=(TType&& other) noexcept \
{ \
    if(this == &other) \
        return *this; \
    this->~TType(); \
    new (this) TType(std::move(other)); \
    return *this; \
} \

