#ifndef CPPLIB_VECTOR_SORTED_HPP
#define CPPLIB_VECTOR_SORTED_HPP

#include <Vector.hpp>

namespace cpplib {

	template<class Data>
	class VectorSorted : public Vector<Data> {
	protected:

		void FindRange(const Data& item, size_t& rlower, size_t& rupper) {
			size_t lower = -1, upper = this->Size();
			for (;;) {
				if (lower + 1 >= upper) {
					rupper = upper;
					rlower = lower;
					return;
				}
				size_t middle = (lower + upper) / 2;
				const Data& median = this->At(middle);
				short c = Compare(item, median);
				if (c < 0) upper = middle;
				else if (c > 0) lower = middle;
				else { 
					rlower = middle - 1; 
					rupper = middle + 1;
					return;
				}
			}
		}

	public:

		VectorSorted(size_t size = 0) 
			: Vector<Data>(size)
		{

		}

		Status Find(const Data& item, size_t& index) const {
			int lower = -1, upper = this->Size();
			for (;;) {
				if (lower + 1 >= upper) return ERROR_NOT_FOUND;
				size_t middle = (lower + upper) / 2;
				const Data& median = this->At(middle);
				short c = Compare(item, median);
				if (c < 0) upper = middle;
				else if (c > 0) lower = middle;
				else {
					index = middle;
					return OK;
				}
			}
		}

		const Data* Find(const Data& item) const {
			size_t index;
			if (Find(item, index) == ERROR_NOT_FOUND) return NULL;
			else return &this->At(index);
		}

		Data* Find(const Data& item) {
			size_t index;
			if (Find(item, index) == ERROR_NOT_FOUND) return NULL;
			else return &this->At(index);
		}

		bool Contains(const Data& item) const {
			return Find(item) != NULL;
		}

		Status Insert(const Data& item) {
			size_t lower, upper;
			FindRange(item, lower, upper);
			return Vector<Data>::Insert(upper, item);
		}

		Status InsertUnique(const Data& item) {
			int lower, upper;
			FindRange(item, lower, upper);
			if (upper - lower >= 2) return ERROR_EXISTS;
			return Vector<Data>::Insert(size_t(upper), item);
		}

		Status FindAndRemove(const Data& item) {
			size_t index;
			DOr(Find(item, index));
			return this->RemoveAt(index);
		}

	};

}

#endif