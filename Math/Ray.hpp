#pragma once
#include "Vector2Int.hpp"

namespace Math
{
    struct Ray
    {
        /// <summary>
        /// Posi��o inicial do raio.
        /// </summary>
        Vector2Int StartPosition;

        /// <summary>
        /// Dire��o do raio.
        /// </summary>
        Vector2Int Direction;

        /// <summary>
        /// Dist�ncia total do raio.
        /// </summary>
        int Lenght;

        /// <summary>
        /// Dist�ncia inicial que deve ser ignorada no come�o do raio.
        /// </summary>
        int StartLenght;

        Ray(const Vector2Int& startPosition, const Vector2Int& direction, int lenght, int startLenght = 0);

        /// <summary>
        /// Checa se um raio traca uma posi��o.
        /// </summary>
        /// <param name="position">Posi��o a ser testada.</param>
        /// <returns>Verdadeiro caso trace e falso caso contr�rio.</returns>
        bool TracesPosition(const Vector2Int& position) const;

        /// <summary>
        /// Fun��o para obter o valor de uma posi��o em uma dist�ncia espec�fica do raio.
        /// </summary>
        /// <param name="n">Dist�ncia desejada.</param>
        /// <returns>Um vetor com as coordenadas da posi��o naquela dist�ncia.</returns>
        Vector2Int Position(int n) const;

    };
}