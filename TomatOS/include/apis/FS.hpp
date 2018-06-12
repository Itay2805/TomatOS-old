#ifndef TOMATO_APIS_FS_HPP
#define TOMATO_APIS_FS_HPP

#include <stdint.h>
#include <stddef.h>

#include <tomato.h>

#include <apis/Term.hpp>

namespace Tomato {

	class FS {
	public:

		class File {
		public:
			enum Mode {
				STAT	= 0,

				READ	= 1,
				WRITE	= 4,
				APPEND	= 8,
			};

		private:
			tomato_file_handle_t * file;
			bool autoclose;

			char* buffer;
			int len;
			int cap;

			int pos;

			Mode mode;

		public:
			File(const char* path, Mode mode = STAT, bool autoclose = true);
			~File();

			void Close();

			int Read();
			bool ReadAll(char* buffer, int length);

			void Write(const char* text, bool appendnull = false);
			void Write(const char* bytes, int length);
			void Write(char byte);
			void WriteLine(const char* text, bool appendnull = false);
			
			void Flush();

			inline size_t GetSize() { return file->size; }
			inline const char* GetName() { return file->name; }
			inline bool IsDir() const { return file->type == TOMATO_FS_FOLDER; }
			inline bool Exists() const { return file != nullptr; }
			inline bool CanRead() const { return mode & READ; }
			inline bool CanWrite() const { return mode & WRITE || mode & APPEND; }

		private:

			void InitBuffer();
			void GrowBuffer(int howmuch);

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