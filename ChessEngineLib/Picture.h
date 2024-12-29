/**
 * @file Picture.h
 *
 * @author Charles B. Owen
 *
 *  Class that represents our animation picture
 */

#pragma once

class PictureObserver;
class Item;
class Board;

/**
 *  Class that represents our animation picture
 */
class Picture
{
private:
    /// The picture size
    wxSize mSize = wxSize(1500, 800);

    /// The observers of this picture
    std::vector<PictureObserver *> mObservers;

    /// The Items associated with this picture
    std::vector<std::shared_ptr<Item>> mItems;

    /// The board inside the picture
    std::shared_ptr<Board> mBoard = nullptr;

public:
    Picture();

    /// Copy Constructor (Disabled)
    Picture(const Picture &) = delete;
    /// Assignment Operator (Disabled)
    void operator=(const Picture &) = delete;

    /**
     * Destructor
    */
    virtual ~Picture() = default;

    /**
     * Get the picture size
     * @return Picture size in pixels
     */
    wxSize GetSize() {return mSize;}

    /**
     * Set the picture size
     * @param size Picture size in pixels
     */
    void SetSize(wxSize size) {mSize = size;}

    void AddObserver(PictureObserver *observer);
    void SetBoard(std::shared_ptr<Board> board) { mBoard = board; };
    std::shared_ptr<Board> GetBoard() { return mBoard; };
    void RemoveObserver(PictureObserver *observer);
    void UpdateObservers();
    void Draw(std::shared_ptr<wxGraphicsContext> graphics);

    void AddItem(std::shared_ptr<Item> Item);

    /** Iterator that iterates over the Items in a picture */
    class ItemIter
    {
    public:
        /**
         * Constructor
         * @param picture Picture we are iterating
         * @param pos Starting position \
         */
        ItemIter(Picture *picture, int pos) : mPicture(picture), mPos(pos) {}

        /**
         * Test for end of the iterator
         * @param other Other object to test against
         * @return True if we this position equals not equal to the other position
         */
        bool operator!=(const ItemIter &other) const
        {
            return mPos != other.mPos;
        }

        /**
         * Get value at current position
         * @return Value at mPos in the collection
         */
        std::shared_ptr<Item> operator *() const { return mPicture->mItems[mPos]; }

        /** Increment the iterator
        * @return Reference to this iterator */
        const ItemIter& operator++()
        {
            mPos++;
            return *this;
        }


    private:
        Picture *mPicture;  ///< Picture we are iterating over
        int mPos;           ///< Position in the collection
    };

    /**
     * Get an iterator for the beginning of the collection
     * @return Iter object at position 0
     */
    ItemIter begin() { return ItemIter(this, 0); }

    /**
     * Get an iterator for the end of the collection
     * @return Iter object at position past the end
     */
    ItemIter end() { return ItemIter(this, mItems.size()); }

    void SetAnimationTime(double time);

    double GetAnimationTime();

    void Load(const wxString& filename);
    /// Updates picture time
    void UpdateTime();

    void Save(const wxString& filename);
};
