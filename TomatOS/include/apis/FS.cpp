#include "FS.hpp"

namespace Tomato {

	namespace FS {

		bool Exists(const char* path) {
			return tomato_fs_exists(path);
		}

		void MakeDir(const char* path) {
			tomato_fs_make_dir(path);
		}

		int GetParentPathLength(const char* path) {
			if (*path == '/' || *path == '\\') {
				path++;
			}
			int total_length = 0;
			int current_length = 0;
			while (*path) {
				if (*path == '/' || *path == '\\') {
					total_length += current_length + 1;
				}
				else {
					current_length++;
				}
				path++;
			}

			return total_length;
		}

		int GetNameLength(const char* path) {
			int length_total = strlen(path);
			int length = GetParentPathLength(path);
			return length_total - length;
		}

		void GetParentPath(const char* path, char* dest, bool nullterminator) {
			int length = GetParentPathLength(path);
			memcpy(dest, path, length);
			if (nullterminator) {
				dest[length] = 0;
			}
		}

		void GetName(const char* path, char* dest, bool nullterminator) {
			int length_total = strlen(path);
			int length = GetParentPathLength(path);
			memcpy(dest, path + length, length_total - length);
			if (nullterminator) {
				dest[length_total - length] = 0;
			}
		}

		File::File(const char* path, Mode mode, bool autoclose)
			: autoclose(autoclose)
			, buffer(nullptr)
			, cap(0)
			, len(0)
			, mode(mode)
		{
			file = tomato_fs_open(path);

			InitBuffer();
		}

		File::~File() {
			if (autoclose) {
				Close();
			}
		}

		int File::Read() {
			if (CanRead()) {
				if (pos >= len) {
					return -1;
				}
				return buffer[pos++];
			}
		}

		bool File::ReadAll(char* buffer, int length) {
			if (CanRead()) {
				memcpy(buffer, this->buffer, length > len ? len : length);
				return len < length;
			}
			return false;
		}

		void File::Write(const char* text, bool appendnull) {
			if (CanWrite()) {
				int textlen = strlen(text);
				int size = textlen + (appendnull ? 1 : 0);
				if (textlen == 0) {
					return;
				}
				GrowBuffer(len + size);
				memcpy(buffer + len, text, textlen);
				len += size;
				if (appendnull) {
					Write((char)0);
				}
			}
		}

		void File::WriteBytes(const char* bytes, int length) {
			if (CanWrite()) {
				GrowBuffer(len + length);
				memcpy(buffer + len, bytes, length);
				len += length;
			}
		}

		void File::WriteLine(const char* text, bool appendnull) {
			if (CanWrite()) {
				Write(text);
				Write('\n');
				if (appendnull) {
					Write((char)0);
				}
			}
		}

		void File::Flush() {
			if (CanWrite()) {
				if (len == 0) return;
				if (mode == APPEND) {
					tomato_fs_write_bytes(file, buffer, len, GetSize());
					len = 0;
				}
				else if (mode == WRITE) {
					tomato_fs_write_bytes(file, buffer, len, 0);
				}
			}
		}

		void File::Write(char byte) {
			if (CanWrite()) {
				GrowBuffer(len + 1);
				buffer[len++] = byte;
			}
		}

		void File::InitBuffer() {
			if (mode != STAT) {
				if (mode == READ) {
					len = GetSize();
					buffer = (char*)malloc(GetSize());
					tomato_fs_read_bytes(file, buffer, GetSize(), 0);
				}
				else {
					buffer = (char*)malloc(64);
					cap = 64;
					len = 0;
				}
			}
		}

		void File::GrowBuffer(int newlen) {
			if (newlen <= cap) {
				return;
			}
			cap = (1 + 2 * cap) > newlen ? (1 + 2 * cap) : newlen;
			realloc(buffer, cap);
		}

		void File::Close() {
			if (file != nullptr) {
				if (buffer != nullptr) {
					Flush();

					free(buffer);
					cap = 0;
					len = 0;
					pos = 0;
					buffer = nullptr;
				}

				tomato_fs_close(file);
				file = nullptr;
				autoclose = false;
			}
		}

		List::List(const char* path) {
			entries = tomato_fs_list(path, &count);
		}

		List::~List() {
			tomato_fs_list_release(entries);
		}

	}


}