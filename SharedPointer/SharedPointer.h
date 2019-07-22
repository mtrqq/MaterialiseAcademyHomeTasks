#pragma once

#include "SharedControlBlock.h"

#include <utility>

namespace nostd
{

	template<typename TValueType>
	class SharedPointer
	{
	public:
		SharedPointer() = default;

		SharedPointer(TValueType* pointer);

		SharedPointer(const SharedPointer &another);

		size_t uses_count() const
		{
			return mp_control_block ? mp_control_block->shared_ptr_count() : 0;
		}

		const TValueType* get() const
		{
			return mp_control_block->get();
		}

		TValueType* get()
		{
			return mp_control_block->get();
		}

		operator bool() const
		{
			return mp_control_block != nullptr &&
				mp_control_block->get() != nullptr;
		}

		void reset(TValueType* i_pointer);

		~SharedPointer();
	private:
		SharedControlBlockBase<TValueType> *mp_control_block;

		SharedPointer(SharedControlBlockBase<TValueType> *block)
			:mp_control_block{ block } 
		{
			++mp_control_block->shared_ptr_count();
		}

		template <typename TValueType>
		friend class WeakPointer;

		/*template<typename TValueType, typename ...Args>
		friend SharedPointer<TValueType> make_shared(Args &&... args);*/
	};

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(TValueType * pointer)
	{
		reset(pointer);
	}

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(const SharedPointer & another)
	{
		mp_control_block = another.mp_control_block;
		++mp_control_block->shared_ptr_count();
	}

	template<typename TValueType>
	void SharedPointer<TValueType>::reset(TValueType * i_pointer)
	{
		if (mp_control_block)
		{
			--mp_control_block->shared_count;
		}
		mp_control_block = new RemoteBlock{ i_pointer };
		++mp_control_block->shared_ptr_count();
	}

	template<typename TValueType>
	SharedPointer<TValueType>::~SharedPointer()
	{
		if (mp_control_block && mp_control_block->is_valid())
		{
			if (mp_control_block->shared_ptr_count() == 0)
			{
				mp_control_block->destroyObject();
				if (mp_control_block->weak_ptr_count() == 0)
				{
					delete mp_control_block;
				}
			}
		}
	}

}
