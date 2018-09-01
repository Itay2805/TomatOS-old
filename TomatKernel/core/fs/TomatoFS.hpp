#ifndef KERNEL_CORE_FS_BLOCK_HPP
#define KERNEL_CORE_FS_BLOCK_HPP

#include <stdint.h>

#include <core/disk/ata.h>

namespace TomatKernel {
	namespace FS {
				
#define SECTOR_SIZE (512)
#define DATA_SIZE (508)

		enum class INodeType {
			NONE = 0,
			FLDR = 'RDLF',
			FILE = 'ELIF',
			DATA = 'ATAD'
		};

		struct PhysicalBlock {
			INodeType type;
			uint8_t data[SECTOR_SIZE - sizeof(INodeType)];
		};

		class Block {
		public:
			PhysicalBlock phys;
			size_t sector;

			Block()
			{

			}

			Block(size_t sector)
				: sector(sector)
			{

			}

			inline void Read() { disk_read((uint8_t*)(&phys), sector); }
			inline void Write() { disk_write((uint8_t*)(&phys), sector); }
			
			inline void Allocate() { phys.type = INodeType::DATA; }
			inline void Deallocate() { phys.type = INodeType::NONE; }

			inline bool IsAllocated() { return phys.type != INodeType::NONE; }

			inline uint8_t* GetPhys() { return (uint8_t*)&phys; }
			inline uint8_t* GetData() { return phys.data; }

		};

		struct Header {
			uint32_t signature1 = 'amoT';
			uint32_t signature2 = 'SFot';

			uint32_t first_free_block;
			uint32_t root_folder;
			uint32_t allocated_count;

			uint8_t empty[SECTOR_SIZE - sizeof(uint32_t) * 4];
		};

		class PBM {
		public:
			static Header* header;
			static Block headerBlock;

			static void Init();
			static size_t AllocateBlock(Block* ref = nullptr);
			static void DeallocateBlock(size_t sector);
			static void Commit();

		};

		struct PhysicalINode {
			INodeType type;
			uint8_t name[16];
			uint8_t owners;
			uint8_t ref_count;
			uint16_t size;
			uint32_t entries[((SECTOR_SIZE - sizeof(INodeType) - 16 - 1 - 1 - 2) / sizeof(uint32_t))];
		};


		class INode {
		public:
			PhysicalINode phys;
			size_t sector;

			INode() {

			}

			INode(size_t sector) 
				: sector(sector)
			{
			}

			void Read();
			void Write();
			void ReadData(uint8_t* buffer, int offset, int length);
			void WriteData(uint8_t* buffer, int offset, int length);

			bool Delete(bool* deleted = nullptr);
			bool Resize(size_t newsize);

		};

		bool HasFilesystem();
		void CreateFilesystem();
		void Format();
		size_t FindInFolder(size_t inode, char* name);
		size_t Resolve(const char* path);

		bool MakeDir(const char* path, uint8_t owners);


		struct ListEntry {
			char name[16];
			size_t inode;
		};

		struct List {

		};


	}
}

#endif