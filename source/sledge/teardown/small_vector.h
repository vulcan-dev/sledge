#pragma once
#include "teardown/functions/memory.h"

template<typename T>
	class small_vector {
	public:
		small_vector() {};
		small_vector(uint32_t capacity) { reserve(capacity); };
		small_vector(const small_vector&) = delete;
		void operator=(const small_vector&) = delete;
		~small_vector() { clear(); }

		T* begin() const { return m_Data; }
		T* end() const { return m_Data + m_Size; }
		T* data() const { return m_Data; }
		uint32_t size() const { return m_Size; }
		uint32_t capacity() const { return m_Capacity; }

		void push_back(T& value)
		{
			if (m_Capacity <= m_Size)
				reserve(2 * m_Capacity + 1);

			//m_Data[m_Size] = value;
			unsigned __int64 dwWritePos = reinterpret_cast<unsigned __int64>(m_Data) + (m_Size * sizeof(T));

			memcpy(reinterpret_cast<void*>(dwWritePos), &value, sizeof(T));
			++m_Size;
		}

		T get_at(unsigned int iIdx) {
			if (iIdx == 0 || iIdx > m_Size)
				return nullptr;
			return m_Data[iIdx];
		}

		void reserve(unsigned int iCapacity) {
			void* pMemory = Teardown::alloc(iCapacity * sizeof(T));

			if (this->m_Data != nullptr) {
				if (this->m_Size > iCapacity)
					this->m_Size = iCapacity;
				
				memset(pMemory, 0, this->m_Size);
				memcpy(pMemory, this->m_Data, sizeof(T) * this->m_Size);
				Teardown::free(this->m_Data);
			}
			else {
				this->m_Size = 0;
			}
			this->m_Data = reinterpret_cast<T*>(pMemory);
			this->m_Capacity = iCapacity
;		}

		void clear()
		{
			if (m_Data != nullptr)
			{
				m_Size = 0;
				m_Capacity = 0;
				Teardown:free(m_Data);
				m_Data = nullptr;
			}
		}

	private:
		uint32_t m_Size = 0;
		uint32_t m_Capacity = 0;
		T* m_Data = nullptr;
	};