#include <iostream>
#include <format>

//TODO: This function sholud be splitted to separated files, but for now there is small amount of code.

#define LOG(prefix,in_template,in_args...) std::cout << "["<< prefix <<"][" << __FILE__ << ":" << __LINE__ << "]" << std::format(in_template,in_args) << std::endl;
//#define LOG_DBG(in_template,in_args...) LOG("DBG",in_template,in_args)
#define LOG_DBG(in_template,in_args...)
#define LOG_INF(in_template,in_args...) LOG("INF",in_template,in_args)
#define LOG_WRN(in_template,in_args...) LOG("WRN",in_template,in_args)
#define LOG_ERR(in_template,in_args...) LOG("ERR",in_template,in_args)

#define PTR_TO_STR(in_ptr) std::to_string((unsigned long long)(void**)in_ptr)


//TODO: macros with DBG_ prefix, should be removed in release build
#define ENSURE(in_condition) if(!(in_condition)) { LOG("ENSURE","condition:({}) not satisfied",#in_condition); }
#define DBG_ENSURE(in_condition) if(!(in_condition)) { LOG("ENSURE","condition:({}) not satisfied",#in_condition); }
#define ENSURE_OR_RETURN(in_condition,in_ret) if(!(in_condition)) { LOG("ENSURE","condition:({}) not satisfied, return:({})",#in_condition,#in_ret); throw std::runtime_error{""}; return in_ret; }
#define DBG_ENSURE_OR_RETURN(in_condition,in_ret) if(!(in_condition)) { LOG("ENSURE","condition:({}) not satisfied, return:({})",#in_condition,#in_ret) ; return in_ret; }
