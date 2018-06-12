#include "FS.hpp"

namespace Tomato {

	bool FS::Exists(const char* path) {
		return tomato_fs_exists(path);
	}

	void FS::MakeDir(const char* path) {
		tomato_fs_make_dir(path);
	}

	FS::File::File(const char* path, Mode mode, bool autoclose)
		: autoclose(autoclose)
		, buffer(nullptr)
		, cap(0)
		, len(0)
		, mode(mode)
	{
		file = tomato_fs_open(path);

		InitBuffer();
	}

	FS::File::~File() {
		if (autoclose && file != nullptr) {
			Close();
		}
	}

	int FS::File::Read() {
		if (CanRead()) {
			if (pos < len) {
				return -1;
			}
			return buffer[pos++];
		}
	}

	bool FS::File::ReadAll(char* buffer, int length) {
		memcpy(buffer, this->buffer, length > len ? len : length);
		return len < length;
	}

	void FS::File::Write(const char* text, bool appendnull) {
		if (CanWrite()) {
			int size = strlen(text) + (appendnull ? 1 : 0);
			GrowBuffer(pos + size);
			memcpy(buffer + pos, text, strlen(text));
			len += size;
			pos += size;
		}
	}

	void FS::File::Write(const char* bytes, int length) {
		if (CanWrite()) {
			GrowBuffer(pos + length);
			memcpy(buffer + pos, bytes, length);
			len += length;
			pos += length;
		}
	}

	void FS::File::WriteLine(const char* text, bool appendnull) {
		if (CanWrite()) {
			Write(text);
			Write('\n');
			if (appendnull) {
				Write((char)0);
			}
		}
	}

	void FS::File::Flush() {
		if (CanWrite()) {
			tomato_fs_write_bytes(file, buffer, len);
		}
	}

	void FS::File::Write(char byte) {
		if (CanWrite()) {
			buffer[pos++] = byte;
		}
	}

	void FS::File::InitBuffer() {
		if (!(mode & STAT)) {
			if (mode & READ || mode & APPEND) {
				len = GetSize();

				if (mode & APPEND) {
					mode = (Mode)(mode | WRITE); // to make sure write is enabled
					buffer = (char*)malloc(GetSize() + 64);
					cap = GetSize() + 64;
					pos = len;
				}
				else {
					buffer = (char*)malloc(GetSize());
					cap = GetSize();
				}

				tomato_fs_read_bytes(file, buffer, GetSize());
			}
			else if (mode & WRITE) {
				buffer = (char*)malloc(64);
				cap = 64;
			}
		}
	}

	void FS::File::GrowBuffer(int newlen) {
		if (newlen <= cap) {
			return;
		}
		cap = (1 + 2 * cap) > newlen ? (1 + 2 * cap) : newlen;
		realloc(buffer, cap);
	}

	void FS::File::Close() {
		if (file != nullptr) {
			if (buffer != nullptr) {
				Flush();

				free(buffer);
				cap = 0;
				len = 0;
				buffer = nullptr;
			}

			tomato_fs_close(file);
			file = nullptr;
		}
	}

	FS::List::List(const char* path) {
		entries = tomato_fs_list(path, &count);
	}

	FS::List::~List() {
		tomato_fs_list_release(entries);
	}


}