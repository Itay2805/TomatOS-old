#ifndef CPPLIB_STATUS_HPP
#define CPPLIB_STATUS_HPP

#include <apis/Term.hpp>
#include <apis/Colors.hpp>

namespace cpplib {

	enum Status {
		OK,
		ERROR_EMPTY,
		ERROR_NOT_FOUND,
		ERROR_FULL,
		ERROR_RANGE,
		ERROR_NO_MEMORY,
	};

	extern const char* ErrorMessages[];

#define DO(action) \
	{ \
		::cpplib::Status __status = (action); \
		if(__status != ::cpplib::OK) { \
			return; \
		} \
	}

#define DOr(action) \
	{ \
		::cpplib::Status __status = (action); \
		if(__status != ::cpplib::OK) { \
			return __status; \
		} \
	}

#define DOPr(action) \
	{ \
		void* __status_pointer = (action); \
		if(__status_pointer == NULL) { \
			return ::cpplib::ERROR_NO_MEMORY; \
		} \
	}

#define DOP(action) \
	{ \
		void* __status_pointer = (action); \
		if(__status_pointer == NULL) { \
			return; \
		} \
	}

}

#endif