#include "Bishop.h"
#include <iostream>

Bishop::Bishop ()
{
   compute_moves ();
}

Bishop::~Bishop ()
{
}

/*==============================================================================
  Get all moves from SQUARE in the current BOARD assumming it is PLAYER'S turn
  to move (moves that may leave the king in check are also included)

  TODO: write a high-level description of the algorithm used to find bishop valid
  moves
  ==============================================================================*/
bitboard
Bishop::get_moves (uint square, Piece::Player player, const Board* board) const
{
   bitboard attacks = 0;
   bitboard blocking_pieces;
   bitboard all_pieces;
   Board::Squares blocker; // First blocking piece

   all_pieces = board->get_all_pieces ();

   // Compute attacks to each of the possible directions a bishop can point to
   for (Diagonal ray = NORTH_EAST; ray <= NORTH_WEST; ++ray)
   {
      blocking_pieces = get_ray (Board::Squares (square), ray) & all_pieces;

      if (ray == NORTH_EAST || ray == NORTH_WEST)
         blocker = Board::Squares (Util::MSB_position (blocking_pieces));
      else
         blocker = Board::Squares (Util::LSB_position (blocking_pieces));

      attacks |=
            get_ray (Board::Squares (square), ray) ^
            (blocking_pieces ? get_ray (blocker, ray) : 0);
   }
   attacks &= ~board->get_pieces (player);

   return attacks;
}

/*==============================================================================
  Compute all moves a bishop can make from every square on the board assuming
  the board is empty.
  ==============================================================================*/
void
Bishop::compute_moves ()
{
   int dx[Piece::RAYS] = { +1, +1, -1, -1 };
   int dy[Piece::RAYS] = { -1, +1, +1, -1 };
   bitboard one = 1;

   for (Board::Squares square = Board::a8; square <= Board::h1; ++square)
   {
      for (uint ray = 0; ray < Piece::RAYS; ++ray)
         this->moves_from[square][ray] = 0;

      this->all_moves_from[square] = 0;
   }

   for (uint row = 0; row < Board::SIZE; ++row)
      for (uint col = 0; col < Board::SIZE; ++col)
      {
         Board::Squares square = Board::Squares (row * Board::SIZE + col);
         Diagonal ray;

         /*--------------------------------------------------------------------
           Traverse all four directions a bishop can move to

             NW   NE
              \   /     NW : North West
               \ /      NE : North East
               / \      SE : South East
              /   \     SW : South West
             SW   SE
         ---------------------------------------------------------------------*/
         for (ray = Piece::NORTH_EAST; ray <= Piece::NORTH_WEST; ++ray)
         {
            int y = row;
            int x = col;
            while (Board::is_inside_board (y + dy[ray], x + dx[ray]))
            {
               y += dy[ray];
               x += dx[ray];
               this->moves_from[square][ray] |= (one << (y * Board::SIZE + x));
            }
            this->all_moves_from[square] |= this->moves_from[square][ray];
         }
      }
}

/*==============================================================================
  Return all possible moves from SQUARE, assuming the board is empty.
  ==============================================================================*/
bitboard
/* Only for pawns is the player to move relevant in computing the potential moves */
Bishop::get_potential_moves (uint square, Player /* player */) const
{
   if (Board::is_inside_board (square))
      return this->all_moves_from[square];

   return 0;
}

/*==============================================================================
  Return all possible moves from SQUARE following DIRECTION, and assuming the
  board is empty.
  ==============================================================================*/
bitboard
Bishop::get_ray (Board::Squares square, Diagonal direction) const
{
   if (Board::is_inside_board (square))
      return this->moves_from[square][direction];

   return 0;
}
