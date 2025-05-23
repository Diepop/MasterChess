#pragma once
#include "IGameListener.hpp"
#include "IPiece.hpp"
#include "Math/Area.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace MasterChess
{
    struct Game;
    struct HitInfo;
    struct IPiece;
    struct IPlayer;
    struct IMovement;

    using std::vector;
    using std::unique_ptr;
    using Math::Vector2Int;
    using Math::Ray;
    using Math::Area;

    /// <summary>
    /// Interface de um tabuleiro de dimens�es n�o espec�ficadas.
    /// </summary>
    struct IBoard : IGameListener
    {
        virtual ~IBoard() = default;

        /// <summary>
        /// Obt�m as pe�as atuais do tabuleiro.
        /// </summary>
        /// <returns>Um vetor com todas as pe�as do tabuleiro.</returns>
        virtual vector<IPiece*> Pieces() const = 0;

        /// <summary>
        /// Obt�m a area v�lida do tabuleiro.
        /// </summary>
        /// <returns></returns>
        virtual const Area& BoardArea() const = 0;

        /// <summary>
        /// Retorna a pe�a que est� no posi��o solicitada.
        /// </summary>
        /// <param name="position">Posi��o que deve ser checada.</param>
        /// <returns>Um pointer para a pe�a que est� na posi��o checada ou nullptr se a casa estiver vazia.</returns>
        /// <remarks>A posi��o deve ser uma posi��o v�lida!</remarks>
        virtual IPiece* At(const Vector2Int& position) const = 0;

        /// <summary>
        /// Retorna a posi��o de uma pe�a que est� no tabuleiro.
        /// </summary>
        /// <param name="piece">Pe�a a ser localizada.</param>
        /// <returns>Um vetor com a posi��o da pe�a.</returns>
        /// <remarks>A pe�a deve estar no tabuleiro na hora da checagem!</remarks>
        virtual Vector2Int FindPieceLocation(IPiece* piece) const = 0;

        /// <summary>
        ///	Adiciona uma pe�a no tabuleiro no local solicitado.
        /// </summary>
        /// <param name="piece">Pe�a a ser adicionada.</param>
        /// <param name="position">Posi��o a ser ocupada.</param>
        /// <remarks>A casa da posi��o solicitada deve estar vazia!</remarks>
        virtual void AddPiece(IPiece* piece, const Vector2Int& position) = 0;

        /// <summary>
        /// Checa se uma pe�a est� incluida no tabuleiro.
        /// </summary>
        /// <param name="piece">Pe�a a ser testada.</param>
        /// <returns>Verdadeira caso esteja e falso caso contr�rio.</returns>
        virtual bool ContainsPiece(IPiece* piece) const = 0;

        /// <summary>
        /// Remove uma pe�a do tabuleiro.
        /// </summary>
        /// <param name="piece">Pe�a a ser removida.</param>
        /// <remarks>A pe�a deve estar no tabuleiro!</remarks>
        virtual void RemovePiece(IPiece* piece) = 0;

        /// <summary>
        /// Reposiciona uma pe�a no tabuleiro.
        /// </summary>
        /// <param name="piece">Pe�a a ser reposicionada.</param>
        /// <param name="position">Posi��o destinada a pe�a</param>
        /// <remarks>A pe�a deve estar no tabuleiro atual e a posi��o solicitada deve ser v�lida!</remarks>
        virtual void RepositionPiece(IPiece* piece, const Vector2Int& position) = 0;

        /// <summary>
        /// Tra�a um raio que colide com a primeira pe�a que esteja no caminho
        /// </summary>
        /// <param name="ray"></param>
        /// <param name="hitInfo"></param>
        /// <returns></returns>
        bool CastRay(const Ray& ray, HitInfo* hitInfo) const;

        void OnGameStart(Game* game) override
        {
            for (auto ptr : Pieces())
                ptr->OnGameStart(game);
        }

    };

}
