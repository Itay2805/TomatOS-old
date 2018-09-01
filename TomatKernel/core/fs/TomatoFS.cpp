#include "TomatoFS.hpp"

#include <string.h>
#include <math.h>

#include <core/cpp.hpp>
#include <core/disk/ata.h>

namespace TomatKernel {
	namespace FS {

		Header* PBM::header;
		Block PBM::headerBlock;

		void Format() {
			size_t count = disk_sector_count();
			
			Block block;
			memset(&block, 0, sizeof(Block));

			for (size_t i = 1; i < count; i++) {
				block.sector = i;
				block.Write();
			}
		}

		void CreateFilesystem() {
			Format();

			Header header;
			header.first_free_block = 3;
			header.root_folder = 2;
			header.allocated_count = 1;

			INode root(2);
			root.phys.type = INodeType::FLDR;
			memcpy(root.phys.name, "<root>", sizeof("<root>"));
			root.phys.owners = 0;
			root.phys.ref_count = 1;
			root.phys.size = 0;

			disk_write((uint8_t*)(&header), 1);
			disk_write((uint8_t*)(&root), 2);
		}

		void INode::Read() {
			Block block(sector);
			block.Read();
			phys = *(PhysicalINode*)(block.GetPhys());
		}
		
		
		void INode::Write() {
			Block block(sector);
			*(PhysicalINode*)(block.GetPhys()) = phys;
			block.Write();
		}
		
		
		void INode::ReadData(uint8_t* buffer, int offset, int length) {
			int first_block = offset / DATA_SIZE;
			int last_block = ceil((offset + length) / (1.0 * DATA_SIZE));
			for (int i = first_block; i < last_block; i++) {
				Block block(phys.entries[i]);
				block.Read();
				if (i + 1 < last_block) {
					memcpy(buffer + i * DATA_SIZE, block.GetData(), DATA_SIZE);
				}
				else {
					memcpy(buffer + i * DATA_SIZE, block.GetData(), length % DATA_SIZE);
				}
			}
		}
		
		
		void INode::WriteData(uint8_t* buffer, int offset, int length) {
			int first_block = offset / DATA_SIZE;
			int last_block = ceil((offset + length) / (1.0 * DATA_SIZE));
			for (int i = first_block; i < last_block; i++) {
				Block block(phys.entries[i]);
				if (i + 1 < last_block) {
					memcpy(block.GetData(), buffer + i * DATA_SIZE, DATA_SIZE);
				}
				else {
					memcpy(block.GetData(), buffer + i * DATA_SIZE, length % DATA_SIZE);
				}
				block.Write();
			}
		}

		bool INode::Delete(bool* deleted) {
			if (sector == 0) {
				return false;
			}
			if (phys.type == INodeType::FILE || phys.type == INodeType::FLDR) {
				// if we have no more references delete all the nodes and free this one
				if (phys.ref_count == 0 || phys.ref_count - 1 == 0) {
					// for folder we need to remove all the child files
					if (phys.type == INodeType::FLDR) {
						for (int i = 0; i < phys.size; i++) {
							INode node(phys.entries[i]);
							node.Read();
							node.Delete();
						}
					}
					// for file we just need to remove all the data blocks
					else {
						int last_block = ceil(phys.size / (1.0 * DATA_SIZE));
						for (int i = 0; i < last_block; i++) {
							PBM::DeallocateBlock(phys.entries[i]);
						}
					}
					// free this node
					PBM::DeallocateBlock(sector);
					sector = 0;
					if (deleted != nullptr) *deleted = true;
				}
				else {
					// there are still references to this file, decrease the ref counter
					phys.ref_count--;
					Write();
					if (deleted != nullptr) *deleted = false;
				}
				return true;
			}
			return false;
		}

		bool INode::Resize(size_t newsize) {
			if (phys.type != INodeType::FILE) return false;
			if (phys.size >= newsize) {
				// deallocate blocks
				int first_block = newsize / DATA_SIZE;
				int last_block = ceil(phys.size / (1.0 * DATA_SIZE));
				for (int i = first_block; i < last_block; i++) {
					PBM::DeallocateBlock(phys.entries[i]);
				}
			}
			else {
				// add more blocks
				int first_block = phys.size / DATA_SIZE;
				size_t last_block = (size_t)ceil(newsize / (1.0 * DATA_SIZE));
				if (last_block >= sizeof(phys.entries) / sizeof(size_t)) {
					return false;
				}
				for (size_t i = first_block; i < last_block; i++) {
					size_t newBlock = PBM::AllocateBlock();
					phys.entries[i] = newBlock;
				}
			}
			phys.size = (uint16_t)newsize;
			return true;
		}

		void PBM::Init() {
			headerBlock = Block(1);
			headerBlock.Read();
			header = (Header*)(headerBlock.GetPhys());
		}

		size_t PBM::AllocateBlock(Block* ref) {
			size_t block = header->first_free_block;
			while (true) {
				header->first_free_block++;
				Block block(header->first_free_block);
				block.Read();
				if (!block.IsAllocated()) {
					break;
				}
			}
			if (ref != nullptr) {
				ref->sector = block;
				ref->Allocate();
				ref->Write();
			}
			else {
				Block b(block);
				b.Allocate();
				b.Write();
			}
			header->allocated_count++;
			return block;
		}
		
		void PBM::DeallocateBlock(size_t sector) {
			Block block(sector);
			block.Deallocate();
			block.Write();
			if (header->first_free_block > sector) {
				header->first_free_block = sector;
			}
			header->allocated_count--;
		}

		void PBM::Commit() {
			headerBlock.Write();
			disk_flush();
		}

		bool HasFilesystem() {
			return PBM::header->signature1 == 'amoT' && PBM::header->signature2 == 'SFot';
		}

		size_t FindInFolder(size_t inode, char* name) {
			INode node(inode);
			node.Read();
			if (node.phys.type != INodeType::FLDR) {
				return 0;
			}
			for (uint32_t i = 0; i < node.phys.size; i++) {
				INode child(node.phys.entries[i]);
				child.Read();
				if (strcmp((char*)child.phys.name, name) == 0) {
					return node.phys.entries[i];
				}
			}
			return 0;
		}

		size_t Resolve(const char* path) {
			if (*path == 0) return PBM::header->root_folder;

			// this is kinda long just incase we will need it
			char* temp = new char[strlen(path) + 1];
			char* temp_start = temp;

			size_t currentBlock = PBM::header->root_folder;
			while (*path) {
				if (*path == '/' || *path == '\\') {
					*temp = 0;
					temp = temp_start;
					currentBlock = FindInFolder(currentBlock, temp);
					if (currentBlock == 0) {
						delete[] temp;
						return 0;
					}
				}
				else {
					*temp = *path;
				}
				temp++;
				path++;
			}

			// resolve last block if any
			if (temp != temp_start) {
				*temp = 0;
				temp = temp_start;
				currentBlock = FindInFolder(currentBlock, temp);
				if (currentBlock == 0) {
					delete[] temp;
					return 0;
				}
			}

			temp = temp_start;
			delete[] temp;
			return currentBlock;
		}


	}
}