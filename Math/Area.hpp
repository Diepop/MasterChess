#pragma once
#include "Vector2Int.hpp"
#include "Ray.hpp"

#include <unordered_set>
#include <vector>

namespace Math
{
    using std::unordered_set;
    using std::vector;

    /// <summary>
    /// Representa uma �rea no tabuleiro.
    /// </summary>
    struct Area
    {
        template<class... Args>
        Area(Args&&... args)
        {
            ((*this |= args), ...);
        }

        Area(Area&&) noexcept = default;
        Area(const Area&) = default;

        /// <summary>
        /// Adiciona uma posi��o na �rea.
        /// </summary>
        /// <param name="position">Posi��o a ser inclu�da.</param>
        void AddPosition(const Vector2Int& position);

        /// <summary>
        /// Remove uma posi��o da �rea.
        /// </summary>
        /// <param name="position">Posi��o a ser exclu�da.</</param>
        void RemovePosition(const Vector2Int& position);

        /// <summary>
        /// Adiciona um raio na �rea.
        /// </summary>
        /// <param name="ray">Raio a ser inclu�do.</param>
        void AddRay(const Ray& ray);

        /// <summary>
        /// Checa se uma posi��o est� dentro da �rea.
        /// </summary>
        /// <param name="position">Posi��o a ser averiguada.</param>
        /// <returns>Verdadeiro caso a posi��o esteja dentro da �rea e falso caso fora.</returns>
        bool IncludesPosition(const Vector2Int& position) const;

        /// <summary>
        /// Concede o valor das posi��es de uma �rea.
        /// </summary>
        /// <returns></returns>
        vector<Vector2Int> Positions() const;

        Area& operator|=(const Area& rhs);

        Area& operator&=(const Area& rhs);

        Area& operator|=(const Vector2Int& rhs);

        Area& operator&=(const Vector2Int& rhs);

        Area& operator|=(const Ray& rhs);

        Area& operator&=(const Ray& rhs);

        template<class Rhs>
        friend Area operator|(Area lhs, Rhs&& rhs)
        {
            return lhs |= rhs;
        }

        template<class Rhs>
        friend Area operator&(Area lhs, Rhs&& rhs)
        {
            return lhs &= rhs;
        }

    private:
        struct Hasher { size_t operator()(const Vector2Int& v) const { return reinterpret_cast<const size_t&>(v); } };
        unordered_set<Vector2Int, Hasher> positions;
    };

}
