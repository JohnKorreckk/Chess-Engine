/**
 * @file MainFrame.cpp
 * @author John Korreck
 */

#include "pch.h"

#include <wx/xrc/xmlres.h>
#include <wx/stdpaths.h>

#include "MainFrame.h"
#include "ViewEdit.h"
#include "PictureFactory.h"

/// Directory within resources that contains the images.
const std::wstring ImagesDirectory = L"/images";


/**
 * Constructor
 * @param resourcesDir Directory path containing resources
 */
MainFrame::MainFrame(std::wstring resourcesDir) :
    mResourcesDir(resourcesDir)
{

}



/**
 * Initialize the MainFrame window.
 */
void MainFrame::Initialize()
{
    wxXmlResource::Get()->LoadFrame(this, nullptr, L"MainFrame");
#ifdef WIN32
    SetIcon(wxIcon(L"mainframe", wxBITMAP_TYPE_ICO_RESOURCE));
#endif

    auto sizer = new wxBoxSizer( wxVERTICAL );

    auto imagesDir = mResourcesDir + ImagesDirectory;

    mViewEdit = new ViewEdit(this, mResourcesDir);

    sizer->Add(mViewEdit,1, wxEXPAND | wxALL );

    SetSizer( sizer );
    Layout();

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);


    //
    // Create the picture
    //
    PictureFactory factory;
    std::cout << mResourcesDir << std::endl;
    mPicture = factory.Create(mResourcesDir);

    // Tell the views about the picture
    mViewEdit->SetPicture(mPicture);
}



/**
 * Exit menu option handlers
 * @param event
 */
void MainFrame::OnExit(wxCommandEvent& event)
{
  Close(true);
}

/**
 * Application about box menu handler
 * @param event The menu command event
 */
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxDialog aboutDlg;
    wxXmlResource::Get()->LoadDialog(&aboutDlg, this, L"AboutDialog");
    aboutDlg.ShowModal();
}


/**
 * Handle a close event. Stop the animation and destroy this window.
 * @param event The Close event
 */
void MainFrame::OnClose(wxCloseEvent& event)
{
    Destroy();
}