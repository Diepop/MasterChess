#pragma once
#include "Math/Vector2Int.hpp"

#include <string_view>

#include <format>

namespace MasterChess
{
    using Math::Vector2Int;

    struct IConsole
    {
        /// <summary>
        /// Adiciona um valor na stack.
        /// Ap�s o seu uso todos os comandos para desenhar s�o somados a essa posi��o at� que a PopOffset seja chamada.
        /// </summary>
        /// <param name="offset">Um vetor com o valor a ser adicionado ao offset.</param>
        /// <remarks>
        ///	Offsets subsequentes s�o adicionados cumultativamente.
        ///	A fun��o PopOffset deve ser chamada logo em seguida que o offset n�o for mais nescess�rio.
        /// </remarks>
        virtual void PushOffset(const Vector2Int& offset) = 0;

        /// <summary>
        /// Retira o �ltimo offset que foi colocado na pilha efetivamente o descartando.
        /// </summary>
        virtual void PopOffset() = 0;

        /// <summary>
        /// Limpa o buffer da tela.
        /// </summary>
        virtual void Clear() = 0;

        /// <summary>
        /// Passa o conte�do do buffer para a tela.
        /// </summary>
        virtual void Display() const = 0;

        /// <summary>
        /// Desenha a borda de um quadrado no buffer.
        /// </summary>
        /// <param name="position">Posi��o superior esquerda do quadrado.</param>
        /// <param name="size">Tamanho do quadrado.</param>
        /// <param name="d">Digito a ser usado no preenchimento</param>
        virtual void DrawSquareBorder(const Vector2Int& position, const Vector2Int& size, char d) = 0;

        /// <summary>
        /// Desenha um quadrado no buffer.
        /// </summary>
        /// <param name="position">Posi��o superior esquerda do quadrado.</param>
        /// <param name="size">Tamanho do quadrado.</param>
        /// <param name="d">Digito a ser usado no preenchimento</param>
        virtual void DrawSquare(const Vector2Int& position, const Vector2Int& size, char d) = 0;

        /// <summary>
        /// Desenha um "pixel" no buffer.
        /// </summary>
        /// <param name="position">Posi��o a ser preenchida.</param>
        /// <param name="d">Digito a ser usado.</param>
        virtual void DrawPixel(const Vector2Int& position, char d) = 0;

        /// <summary>
        /// Desenha um texto na tela.
        /// </summary>
        /// <param name="position">Posi��o do primeiro caract�r.</param>
        /// <param name="sv">Texto a ser desenhado.</param>
        virtual void DrawString(const Vector2Int& position, std::string_view sv) = 0;

        template<class... Args>
            requires (sizeof...(Args) != 0)
        void DrawString(const Vector2Int& position, std::format_string<Args...> sv, Args&&... args)
        {
            return DrawString(position, std::format(sv, std::forward<Args>(args)...));
        }

    };

}
