#ifndef TOMATO_APIS_FS_HPP
#define TOMATO_APIS_FS_HPP

#include <stdint.h>
#include <stddef.h>

#include <tomato.h>

#include <apis/Term.hpp>

namespace Tomato {

	namespace FS {

		class File {
		public:
			enum Mode {
				STAT,
				READ,
				WRITE,
				APPEND,
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

			template<typename T>
			T Read(bool* eof = nullptr) {
				T def = {};
				if (CanRead()) {
					char buf[sizeof(T)];
					for (int i = 0; i < sizeof(T); i++) {
						int read = Read();
						if (read == -1) {
							if (eof != nullptr) *eof = true;
							return def;
						}
						buf[i] = read;
					}
					if (eof != nullptr) *eof = false;
					return *(T*)buf;
				}
				if (eof != nullptr) *eof = true;
				return def;
			}

			int Read();
			bool ReadAll(char* buffer, int length);

			void Write(const char* text, bool appendnull = false);
			void Write(char byte);
			void WriteLine(const char* text, bool appendnull = false);
			void WriteBytes(const char* bytes, int length);

			template<typename T>
			void Write(const T t) {
				WriteBytes((char*)&t, sizeof(T));
			}

			void Flush();

			inline size_t GetSize() { return file->size; }
			inline const char* GetName() { return file->name; }
			inline bool IsDir() const { return file->type == TOMATO_FS_FOLDER; }
			inline bool IsValid() const { return file->type != TOMATO_FS_NOT_EXISTS; }
			inline bool CanRead() const { return IsValid() && mode == READ; }
			inline bool CanWrite() const { return mode == WRITE || mode == APPEND; }

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

		int GetParentPathLength(const char* path);
		int GetNameLength(const char* path);

		void GetParentPath(const char* path, char* dest, bool nullterminator = true);
		void GetName(const char* path, char* dest, bool nullterminator = true);

		bool Exists(const char* path);
		void MakeDir(const char* path);

	}

}

#endif