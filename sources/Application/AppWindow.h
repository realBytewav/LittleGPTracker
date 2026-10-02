
#ifndef _APP_WINDOW_H_
#define _APP_WINDOW_H_


// Upper bound on the character grid, and with it the size of the four screen
// buffers below. The default is the classic 40x30, so every existing platform
// keeps exactly the 1200-cell buffers it has always had. A platform whose
// panel can usefully show more opts in from its makefile, e.g.
//   -DAPPWINDOW_MAX_COLS=64 -DAPPWINDOW_MAX_ROWS=48
// and pays the extra static RAM only there. This matters: AppWindow is in
// COMMONFILES, so an unconditional bump would land on the NDS and GP32 too.
#ifndef APPWINDOW_MAX_COLS
#define APPWINDOW_MAX_COLS 40
#endif
#ifndef APPWINDOW_MAX_ROWS
#define APPWINDOW_MAX_ROWS 30
#endif
#define APPWINDOW_MAX_CELLS (APPWINDOW_MAX_COLS * APPWINDOW_MAX_ROWS)

#include "Application/Views/ChainView.h"
#include "Application/Views/ConsoleView.h"
#include "Application/Views/GrooveView.h"
#include "Application/Views/InstrumentView.h"
#include "Application/Views/MixerView.h"
#include "Application/Views/NullView.h"
#include "Application/Views/PhraseView.h"
#include "Application/Views/ProjectView.h"
#include "Application/Views/SongView.h"
#include "Application/Views/TableView.h"
#include "Application/Views/ViewData.h"
#include "Foundation/Observable.h"
#include "System/Process/SysMutex.h"
#include "System/io/Status.h"
#include "UIFramework/SimpleBaseClasses/GUIWindow.h"

#define PROP_INVERT 0x80

class AppWindow : public GUIWindow, I_Observer, Status {
  protected:
    AppWindow(I_GUIWindowImp &imp);
    virtual ~AppWindow();

  public:
    static AppWindow *Create(GUICreateWindowParams &);
    void LoadProject(const Path &path);
    void SaveLastProject(const Path &p);
    void CloseProject();

    virtual void Clear(bool all = false);
    virtual void ClearRect(GUIRect &rect);
    virtual void SetColor(ColorDefinition cd);
    void SetDirty();

  protected: // GUIWindow implementation
    virtual bool onEvent(GUIEvent &event);
    virtual void onUpdate();
    virtual void LayoutChildren();
    virtual void Flush();
    virtual void Redraw();

    // override draw string to avoid going too far off
    // the screen.
    virtual void DrawString(const char *string, GUIPoint &pos,
                            GUITextProperties &props, bool overlay = false);

    // I_Observer implementation

    virtual void Update(Observable &o, I_ObservableData *d);

    // Status implementation

    virtual void Print(char *);

    void defineColor(const char *colorName, GUIColor &color);

    void onQuitApp();

  private:
    View *_currentView;
    ViewData *_viewData;
    SongView *_songView;
    ChainView *_chainView;
    PhraseView *_phraseView;
    ProjectView *_projectView;
    InstrumentView *_instrumentView;
    TableView *_tableView;
    GrooveView *_grooveView;
    NullView *_nullView;
    MixerView *_mixerView;

    Path _root;

    bool _isDirty;
    bool _closeProject;
    bool _loadAfterSaveAsProject;
    bool _loadAfterResume;
    bool _shouldQuit;
    unsigned short _mask;
    unsigned long _lastA;
    unsigned long _lastB;
    char _statusLine[80];
    std::string _newProjectToLoad;
    // Character screen, diffed against _preScreen so Flush only repaints
    // cells that actually changed. Sized for the largest grid we allow;
    // cols_ * rows_ of it is live. Was a fixed 40x30 (1200).
    unsigned char _charScreen[APPWINDOW_MAX_CELLS];
    unsigned char _charScreenProp[APPWINDOW_MAX_CELLS];
    unsigned char _preScreen[APPWINDOW_MAX_CELLS];
    unsigned char _preScreenProp[APPWINDOW_MAX_CELLS];

    static GUIColor backgroundColor_;
    static GUIColor normalColor_;
    static GUIColor borderColor_;
    static GUIColor songviewfeColor_;
    static GUIColor songview00Color_;
    static GUIColor highlight2Color_;
    static GUIColor highlightColor_;
    static GUIColor consoleColor_;
    static GUIColor cursorColor_;
    static GUIColor playColor_;
    static GUIColor muteColor_;
    static GUIColor rownumberColor_;
    static GUIColor rownumber2Color_;
    static GUIColor majorbeatColor_;
    static GUIColor columnTitleColor_;
#define LAST_PROJECT_NAME "bin:last_project"

    ColorDefinition colorIndex_;

    // Pixel size of one character cell.
    static int charWidth_;
    static int charHeight_;
    // Size of the character grid, in cells, derived from the window rect.
    static int cols_;
    static int rows_;

    SysMutex drawMutex_;

    Path GetLastProjectPath();
};

#endif
