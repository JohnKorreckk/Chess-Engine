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
        mBoard->GeneratePossibleMoves();
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
        std::cout << "POSSIBLE MOVES: " << std::endl;
        for (auto const & move : possibleMoves)
        {
            std::cout << move << std::endl;
        }
        // std::cout << move << std::endl;
        if (std::find(possibleMoves.begin(), possibleMoves.end(), move) != possibleMoves.end())
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
            mBoard->SetWhiteTurn(!whiteTurn);
            mBoard->UpdateBoard(move);
            mBoard->GeneratePossibleMoves();
            GetPicture()->UpdateObservers();
        }
        else
        {
            wxPoint oldPos = mSelectedPiece->GetSquare()->GetPosition();
            mSelectedPiece->SetPosition(wxPoint(oldPos.x-(squareSize/2), oldPos.y-(squareSize/2)));
            GetPicture()->UpdateObservers();
        }
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