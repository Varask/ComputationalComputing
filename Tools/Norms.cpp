#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>

struct Norms
{
    /// @brief  L1 / Manhattan norm of a vector
    /// @param v 
    /// @return 
    static long double L1(const std::vector<long double>& v)
    {
        long double sum = 0;
        for (long double x : v)
            sum += std::abs(x);
        return sum;
    }

    /// @brief  L2 / Euclidean norm of a vector
    /// @param v
    /// @return
    static long double L2(const std::vector<long double>& v)
    {
        long double sum = 0;
        for (long double x : v)
            sum += x * x;
        return std::sqrt(sum);
    }

    /// @brief  LInf / Chebyshev norm of a vector
    /// @param v
    /// @return 
    static long double LInf(const std::vector<long double>& v)
    {
        long double max = 0;
        for (long double x : v) {
            max = std::max(max, std::abs(x));  // Corrected comparison and update logic
        }
        return max;
    }


    /// @brief  Lp norm of a vector
    /// @param v the vector
    /// @param p the norm
    /// @return 
    static long double Lp(const std::vector<long double>& v, long double p)
    {
        long double sum = 0;
        for (auto x : v)
            sum += std::pow(std::abs(x), p);
        return std::pow(sum, 1.0 / p);
    }

    enum class NormType
    {
        L1,
        L2,
        LInf,
        Lp
    };    

    static long double calcNorm(const std::vector<long double>& v, NormType normType, long double p = 2)
    {
        switch (normType)
        {
        case NormType::L1:
            return L1(v);
        case NormType::L2:
            return L2(v);
        case NormType::LInf:
            return LInf(v);
        case NormType::Lp:
            return Lp(v, p);
        default:
            return 0;
        }
    }

    /// @brief  Interpolate vector to a given size
    /// @param v the vector
    /// @param newSize the target size
    /// @return interpolated vector
    static std::vector<long double> interpolate(const std::vector<long double>& v, size_t newSize)
    {
        std::vector<long double> result(newSize);
        long double scale = static_cast<long double>(v.size() - 1) / (newSize - 1);
        for (size_t i = 0; i < newSize; ++i)
        {
            long double pos = i * scale;
            size_t left = static_cast<size_t>(pos);
            size_t right = std::min(left + 1, v.size() - 1);
            long double weight = pos - left;
            result[i] = v[left] * (1 - weight) + v[right] * weight;
        }
        return result;
    }

    /// @brief  Normalize the Norm of a vector
    /// @param v_1 the 1st vector
    /// @param v_2 the 2nd vector
    /// @param normType the type of norm
    /// @return
    static long double Normalize(const std::vector<long double>& error_v, NormType normType, long double p = 2)
    {   
        int len_error = error_v.size();
        long double norm = calcNorm(error_v, normType, p);
        return norm / len_error;
    }
};