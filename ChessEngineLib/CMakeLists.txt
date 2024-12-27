project(ChessEngineLib)

set(SOURCE_FILES
        pch.h
        MainFrame.cpp MainFrame.h
        Picture.cpp Picture.h
        PictureFactory.cpp PictureFactory.h
        PictureObserver.cpp PictureObserver.h
        PolyDrawable.cpp PolyDrawable.h
        Drawable.cpp Drawable.h
        ImageDrawable.cpp ImageDrawable.h
        Item.cpp Item.h
        ViewEdit.cpp ViewEdit.h
        Board.cpp
        Board.h
        Piece.cpp
        Piece.h
        Square.cpp
        Square.h
        BoardFactory.cpp
        BoardFactory.h
)

find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)
include(${wxWidgets_USE_FILE})

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${MACHINE_LIBRARY})
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)