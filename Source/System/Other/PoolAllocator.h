#pragma once

#include<vector>

// class Tを最大MAXSIZE個確保可能なアロケーター
template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
	// コンストラクタ
	PoolAllocator() { }

	// デストラクタ
	~PoolAllocator()
	{
		freelist = nullptr;
	}

	//メモリを最大数確保する
	void Initialize()
	{
		//メモリをMAXSIZE分確保する
		buffer = std::vector<ElementType>(MAXSIZE);

		//bufferの各要素のnextを設定する(i番最後の要素はnullにしたいので省く)
		for (int index = 0; index < MAXSIZE - 1; index++)
		{
			buffer.at(index).next = reinterpret_cast<std::uintptr_t>(&(buffer.at(index + 1)));
		}

		//一番最後の要素はnextがないのでnullにする
		buffer.back().next = reinterpret_cast<std::uintptr_t>(nullptr);

		//まだどれも未使用なのでbufferの0番目をfreelistに登録する
		freelist = &(buffer.front());
	}

	// 確保できない場合はnullptrを返す
	T* Alloc()
	{
		//freelistがnullptrなら確保出来るメモリがないのでnullを返す
		if (freelist == nullptr)
		{
			return nullptr;
		}

		//freelistにある要素を戻り値用変数に割り当て、freelistを次の要素に更新する
		void* result = reinterpret_cast<void*>(freelist);
		freelist = reinterpret_cast<ElementType*>(freelist->next);

		return reinterpret_cast<T*>(result);
	}

	//解放
	void Free(T* addr)
	{
		//addrがnullptrなら無視する
		if (addr != nullptr)
		{
			//解放するメモリのnextを現在のfreelistに更新してその後freelistを更新する
			ElementType* element = reinterpret_cast<ElementType*>(addr);
			element->next = reinterpret_cast<std::uintptr_t>(freelist);
			freelist = element;
		}
	}

	T* GetBuffer(size_t index)
	{
		return reinterpret_cast<T*>(&buffer.at(index).v);
	}

private:
	union ElementType
	{
		ElementType(){}
		~ElementType(){}

		T v;
		std::uintptr_t next;
	};

	std::vector<ElementType> buffer;//MAXSIZE分確保するメモリの配列
	ElementType* freelist;//未使用のbuffer内の要素をいれる

};
