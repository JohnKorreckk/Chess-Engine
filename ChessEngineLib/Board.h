/**
 * @file Board.h
 * @author John Korreck
 *
 *
 */
 
#ifndef BOARD_H
#define BOARD_H

#include "Square.h"
#include "Item.h"

#include <unordered_set>

class Board : public Item {
private:
 /// The actual board object
 std::vector<std::vector<int>> mBoard{8, std::vector<int>(8, 0)};
 /// The current relative position
 wxPoint mRelativePosition = wxPoint(0,0);
 /// The current board position
 std::wstring mChessPosition = L"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
 /// The current player's color
 bool isWhite = true;
 /// All pieces in the board
 std::vector<std::shared_ptr<Square>> mSquares;
 /// All pieces in the board
 std::vector<std::shared_ptr<Piece>> mPieces;
 /// All possible moves
 std::vector<std::wstring> mPossibleMoves;
 /// Whose turn is it?
 bool mWhiteTurn = true;


public:
 /// Destructor
 virtual ~Board() = default;

 /// Disabled copy constructor
 Board(const Board &) = delete;

 /// Default constructor
 Board() = delete;

 /// Constructor
 Board(std::wstring& name, std::wstring resourcesDir);

 /// Disabled assignment operator
 void operator=(const Board &) = delete;

 /// Update the current position
 void UpdateChessPosition(std::wstring chessPosition) {mChessPosition = chessPosition;};

 /// Get the current relative position
 void SetRelativePosition(wxPoint pos) {mRelativePosition = pos;}

 std::vector<std::vector<int>> GetBoard() {return mBoard; };

 /// Set the current position
 wxPoint GetRelativePosition() { return mRelativePosition; };

 void SetWhiteTurn(bool whiteTurn) { mWhiteTurn = whiteTurn; }

 bool GetWhiteTurn() { return mWhiteTurn; }

 /**
  * Interprets Fen position and returns an 8x8 vector containing the board and its pieces
  * @param fenString
  * @return The vector corresponding to the fen string
  */
 std::vector<std::vector<int>> FenParser(std::wstring fenString);
 std::shared_ptr<Square> GetClosestSquare(wxPoint pos) override;
 void GeneratePossibleMoves();
 std::vector<int> GenerateSlidingMoves(std::vector<std::vector<int>> board);
 void AddSquare(std::shared_ptr<Square> square) { mSquares.push_back(square); }
 std::vector<std::shared_ptr<Square>> GetSquares() { return mSquares; }
 void AddPiece(std::shared_ptr<Piece> piece) { mPieces.push_back(piece); }
 std::vector<std::shared_ptr<Piece>> GetPieces() { return mPieces; }
 std::shared_ptr<Piece> HitTest(wxPoint pos);
 void UpdateBoard(std::wstring const &move);
 std::vector<std::wstring> GetPossibleMoves() { return mPossibleMoves; }
};



#endif //BOARD_H
