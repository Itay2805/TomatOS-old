#ifndef TOMATO_APIS_FS_HPP
#define TOMATO_APIS_FS_HPP

#include <stdint.h>

#include <tomato.h>

#include <apis/Term.hpp>

namespace Tomato {

	class FS {
	public:

		class File {
		private:
			tomato_file_handle_t * file;

		public:
			File(const char* path);
			~File();

			inline size_t GetSize() { return file->size; }
			inline const char* GetName() { return file->name; }
			inline bool IsDir() const { return file->type == TOMATO_FS_FOLDER; }
			inline bool Exists() const { return file != nullptr; }
			
		};

		class List {
		private:
			tomato_list_entry_t * entries;
			int count;

		public:

			List(const char* path);
			~List();

			inline char* operator[](int index) {
				return entries[index].name;
			}

			inline int Count() const { return count; }
		};

		static bool Exists(const char* path);
		static void MakeDir(const char* path);

	};

}

#endif