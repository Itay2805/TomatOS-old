#ifndef TOMATO_APIS_OS_H
#define TOMATO_APIS_OS_H

#include <tomato.h>

namespace Tomato {

	class OS {
	private:

	public:

		static inline const char* Version() {
			return tomato_os_version();
		}

	};

}

#endif