#ifndef CPPLIB_PAIR_HPP
#define CPPLIB_PAIR_HPP

#include <stdint.h>

namespace cpplib {

	template<class Key_, class Data_>
	class KeyData {
	public:
		typedef Key_ TKey;
		typedef Data_ TData;

	protected:
		TKey key;
		TData data;

	public:
		void Set(const TKey& key, const TData& data) {
			this->key = key;
			this->data = data;
		}

		void Set(const TKey& key) {
			this->key = key;
		}

		void Set(const KeyData<TKey, TData>& r) {
			Set(r.key, r.data);
		}

		KeyData(TKey key, TData data)
			: key(key)
			, data(data)
		{

		}

		KeyData(TKey key)
			: key(key)
		{

		}

		KeyData(const KeyData<TKey, TData>& r)
			: key(r.key)
			, data(r.data)
		{

		}
		
		const TKey& Key() const { return key; }
		const TData& Data() const { return data; }

		TKey& Key() { return key; }
		TData& Data() { return data; }

	};
	
	template<class Key, class Data>
	short Compare(const KeyData<Key, Data>& a, const KeyData<Key, Data>& b) {
		return Compare(a.Key(), b.Key());
	}

	template<class Key, class Data>
	int64_t HashValue(const KeyData<Key, Data>& a) {
		return HashValue(a.Key());
	}

	template<class Key, class Data>
	void Duplicate(KeyData<Key, Data>& to, const KeyData<Key, Data>& from) {
		Duplicate(to.Key(), from.Key());
		Duplicate(to.Data(), from.Data());
	}

	template<class Key, class Data>
	void Free(KeyData<Key, Data>& kd) {
		Free(kd.Key());
		Free(kd.Data());
	}

	template<class _Data>
	class Pair : public KeyData<_Data, _Data> {
	public:
		Pair(_Data x, _Data y)
			: KeyData<_Data, _Data>(x, y)
		{

		}

		Pair()
			: KeyData<_Data, _Data>()
		{

		}

		const _Data& X() const { return this->key; }
		const _Data& Y() const { return this->data; }

		_Data& X() { return this->key; }
		_Data& Y() { return this->data; }

	};

	template<class Data>
	short Compare(const Pair<Data>& a, const Pair<Data>& b) {
		short cx = Compare(a.X(), b.X());
		if (cx < 0) return -1;
		if (cx > 0) return 1;
		return Compare(a.Y(), b.Y());
	}

	template<class Data>
	int64_t HashValue(const Pair<Data>& a) {
		return HashValue(a.X()) + HashValue(a.Y());
	}

}

#endif