#ifndef CPPLIB_VECTOR_HPP
#define CPPLIB_VECTOR_HPP

#include <Memory.hpp>
#include <Tmplate.hpp>
#include <Status.hpp>

namespace cpplib {

	template<class Data>
	class VectorCursor;

	template<class Data_>
	class Vector {
	public:
		typedef Data_ Data;
		typedef VectorCursor<Data_> Cursor;

	protected:
		Pointer<Data> info;

	public:
		Vector(size_t size = 0) {
			info.Alloc(size);
		}

		void Free() {
			info.Free();
		}

		void RemoveAll() {
			info.Free();
		}

		Status Resize(size_t newsize) {
			Data* oldElements;
			size_t oldsize = info.size;
			DOr(info.Realloc(newsize, oldElements));
			if (oldElements == NULL) {
				return OK;
			}
			DOr(info.Copy(0, oldElements, Min(oldsize, newsize)));
			VecFree(oldElements);
			return OK;
		}

		Status ResizeIfNeeded(size_t minNewSize, int delta = 1) {
			return Resize(NewSize(info.size, minNewSize, delta));
		}

		Status AppendEmpty() {
			return ResizeIfNeeded(info.size + 1);
		}

		void Share(const Vector<Data>& from) {
			return info.Share(from.info);
		}

		void Steal(Vector<Data>& from) {
			return info.Steal(from.info);
		}

		void operator=(const Vector<Data>& from) {
			Share(from);
		}

		size_t Size() const { return info.size; }
		size_t Count() const { return info.size; }
		bool IsEmpty() const { return info.ptr == NULL; }

		const Data& Last() const { return info[info.size - 1]; }
		Data& Last() { return info[info.size - 1]; }

		Data* Buffer() { return info.ptr; }
		const Data* Buffer() const { return info.ptr; }

		Data& At(size_t index) { return info[index]; }
		const Data& At(size_t index) const { return info[index]; }

		Data& operator[](size_t index) { return info[index]; }
		const Data& operator[](size_t index) const { return info[index]; }

		void Append(const Data& data) {
			size_t oldSize = info.size;
			size_t newSize = oldSize + 1;
			Data* oldElements;
			info.Realloc(newSize, oldElements);
			if (oldElements != NULL) {
				info.Copy(0, oldElements, oldSize);
			}
			info[oldSize] = data;
			VecFree(oldElements);
		}

		Status Replace(size_t from, size_t to, const Data* with, size_t f, size_t t) {
			Sort2(from, to);
			Sort2(f, t);
			if (to > info.size) return ERROR_RANGE;
			size_t oldSize = info.size;
			size_t replacedSize = to - from;
			size_t replacingSize = t - f;
			size_t newSize = oldSize - replacedSize + replacingSize;
			if (newSize == oldSize) {
				info.Copy(from, with + f, replacingSize);
			}
			else {
				Data* oldElements;
				info.Realloc(newSize, oldElements);
				if (oldElements != NULL) info.Copy(0, oldElements, from);
				info.Copy(from, with + f, replacingSize);
				info.Copy(to - replacedSize + replacingSize, oldElements + to, oldSize - to);
				VecFree(oldElements);
			}
			return OK;
		}

		Status Replace(size_t from, size_t to, const Vector<Data>& with, size_t f, size_t t) {
			return Relace(from, to, with.Buffer(), f, t);
		}

		Status Replace(size_t from, size_t to, const Vector<Data>& with) {
			return Relace(from, to, with.Buffer(), 0, with.Size());
		}

		Status Replace(size_t from, size_t to, const Data& with) {
			return Replace(from, to, &with, 0, 1);
		}

		Status Set(size_t at, const Data* s, size_t f, size_t t) {
			return Replace(at, Min(info.size, at + t - f), s, f, t);
		}

		Status Set(size_t at, const Data& c) {
			return Set(at, &c, 0, 1);
		}

		Status Set(size_t at, const Vector<Data>& with) {
			return Set(at, with.Buffer(), 0, with.Size());
		}

		Status Insert(size_t at, const Data* s, size_t f, size_t t) {
			return Replace(at, at, s, f, t);
		}

		Status Insert(size_t at, const Vector<Data>& s, size_t f, size_t t) {
			return Replace(at, at, s, f, t);
		}

		Status Insert(size_t at, const Vector<Data>& s) {
			return Replace(at, at, s);
		}

		Status Insert(size_t at, const Data& c) {
			return Replace(at, at, c);
		}

		Status Insert(const Data& c) {
			return Insert(info.size, c);
		}

		Status Append(const Vector<Data>& s) {
			return Insert(info.size, s);
		}

		Status Prepend(const Vector<Data>& s) {
			return Insert(0, s);
		}

		Status Prepend(const Data& c) {
			return Insert(0, c);
		}

		Status RemoveRange(size_t from, size_t to) {
			return Replace(from, to, info.ptr, 0, 0);
		}

		Status RemoveAt(size_t index) {
			return RemoveRange(index, index + 1);
		}

		Status Truncate(size_t i = 0) {
			return RemoveAt(i, info.size);
		}

		void Duplicate(const Vector<Data>& from) {
			::cpplib::Duplicate(info, from.info);
		}

		Status Find(Data item, size_t from, size_t to, size_t& result) {
			Sort2(from, to);
			if (to > info.size) return ERROR_RANGE;
			for (size_t i = from; i < to; ++i) {
				if (Compare(At(i), item) == 0) {
					result = i;
					return OK;
				}
			}
			return ERROR_NOT_FOUND;
		}

		Status Find(Data item, size_t& result) {
			return Find(item, 0, Size(), result);
		}

		bool Contains(Data item) {
			size_t result;
			return Find(item, result) == OK;
		}

		const Data* Find(Data item) const {
			size_t index;
			if (Find(item, index) == ERROR_NOT_FOUND) {
				return NULL;
			}
			else {
				return &At(index);
			}
		}

		Data* Find(Data item) {
			size_t index;
			if (Find(item, index) == ERROR_NOT_FOUND) {
				return NULL;
			}
			else {
				return &At(index);
			}
		}

		Status FindAndRemove(Data item) {
			size_t index;
			DOr(Find(item, index));
			return RemoveAt(index);
		}

	};

	template<class Data>
	void Duplicate(Vector<Data>& to, const Vector<Data>& from) {
		to.Duplicate(from);
	}

	template<class Data>
	void Free(Vector<Data>& v) {
		v.Free();
	}

	template<class Data>
	class VectorCursor {
	protected:
		Vector<Data>* vector;
		size_t index;

	public:

		bool IsValid() const { return index < vector->Size(); }
		bool IsNextValid() const { return index + 1 < vector->Size(); }

		const Data& Item() const { return vector->At(index); }
		Data& Item() { return vector->At(index); }
		size_t Index() const { return index; }

		void Reset(Vector<Data>& vec) {
			vector = &vec;
			index = 0;
		}

		void Assign(const VectorCursor<Data>& c) {
			vector = c.vector;
		}

		VectorCursor(Vector<Data>& vec) {
			Reset(vec);
		}

		VectorCursor(VectorCursor<Data>& c) {
			Assign(c);
		}

		VectorCursor(size_t index = 0) {
			this->index = index;
		}

		void operator=(const VectorCursor<Data> cursor) {
			Assign(cursor);
		}

		void Next() { ++index; }
		void operator++() { Next(); }
		
		const VectorCursor<Data>* Clone() const { return new VectorCursor<Data>(index); }

		Status Find(Data item) {
			for (; IsValid(); Next()) {
				if (Compare(Item(), item) == 0) return OK;
			}
			return ERROR_NOT_FOUND;
		}

	};

}

#endif 