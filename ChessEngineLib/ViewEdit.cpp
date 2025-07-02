/**
 * @file ViewEdit.cpp
 * @author Charles B. Owen
 */
#include "pch.h"

#include <wx/dcbuffer.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

#include "ViewEdit.h"
#include "Picture.h"
#include "Square.h"
#include "Item.h"
#include "Drawable.h"
#include "Board.h"
#include "Piece.h"

/// A scaling fItem, converts mouse motion to rotation in radians
const double RotationScaling = 0.02;

/**
 * Constructor
 * @param parent Pointer to wxFrame object, the main frame for the application
 * @param resourcesDir the directory of images to choose from
 */
ViewEdit::ViewEdit(wxFrame* parent, std::wstring resourcesDir) :wxScrolledCanvas(parent, wxID_ANY), mResourcesDir(resourcesDir)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    Bind(wxEVT_PAINT, &ViewEdit::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &ViewEdit::OnLeftDown, this);
    Bind(wxEVT_LEFT_UP, &ViewEdit::OnLeftUp, this);
    Bind(wxEVT_LEFT_DCLICK, &ViewEdit::OnLeftDoubleClick, this);
    Bind(wxEVT_MOTION, &ViewEdit::OnMouseMove, this);
}

/**
 * Force an update of this window when the picture changes.
 */
void ViewEdit::UpdateObserver()
{
    Refresh();
}



/**
 * Paint event, draws the window.
 * @param event Paint event object
 */
void ViewEdit::OnPaint(wxPaintEvent& event)
{
    auto size = GetPicture()->GetSize();
    SetVirtualSize(size.GetWidth(), size.GetHeight());
    SetScrollRate(1, 1);

    wxAutoBufferedPaintDC dc(this);
    DoPrepareDC(dc);

    wxBrush background(*wxWHITE);
    dc.SetBackground(background);
    dc.Clear();

    // Create a graphics context
    auto graphics = std::shared_ptr<wxGraphicsContext>(wxGraphicsContext::Create( dc ));

    GetPicture()->Draw(graphics);
}

/**
 * Handle the left mouse button down event
 * @param event
 */
void ViewEdit::OnLeftDown(wxMouseEvent &event)
{
    auto click = CalcUnscrolledPosition(event.GetPosition());
    mLastMouse = click;

    //
    // Did we hit anything?
    //

    std::shared_ptr<Item> hitItem;
    std::shared_ptr<Drawable> hitDrawable;
    for (auto item : *GetPicture())
    {
        // Note: We do not exit when we get the first hit, since
        // we are looking at these in drawing order. Instead, we
        // keep the last hit.
        std::shared_ptr<Drawable> drawable = item->HitTest(wxPoint(click.x, click.y));
        if (drawable != nullptr)
        {
            hitItem = item;
            hitDrawable = drawable;
        }
    }

    std::shared_ptr<Piece> piece = GetPicture()->GetBoard()->HitTest(wxPoint(click.x, click.y));
    std::shared_ptr<Board> hitBoard;
    std::shared_ptr<Piece> hitPiece;
    if (piece != nullptr)
    {
        hitBoard = GetPicture()->GetBoard();
        hitPiece = piece;
    }

    // If we hit something determine what we do with it based on the
    // current mode.
    if (hitItem != nullptr)
    {
        mSelectedItem = hitItem;
        mSelectedDrawable = hitDrawable;
    }
    // If we hit something determine what we do with it based on the
    // current mode.
    if (hitBoard != nullptr)
    {
        mBoard = hitBoard;
        mBoard->GeneratePossibleMoves(false);
        mSelectedPiece = hitPiece;
        mSelectedDrawable = hitPiece;
        mBoard->MoveToBack(mSelectedPiece);
    }
}

/**
* Handle the left mouse button up event
* @param event
*/
void ViewEdit::OnLeftUp(wxMouseEvent &event)
{
    if (mSelectedPiece && mSelectedPiece->IsMovable())
    {
        bool whiteTurn = mBoard->GetWhiteTurn();
        /// Size of each square
        const int squareSize = 75;
        std::shared_ptr<Square> newSquare = mBoard->GetClosestSquare(wxPoint(mSelectedPiece->GetPosition().x + 35, mSelectedPiece->GetPosition().y + 30));
        std::wstring move = mSelectedPiece->GetSquare()->GetName() + newSquare->GetName();
        std::vector<std::wstring> possibleMoves = mBoard->GetPossibleMoves();
        if (possibleMoves.empty())
        {
            wxPoint oldPos = mSelectedPiece->GetSquare()->GetPosition();
            mSelectedPiece->SetPosition(wxPoint(oldPos.x-(squareSize/2), oldPos.y-(squareSize/2)));
            mBoard->displayWinner();
            std::cout << "WIN" << std::endl;
            GetPicture()->UpdateObservers();
        }
        else if (move == L"e1h1" || move == L"e1g1")
        {
            if (std::find(possibleMoves.begin(), possibleMoves.end(), L"OO") != possibleMoves.end())
            {
                std::shared_ptr<Square> newRookSquare;
                std::shared_ptr<Square> newKingSquare;
                std::shared_ptr<Square> oldRookSquare;
                std::shared_ptr<Square> oldKingSquare;
                Piece* rook = nullptr;

                for (auto square : mBoard->GetSquares())
                {

                    if (square->GetName() == L"f1")
                    {
                        newRookSquare = square;
                    }
                    else if (square->GetName() == L"g1")
                    {
                        newKingSquare = square;
                    }
                    else if (square->GetName() == L"h1")
                    {
                        oldRookSquare = square;
                        rook = square->GetPiece();
                    }
                    else if (square->GetName() == L"e1")
                    {
                        oldKingSquare = square;
                    }
                }
                newKingSquare->SetPiece(&*mSelectedPiece);
                if (rook)
                {
                    newRookSquare->SetPiece(rook);
                    rook->SetPosition(wxPoint(newRookSquare->GetCenter().x-(squareSize/2), newRookSquare->GetCenter().y-(squareSize/2)));
                }
                mSelectedPiece->SetPosition(wxPoint(newKingSquare->GetCenter().x-(squareSize/2), newKingSquare->GetCenter().y-(squareSize/2)));
                oldKingSquare->SetPiece(nullptr);
                oldRookSquare->SetPiece(nullptr);
                mBoard->UpdateBoard(L"OO");
                std::cout << "Update Board Called" << std::endl;
                mBoard->SetWhiteTurn(!whiteTurn);
                mBoard->GeneratePossibleMoves(false);
                GetPicture()->UpdateObservers();
            }
        }
        else if (move == L"e8h8" || move == L"e8g8")
        {
            if (std::find(possibleMoves.begin(), possibleMoves.end(), L"OO") != possibleMoves.end())
            {
                std::shared_ptr<Square> newRookSquare;
                std::shared_ptr<Square> newKingSquare;
                std::shared_ptr<Square> oldRookSquare;
                std::shared_ptr<Square> oldKingSquare;
                Piece* rook = nullptr;

                for (auto square : mBoard->GetSquares())
                {

                    if (square->GetName() == L"f8")
                    {
                        newRookSquare = square;
                    }
                    else if (square->GetName() == L"g8")
                    {
                        newKingSquare = square;
                    }
                    else if (square->GetName() == L"h8")
                    {
                        oldRookSquare = square;
                        rook = square->GetPiece();
                    }
                    else if (square->GetName() == L"e8")
                    {
                        oldKingSquare = square;
                    }
                }
                newKingSquare->SetPiece(&*mSelectedPiece);
                if (rook)
                {
                    newRookSquare->SetPiece(rook);
                    rook->SetPosition(wxPoint(newRookSquare->GetCenter().x-(squareSize/2), newRookSquare->GetCenter().y-(squareSize/2)));
                }
                mSelectedPiece->SetPosition(wxPoint(newKingSquare->GetCenter().x-(squareSize/2), newKingSquare->GetCenter().y-(squareSize/2)));
                oldKingSquare->SetPiece(nullptr);
                oldRookSquare->SetPiece(nullptr);
                mBoard->UpdateBoard(L"OO");
                std::cout << "Update Board Called" << std::endl;
                mBoard->SetWhiteTurn(!whiteTurn);
                mBoard->GeneratePossibleMoves(false);
                GetPicture()->UpdateObservers();
            }
        }
        else if (move == L"e8a8" || move == L"e8b8")
        {
            if (std::find(possibleMoves.begin(), possibleMoves.end(), L"OOO") != possibleMoves.end())
            {
                std::shared_ptr<Square> newRookSquare;
                std::shared_ptr<Square> newKingSquare;
                std::shared_ptr<Square> oldRookSquare;
                std::shared_ptr<Square> oldKingSquare;
                Piece* rook = nullptr;

                for (auto square : mBoard->GetSquares())
                {

                    if (square->GetName() == L"d8")
                    {
                        newRookSquare = square;
                    }
                    else if (square->GetName() == L"c8")
                    {
                        newKingSquare = square;
                    }
                    else if (square->GetName() == L"a8")
                    {
                        oldRookSquare = square;
                        rook = square->GetPiece();
                    }
                    else if (square->GetName() == L"e8")
                    {
                        oldKingSquare = square;
                    }
                }
                newKingSquare->SetPiece(&*mSelectedPiece);
                if (rook)
                {
                    newRookSquare->SetPiece(rook);
                    rook->SetPosition(wxPoint(newRookSquare->GetCenter().x-(squareSize/2), newRookSquare->GetCenter().y-(squareSize/2)));
                }
                mSelectedPiece->SetPosition(wxPoint(newKingSquare->GetCenter().x-(squareSize/2), newKingSquare->GetCenter().y-(squareSize/2)));
                oldKingSquare->SetPiece(nullptr);
                oldRookSquare->SetPiece(nullptr);
                mBoard->UpdateBoard(L"OOO");
                std::cout << "Update Board Called" << std::endl;
                mBoard->SetWhiteTurn(!whiteTurn);
                mBoard->GeneratePossibleMoves(false);
                GetPicture()->UpdateObservers();
            }
        }
        else if (move == L"e1a1" || move == L"e1b1")
        {
            if (std::find(possibleMoves.begin(), possibleMoves.end(), L"OOO") != possibleMoves.end())
            {
                std::shared_ptr<Square> newRookSquare;
                std::shared_ptr<Square> newKingSquare;
                std::shared_ptr<Square> oldRookSquare;
                std::shared_ptr<Square> oldKingSquare;
                Piece* rook = nullptr;

                for (auto square : mBoard->GetSquares())
                {

                    if (square->GetName() == L"d1")
                    {
                        newRookSquare = square;
                    }
                    else if (square->GetName() == L"c1")
                    {
                        newKingSquare = square;
                    }
                    else if (square->GetName() == L"a1")
                    {
                        oldRookSquare = square;
                        rook = square->GetPiece();
                    }
                    else if (square->GetName() == L"e1")
                    {
                        oldKingSquare = square;
                    }
                }
                newKingSquare->SetPiece(&*mSelectedPiece);
                if (rook)
                {
                    newRookSquare->SetPiece(rook);
                    rook->SetPosition(wxPoint(newRookSquare->GetCenter().x-(squareSize/2), newRookSquare->GetCenter().y-(squareSize/2)));
                }
                mSelectedPiece->SetPosition(wxPoint(newKingSquare->GetCenter().x-(squareSize/2), newKingSquare->GetCenter().y-(squareSize/2)));
                oldKingSquare->SetPiece(nullptr);
                oldRookSquare->SetPiece(nullptr);
                mBoard->UpdateBoard(L"OOO");
                std::cout << "Update Board Called" << std::endl;
                mBoard->SetWhiteTurn(!whiteTurn);
                mBoard->GeneratePossibleMoves(false);
                GetPicture()->UpdateObservers();
            }
        }
        else if (std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end())
        {
            if (newSquare->GetPiece())
            {
                newSquare->GetPiece()->SetPosition(wxPoint(0,0));
            }
            if (mSelectedPiece->GetSquare())
            {
                mSelectedPiece->GetSquare()->SetPiece(nullptr);
            }
            newSquare->SetPiece(&*mSelectedPiece);
            mSelectedPiece->SetPosition(wxPoint(newSquare->GetCenter().x-(squareSize/2), newSquare->GetCenter().y-(squareSize/2)));
            int pieceNum = std::stoi(mSelectedPiece->GetName());
            if (pieceNum == 17)
            {
                mBoard->SetBlackKingSquare({move[2], move[3]});
            }
            if (pieceNum == 9)
            {
                mBoard->SetWhiteKingSquare({move[2], move[3]});
            }
            mBoard->UpdateBoard(move);
            std::cout << "Update Board Called" << std::endl;
            mBoard->SetWhiteTurn(!whiteTurn);
            mBoard->GeneratePossibleMoves(false);
            GetPicture()->UpdateObservers();
        }
        else
        {
            std::cout << "Move not viable" << std::endl;
            wxPoint oldPos = mSelectedPiece->GetSquare()->GetPosition();
            mSelectedPiece->SetPosition(wxPoint(oldPos.x-(squareSize/2), oldPos.y-(squareSize/2)));
            GetPicture()->UpdateObservers();
        }
        for (auto file : mBoard->GetBoard())
        {
            for (auto rank : file)
            {
                std::cout << rank << " | ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    OnMouseMove(event);
}

/**
* Handle the mouse move event
* @param event
*/
void ViewEdit::OnMouseMove(wxMouseEvent &event)
{
    auto click = CalcUnscrolledPosition(event.GetPosition());

    wxPoint newMouse(click.x, click.y);
    wxPoint delta = newMouse - mLastMouse;
    mLastMouse = newMouse;

    if (event.LeftIsDown())
    {
        switch (mMode)
        {
        case Mode::Move:
            if (mSelectedDrawable != nullptr)
            {
                if (mSelectedDrawable->IsMovable())
                {
                    mSelectedDrawable->SetPosition(mSelectedDrawable->GetPosition() + delta);
                }
                else
                {
                    if (mSelectedItem)
                    {
                        mSelectedItem->SetItemPosition(mSelectedItem->GetItemPosition() + delta);
                    }
                }
                GetPicture()->UpdateObservers();
            }
            break;

        default:
            break;
        }

    }
    else
    {
        if (mSelectedDrawable != nullptr)
        {
            mSelectedDrawable = nullptr;
            mSelectedItem = nullptr;
        }
    }
}

/**
 * Handle the left mouse button double-click event
 * @param event
 */
void ViewEdit::OnLeftDoubleClick(wxMouseEvent &event)
{
}