#include <iostream>
#include <format>

#define LOG(prefix,in_template,in_args...) std::cout << "["<< prefix <<"][" << __FILE__ << ":" << __LINE__ << "]" << std::format(in_template,in_args) << std::endl;
#define LOG_DBG(in_template,in_args...) LOG("DBG",in_template,in_args)
#define LOG_INF(in_template,in_args...) LOG("INF",in_template,in_args)
#define LOG_WRN(in_template,in_args...) LOG("WRN",in_template,in_args)
#define LOG_ERR(in_template,in_args...) LOG("ERR",in_template,in_args)
#define ENSURE_OR_RETURN(in_condition,in_ret) if(!(in_condition)) { LOG("ENSURE","condition:({}) not satisfied, return:({})",#in_condition,#in_ret) ; return in_ret; }
