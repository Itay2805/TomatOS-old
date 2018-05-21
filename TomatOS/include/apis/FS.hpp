#ifndef TOMATO_APIS_FS_HPP
#define TOMATO_APIS_FS_HPP

#include <stdint.h>

namespace Tomato {

	class File {
	private:
		uint32_t handle;
		FS::Mode mode;

	public:
		
		void Write(const char* text);
		void Write(size_t count, uintptr_t buffer);
		void Read(size_t count, uintptr_t buffer);
		void Flush();
		void Close();

		inline uint32_t GetHandle() const { return handle; }
		inline FS::Mode GetMode() const { return mode; }

	};

	class FS {
	public:

		enum Mode : uint8_t {
			READ = 1 << 0,
			WRITE = 1 << 1,
			APPEND = 1 << 2,
		};

		static File Open(const char* path, Mode mode = (Mode)(READ | WRITE));
		static bool Exists(const char* path);
		static bool IsDir(const char* path);
		static size_t GetSize(const char* path);
		static void MakeDir(const char* path);
		static void Move(const char* from, const char* to);
		static void Copy(const char* from, const char* to);
		static void Delete(const char* path);

	};

}

#endif