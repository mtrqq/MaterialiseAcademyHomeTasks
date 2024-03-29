#pragma once

#include <initializer_list>
#include <stdexcept>
#include <cmath>

namespace nostd
{
	template <unsigned DimensionsCount>
	class KDVector
	{
	public:
		KDVector(double initial_value = 0.0);
		template <typename Container>
		KDVector(const Container &init_list);

		double Length() const;
		double DotProduct(const KDVector<DimensionsCount> &other) const;
		double AngleBetween(const KDVector<DimensionsCount> &other) const;

		KDVector<DimensionsCount> UnitVector() const;

		KDVector<DimensionsCount> operator-(const KDVector<DimensionsCount> &i_other) const;
		KDVector<DimensionsCount> operator+(const KDVector<DimensionsCount> &i_other) const;
		KDVector<DimensionsCount> operator*(double i_scalar) const;

		double& operator[](unsigned index)
		{
			return m_coordinates[index];
		}
		double& At(unsigned index)
		{
			if (index >= DimensionsCount) throw std::out_of_range{ "vector subscript is out of range" };
			return m_coordinates[index];
		}

		const double& At(unsigned index) const
		{
			if (index >= DimensionsCount) throw std::out_of_range{ "vector subscript is out of range" };
			return m_coordinates[index];
		}

	private:
		double m_coordinates[DimensionsCount];

		template <typename Proccessor>
		KDVector<DimensionsCount> BinaryOperation(const KDVector<DimensionsCount>& i_other,
			const Proccessor& function) const;

		template <typename Proccessor>
		KDVector<DimensionsCount> UnaryOperation(const Proccessor& function) const;
	};

	template<unsigned DimensionsCount>
	KDVector<DimensionsCount>::KDVector(double initial_value)
	{
		for (auto &element : m_coordinates)
		{
			element = initial_value;
		}
	}

	template<unsigned DimensionsCount>
	template <typename Container>
	KDVector<DimensionsCount>::KDVector(const Container &init_list)
	{
		unsigned entry_counter{};
		for (auto &element : init_list)
		{
			m_coordinates[entry_counter++] = element;
		}
	}

	template<unsigned DimensionsCount>
	double KDVector<DimensionsCount>::Length() const
	{
		double accumulation{};
		for (unsigned i = 0; i < DimensionsCount; ++i)
		{
			accumulation += m_coordinates * m_coordinates;
		}
		return std::sqrt(accumulation);
	}

	template<unsigned DimensionsCount>
	double KDVector<DimensionsCount>::DotProduct(const KDVector<DimensionsCount>& other) const
	{
		double result{};
		for (unsigned i = 0; i < DimensionsCount; ++i)
		{
			result += m_coordinates[i] * other.At(i);
		}
		return result;
	}

	template<unsigned DimensionsCount>
	double KDVector<DimensionsCount>::AngleBetween(const KDVector<DimensionsCount>& other) const
	{
		return std::acos(DotProduct(other) / (Length() * other.Length()));
	}

	template<unsigned DimensionsCount>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::UnitVector() const
	{
		auto length = Length();
		return UnaryOperation([length](const double &proccessed) {return proccessed / length; });
	}

	template<unsigned DimensionsCount>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::operator-(const KDVector<DimensionsCount>& i_other) const
	{
		return BinaryOperation(i_other, std::minus<double>{});
	}

	template<unsigned DimensionsCount>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::operator+(const KDVector<DimensionsCount>& i_other) const
	{
		return BinaryOperation(i_other, std::plus<double>{});
	}

	template<unsigned DimensionsCount>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::operator*(double i_scalar) const
	{
		return UnaryOperation([i_scalar](const double &proccesed) {return proccesed / i_scalar; });
	}

	template<unsigned DimensionsCount>
	template<typename Proccessor>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::BinaryOperation(const KDVector<DimensionsCount>& i_other, const Proccessor & function) const
	{
		KDVector<DimensionsCount> result;
		for (unsigned i = 0; i < DimensionsCount; ++i)
		{
			result.At(i) = function(m_coordinates[i], i_other.At(i));
		}
		return result;
	}

	template<unsigned DimensionsCount>
	template<typename Proccessor>
	KDVector<DimensionsCount> KDVector<DimensionsCount>::UnaryOperation(const Proccessor& function) const
	{
		KDVector<DimensionsCount> result;
		for (unsigned i = 0; i < DimensionsCount; ++i)
		{
			result.At(i) = function(m_coordinates[i]);
		}
		return result;
	}

}