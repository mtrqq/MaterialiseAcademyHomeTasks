#pragma once

#include <valarray>

template <typename TValueType, size_t X, size_t Y>
class Matrix;

template<typename DerivedClass>
struct traits {};

template <class Successor>
class MatrixWalker
{
public:
	MatrixWalker(const Matrix<typename traits<Successor>::type, traits<Successor>::X, traits<Successor>::Y>& matrix, size_t index)
		:matrix{ matrix },
		walkerIndex{ index } {}

	MatrixWalker(const MatrixWalker& another) :
		walkerIndex{ another.walkerIndex },
		matrix{ another.matrix } {}

	MatrixWalker& operator++()
	{
		++walkerIndex;
		return *this;
	}

	bool isValid() const
	{
		return walkerIndex < traits<Successor>::mg_iteration_count && walkerIndex >= 0;
	}
protected:
	const Matrix<typename traits<Successor>::type, traits<Successor>::X, traits<Successor>::Y>& matrix;
	size_t walkerIndex;
};

template<typename T, size_t N, size_t K, typename NeededType>
class ColumnWalker : public MatrixWalker<ColumnWalker<T, N, K,NeededType>>
{
public:
	ColumnWalker(const Matrix<T,N,K> &matrix, size_t index = 0)
		:MatrixWalker<ColumnWalker>{ matrix,index } {}

	// Copy ctor of base class ?

	std::valarray<NeededType> operator*()
	{
		std::valarray<NeededType> array(traits<ColumnWalker>::mg_token_size);
		for (size_t i = 0; i < traits<ColumnWalker>::mg_token_size; ++i)
		{
			array[i] = static_cast<NeededType>(this->matrix.At(i, this->walkerIndex));
		}
		return array;
	}
};

template<typename T, size_t N, size_t K, typename Needed>
struct traits<ColumnWalker<T, N, K, Needed>>
{
	static constexpr size_t X = N;
	static constexpr size_t Y = K;
	static constexpr size_t mg_iteration_count = K;
	static constexpr size_t mg_token_size = N;
	using type = T;
	using ValArrayType = Needed;
};

template<typename T, size_t N, size_t K, typename NeededType>
class RowWalker : public MatrixWalker<RowWalker<T, N, K, NeededType>>
{
public:
	RowWalker(const Matrix<T,N,K>& matrix, size_t index = 0)
		:MatrixWalker<RowWalker>{ matrix,index } {}

	std::valarray<NeededType> operator*() const
	{
		std::valarray<NeededType> array(traits<RowWalker>::mg_token_size);
		for (size_t i = 0; i < traits<RowWalker>::mg_token_size; ++i)
		{
			array[i] = static_cast<NeededType>(this->matrix.At(this->walkerIndex, i));
		}
		return array;
	}
};

template<typename T, size_t N, size_t K, typename Needed>
struct traits<RowWalker<T, N, K, Needed>>
{
	static constexpr size_t X = N;
	static constexpr size_t Y = K;
	static constexpr size_t mg_iteration_count = N;
	static constexpr size_t mg_token_size = K;
	using type = T;
	using ValArrayType = Needed;
};