#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <iostream>
#include <string>

#ifdef SLG_ENABLE_DEBUG
inline void print_debug(const std::string &Msg) {
	std::cout << "[DEBUG] " << Msg << "\n";
}
#endif

#ifdef SLG_ENABLE_ERROR
inline void print_error(const std::string &Msg) {
	std::cout << "[ERROR] " << Msg << "\n";
}
#endif

#ifdef SLG_ENABLE_INFO
inline void print_info(const std::string &Msg) {
	std::cout << "[INFO] " << Msg << "\n";
}
#endif

#endif

