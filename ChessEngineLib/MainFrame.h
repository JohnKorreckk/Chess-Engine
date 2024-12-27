/**
* @file MainFrame.h
 * @author John Korreck
 *
 * The top-level (main) frame of the application
 */
#ifndef _MAINFRAME_H_
#define _MAINFRAME_H_

class Picture;
class ViewEdit;

/**
 * The top-level (main) frame of the application
 */
class MainFrame : public wxFrame
{
private:
 /// View class for our edit window
 ViewEdit *mViewEdit;

 void OnExit(wxCommandEvent& event);
 void OnAbout(wxCommandEvent&);
 void OnClose(wxCloseEvent &event);

 /// The resources directory to use
 std::wstring mResourcesDir;

 /// The picture object we are viewing/editing
 std::shared_ptr<Picture> mPicture;

public:
 MainFrame(std::wstring resourcesDir);

 void Initialize();
};

#endif //_MAINFRAME_H_
