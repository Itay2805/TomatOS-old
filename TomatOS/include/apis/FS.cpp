#include "FS.hpp"

namespace Tomato {

	bool FS::Exists(const char* path) {
		return tomato_fs_exists(path);
	}

	void FS::MakeDir(const char* path) {
		tomato_fs_make_dir(path);
	}

	FS::File::File(const char* path) {
		file = tomato_fs_open(path);
	}

	FS::File::~File() {
		tomato_fs_close(file);
	}

	FS::List::List(const char* path) {
		entries = tomato_fs_list(path, &count);
	}

	FS::List::~List() {
		tomato_fs_list_release(entries);
	}


}