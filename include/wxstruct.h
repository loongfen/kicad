/****************/
/*  wxstruct.h  */
/****************/

#ifndef  WXSTRUCT_H
#define  WXSTRUCT_H


#include <vector>

#include <wx/socket.h>
#include "wx/log.h"
#include "wx/config.h"
#include <wx/wxhtml.h>
#include <wx/laywin.h>
#include <wx/aui/aui.h>

#include "colors.h"
#include "common.h"

//C++ guarantees that operator delete checks its argument for null-ness
#ifndef SAFE_DELETE
#define SAFE_DELETE( p ) delete (p); (p) = NULL;
#endif

#ifndef EESCHEMA_INTERNAL_UNIT
#define EESCHEMA_INTERNAL_UNIT 1000
#endif

//  Option for dialog boxes
#define DIALOG_STYLE wxDEFAULT_DIALOG_STYLE | wxFRAME_FLOAT_ON_PARENT | \
    MAYBE_RESIZE_BORDER

#define KICAD_DEFAULT_DRAWFRAME_STYLE wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS

class EDA_ITEM;
class EDA_Rect;
class EDA_DRAW_PANEL;
class WinEDA_MsgPanel;
class BASE_SCREEN;
class WinEDA_Toolbar;
class WinEDAChoiceBox;
class PARAM_CFG_BASE;
class Ki_PageDescr;
class Ki_HotkeyInfo;
class PLOTTER;

enum id_librarytype {
    LIBRARY_TYPE_EESCHEMA,
    LIBRARY_TYPE_PCBNEW,
    LIBRARY_TYPE_DOC,
    LIBRARY_TYPE_SYMBOL
};

enum id_drawframe {
    NOT_INIT_FRAME = 0,
    SCHEMATIC_FRAME,
    LIBEDITOR_FRAME,
    VIEWER_FRAME,
    PCB_FRAME,
    MODULE_EDITOR_FRAME,
    CVPCB_FRAME,
    CVPCB_DISPLAY_FRAME,
    GERBER_FRAME,
    TEXT_EDITOR_FRAME,
    DISPLAY3D_FRAME,
    KICAD_MAIN_FRAME
};

enum id_toolbar {
    TOOLBAR_MAIN = 1,       // Main horizontal Toolbar
    TOOLBAR_TOOL,           // Right vertical Toolbar (list of tools)
    TOOLBAR_OPTION,         // Left vertical Toolbar (option toolbar
    TOOLBAR_AUX             // Secondary horizontal Toolbar
};


/***********************************************/
/* Classes for basic main frames used in kicad */
/***********************************************/


/******************************************************************/
/* Basic frame for kicad, eeschema, pcbnew and gerbview.          */
/* not directly used: the real frames are derived from this class */
/******************************************************************/

class EDA_BASE_FRAME : public wxFrame
{
public:
    int             m_Ident;        // Id Type (pcb, schematic, library..)
    wxPoint         m_FramePos;
    wxSize          m_FrameSize;
    int             m_MsgFrameHeight;

    WinEDA_Toolbar* m_HToolBar;     // Standard horizontal Toolbar
    bool            m_FrameIsActive;
    wxString        m_FrameName;    // name used for writing and reading setup
                                    // It is "SchematicFrame", "PcbFrame" ....
    wxString        m_AboutTitle;   // Name of program displayed in About.

    wxAuiManager   m_auimgr;

public:
    EDA_BASE_FRAME( wxWindow* father, int idtype,
                    const wxString& title,
                    const wxPoint& pos, const wxSize& size,
                    long style = KICAD_DEFAULT_DRAWFRAME_STYLE );
    ~EDA_BASE_FRAME();

    void         GetKicadHelp( wxCommandEvent& event );
    void         GetKicadAbout( wxCommandEvent& event );

    /**
     * Copy the version information to the clipboard for bug reporting purposes.
     */
    void         CopyVersionInfoToClipboard( wxCommandEvent& event );
    void         PrintMsg( const wxString& text );

    /**
     * Append the copy version information to clipboard help menu entry to \a aMenu.
     *
     * @param aMenu - The menu to append.
     */
    void         AddHelpVersionInfoMenuEntry( wxMenu* aMenu );

    virtual void LoadSettings();
    virtual void SaveSettings();

    // Read/Save and Import/export hotkeys config

    /**
     * Function ReadHotkeyConfig
     * Read configuration data and fill the current hotkey list with hotkeys
     * @param aDescList = current hotkey list descr. to initialise.
     */
    int          ReadHotkeyConfig( struct Ki_HotkeyInfoSectionDescriptor* aDescList );

    /**
     * Function WriteHotkeyConfig
     * Store the current hotkey list
     * It is stored using the standard wxConfig mechanism or a file.
     *
     * @param aDescList = pointer to the current hotkey list.
     * @param aFullFileName = a wxString pointer to a fuill file name.
     *  if NULL, use the standard wxConfig mechanism (default)
     * the output format is: shortcut  "key"  "function"
     * lines starting with # are comments
     */
    int          WriteHotkeyConfig( struct Ki_HotkeyInfoSectionDescriptor* aDescList,
                                    wxString * aFullFileName = NULL);

    /**
     * Function ReadHotkeyConfigFile
     * Read an old configuration file (&ltfile&gt.key) and fill the current hotkey list
     * with hotkeys
     * @param aFilename = file name to read.
     * @param aDescList = current hotkey list descr. to initialise.
     */
    int          ReadHotkeyConfigFile( const wxString& aFilename,
                                       struct Ki_HotkeyInfoSectionDescriptor* aDescList );

    /**
     * Function ImportHotkeyConfigFromFile
     * Prompt the user for an old hotkey file to read, and read it.
     * @param aDescList = current hotkey list descr. to initialise.
     */
    void         ImportHotkeyConfigFromFile( struct Ki_HotkeyInfoSectionDescriptor* aDescList );

    /**
     * Function ExportHotkeyConfigToFile
     * Prompt the user for an old hotkey file to read, and read it.
     * @param aDescList = current hotkey list descr. to initialise.
     */
    void         ExportHotkeyConfigToFile( struct Ki_HotkeyInfoSectionDescriptor* aDescList );

    /**
     * Function SetLanguage
     * called on a language menu selection
     * when using a derived function, do not forget to call this one
     */
    virtual void SetLanguage( wxCommandEvent& event );

    wxString     GetFileFromHistory( int cmdId, const wxString& type );
    void         SetLastProject( const wxString& FullFileName );
    void         DisplayActivity( int PerCent, const wxString& Text );
    virtual void ReCreateMenuBar();
};


/*******************************************************/
/* Basic draw frame for eeschema, pcbnew and gerbview. */
/*******************************************************/

class EDA_DRAW_FRAME : public EDA_BASE_FRAME
{
public:
    EDA_DRAW_PANEL*   DrawPanel;            // Draw area
    WinEDA_MsgPanel*  MsgPanel;             // Panel used to display some
                                            //  info (bottom of the screen)
    WinEDA_Toolbar*   m_VToolBar;           // Vertical (right side) Toolbar
    WinEDA_Toolbar*   m_AuxVToolBar;        // Auxiliary Vertical (right side)
                                            // Toolbar
    WinEDA_Toolbar*   m_OptionsToolBar;     // Options Toolbar (left side)
    WinEDA_Toolbar*   m_AuxiliaryToolBar;   // Auxiliary Toolbar used in pcbnew

    WinEDAChoiceBox*  m_SelGridBox;         // Choice box to choose the grid
                                            // size
    WinEDAChoiceBox*  m_SelZoomBox;         // Choice box to choose the zoom
                                            // value

    int          m_CursorShape;             // shape for cursor (0 = default
                                            // cursor)
    int          m_ID_current_state;        // Id of active button on the
                                            // vertical toolbar
    int          m_ID_last_state;           // Id of previous active button
                                            // on the vertical toolbar
    int          m_HTOOL_current_state;     // Id of active button on
                                            // horizontal toolbar

    int          m_InternalUnits;           // Internal units count in 1 inch
                                            // = 1000 for eeschema, = 10000
                                            // for PCBnew and Gerbview

    bool         m_Draw_Axis;               // TRUE to show X and Y axis
    bool         m_Draw_Grid_Axis;     /* TRUE to show grid axis. */
    bool         m_Draw_Sheet_Ref;          // TRUE to show frame references

    bool         m_Print_Sheet_Ref;         // TRUE to print frame references
    bool         m_Draw_Auxiliary_Axis;     /* TRUE to show auxiliary axis.
                                             * Used in pcbnew: the auxiliary
                                             * axis is the origin of
                                             * coordinates for drill, gerber
                                             * and component position files
                                             */
    wxPoint      m_Auxiliary_Axis_Position; /* position of the auxiliary axis */

protected:
    Ki_HotkeyInfoSectionDescriptor * m_HotkeysZoomAndGridList;
    int          m_LastGridSizeId;
    bool         m_DrawGrid;                // hide/Show grid
    int          m_GridColor;               // Grid color

private:
    BASE_SCREEN* m_currentScreen;           ///< current used SCREEN
    bool         m_snapToGrid;              ///< Indicates if cursor should be snapped to grid.

protected:
    void SetScreen( BASE_SCREEN* aScreen )
    {
        m_currentScreen = aScreen;
    }

public:
    EDA_DRAW_FRAME( wxWindow* father, int idtype,
                    const wxString& title,
                    const wxPoint& pos, const wxSize& size,
                    long style = KICAD_DEFAULT_DRAWFRAME_STYLE );

    ~EDA_DRAW_FRAME();

    virtual wxString GetScreenDesc();

    /**
     * Function GetBaseScreen
     * is virtual and returns a pointer to a BASE_SCREEN or one of its
     * derivatives.  It may be overloaded by derived classes.
     */
    virtual BASE_SCREEN* GetScreen() const { return m_currentScreen; }

    void             OnMenuOpen( wxMenuEvent& event );
    void             OnMouseEvent( wxMouseEvent& event );
    virtual void     OnHotKey( wxDC* aDC, int aHotKey, const wxPoint& aPosition,
                               EDA_ITEM* aItem = NULL );

    /**
     * Function AddMenuZoomAndGrid (virtual)
     * Add standard zoom commands and submenu zoom and grid selection to a popup menu
     * uses zoom hotkeys info base to add hotkeys info to menu commands
     * @param aMasterMenu = the menu to populate.
     */
    virtual void     AddMenuZoomAndGrid( wxMenu* aMasterMenu );

    void             EraseMsgBox();
    void             Process_PageSettings( wxCommandEvent& event );
    virtual void     SetToolbars();

    /**
     * Function SetLanguage
     * called on a language menu selection
     * when using a derived function, do not forget to call this one
     */
    virtual void     SetLanguage( wxCommandEvent& event );

    virtual void     ReCreateHToolbar() = 0;
    virtual void     ReCreateVToolbar() = 0;
    virtual void     ReCreateMenuBar();
    virtual void     ReCreateAuxiliaryToolbar();

    /**
     * Function SetToolID
     * Enables the icon of the selected tool in the vertical toolbar.
     * (Or tool ID_NO_SELECT_BUTT default if no new selection)
     * @param aId = new m_ID_current_state value (if aId >= 0)
     * @param aCursor = the new cursor shape
     * @param aToolMsg = tool message in status bar
     * if (aId >= 0)
     * Updates all variables related:
     *      m_ID_current_state, cursor shape and message in status bar
     * If (aId < 0)
     *      Only updates the cursor shape and message in status bar
     *      (does not the current m_ID_current_state value
     */
    virtual void     SetToolID( int aId, int aCursor, const wxString& aToolMsg );

    /* These 4 functions provide a basic way to show/hide grid
     * and /get/set grid color.
     * These parameters are saved in kicad config for each main frame
     */
    /**
     * Function IsGridVisible() , virtual
     * @return true if the grid must be shown
     */
    virtual bool     IsGridVisible()
    {
        return m_DrawGrid;
    }

    /**
     * Function SetGridVisibility() , virtual
     * It may be overloaded by derived classes
     * @param aVisible = true if the grid must be shown
     */
    virtual void     SetGridVisibility( bool aVisible )
    {
        m_DrawGrid = aVisible;
    }

    /**
     * Function GetGridColor() , virtual
     * @return the color of the grid
     */
    virtual int     GetGridColor()
    {
        return m_GridColor;
    }

    /**
     * Function SetGridColor() , virtual
     * @param aColor = the new color of the grid
     */
    virtual void     SetGridColor( int aColor )
    {
        m_GridColor = aColor;
    }

    /**
     * Function GetGridPosition
     * returns the nearest grid position to \a aPosition if a screen is defined and snap to
     * grid is enabled.  Otherwise, the origianl positions is returned.
     * @see m_snapToGrid and m_BaseScreen members.
     * @param aPosition The position to test.
     * @return The wxPoint of the appropriate cursor position.
     */
    wxPoint GetGridPosition( const wxPoint& aPosition );

    /**
     * Command event handler for selecting grid sizes.
     *
     * All commands that set the grid size should eventually end up here.
     * This is where the application setting is saved.  If you override
     * this method, make sure you call down to the base class.
     *
     * @param event - Command event passed by selecting grid size from the
     *                grid size combobox on the toolbar.
     */
    virtual void     OnSelectGrid( wxCommandEvent& event );
    virtual void     OnSelectZoom( wxCommandEvent& event );

    /**
     * Function GeneralControle
     * performs application specific control using \a aDC at \a aPosition in logical units.
     * <p>
     * Override this function for application specific control.  This function gets
     * called on every mouse and key event.
     *</p>
     * @param aDC A device context.
     * @param aPosition The current cursor position in logical (drawing) units.
     */
    virtual void     GeneralControle( wxDC* aDC, const wxPoint& aPosition ) { /* dummy */ }

    virtual void     OnSize( wxSizeEvent& event );
    void             OnEraseBackground( wxEraseEvent& SizeEvent );

    virtual void     OnZoom( wxCommandEvent& event );
    void             OnGrid( int grid_type );

    /**
     * Function RedrawScreen
     * redraws the entire screen area by updating the scroll bars and mouse pointer in
     * order to have \a aCenterPoint at the center of the screen.
     * @param aCenterPoint The position in logical units to center the scroll bars.
     * @param aWarpPointer Moves the mouse cursor to \a aCenterPoint if true.
     */
    void             RedrawScreen( const wxPoint& aCenterPoint, bool aWarpPointer );

    void             Zoom_Automatique( bool aWarpPointer );

    /* Set the zoom level to show the area Rect */
    void             Window_Zoom( EDA_Rect& Rect );

    /* Return the zoom level which displays the full page on screen */
    virtual int      BestZoom() = 0;

    /* Return the current zoom level */
    int              GetZoom( void );

    void             TraceWorkSheet( wxDC* DC, BASE_SCREEN* screen, int line_width );
    void             PlotWorkSheet( PLOTTER *plotter, BASE_SCREEN* screen );

    /**
     * Function GetXYSheetReferences
     * Return the X,Y sheet references where the point position is located
     * @param aScreen = screen to use
     * @param aPosition = position to identify by YX ref
     * @return a wxString containing the message locator like A3 or B6
     *         (or ?? if out of page limits)
     */
    wxString         GetXYSheetReferences( BASE_SCREEN* aScreen, const wxPoint& aPosition );

    void             DisplayToolMsg( const wxString& msg );
    virtual void     RedrawActiveWindow( wxDC* DC, bool EraseBg ) = 0;
    virtual void     OnLeftClick( wxDC* DC, const wxPoint& MousePos ) = 0;
    virtual void     OnLeftDClick( wxDC* DC, const wxPoint& MousePos );
    virtual bool     OnRightClick( const wxPoint& MousePos, wxMenu* PopMenu ) = 0;
    virtual void     ToolOnRightClick( wxCommandEvent& event );
    void             AdjustScrollBars( const wxPoint& aCenterPosition );

    /**
     * Function OnActivate (virtual)
     * is called when activating the frame.
     * In derived classes with a overriding OnActivate function,
     * do not forget to call this EDA_DRAW_FRAME::OnActivate( event ) basic function.
     */
    virtual void     OnActivate( wxActivateEvent& event );

    /**
     * Function UpdateStatusBar
     * updates the status bar information.
     *
     * The base method updates the absolute and relative coordinates and the
     * zoom information.  If you override this virtual method, make sure to call
     * this subclassed method.  The status bar can draw itself.  This is not
     * a drawing function per se, but rather updates lines of text held by
     * the components within the status bar which is owned by the wxFrame.
     * <p>
     * On a MAC, be careful about calling this function when there is an
     * existing wxDC in existence on a sibling window.
     */
    virtual void     UpdateStatusBar();

    void             DisplayUnitsMsg();

    /* Handlers for block commands */
    virtual void     InitBlockPasteInfos();
    virtual bool     HandleBlockBegin( wxDC* DC, int cmd_type,const wxPoint& startpos );

    /**
     * Function ReturnBlockCommand
     * Returns the block command internat code (BLOCK_MOVE, BLOCK_COPY...)
     * corresponding to the keys pressed (ALT, SHIFT, SHIFT ALT ..) when
     * block command is started by dragging the mouse.
     * @param aKey = the key modifiers (Alt, Shift ...)
     * @return the block command id (BLOCK_MOVE, BLOCK_COPY...)
     */
    virtual int  ReturnBlockCommand( int aKey );

    /**
     * Function HandleBlockPlace( )
     * Called after HandleBlockEnd, when a block command needs to be
     * executed after the block is moved to its new place
     * (bloc move, drag, copy .. )
     * Parameters must be initialized in GetScreen()->m_BlockLocate
     */
    virtual void HandleBlockPlace( wxDC* DC );

    /**
     * Function HandleBlockEnd( )
     * Handle the "end"  of a block command,
     * i.e. is called at the end of the definition of the area of a block.
     * depending on the current block command, this command is executed
     * or parameters are initialized to prepare a call to HandleBlockPlace
     * in GetScreen()->m_BlockLocate
     * @return false if no item selected, or command finished,
     * true if some items found and HandleBlockPlace must be called later
     */
    virtual bool HandleBlockEnd( wxDC* DC );

    void             CopyToClipboard( wxCommandEvent& event );

    /* interprocess communication */
    void             OnSockRequest( wxSocketEvent& evt );
    void             OnSockRequestServer( wxSocketEvent& evt );

    virtual void     LoadSettings();
    virtual void     SaveSettings();

    /**
     * Append a message to the message panel.
     *
     * This helper method checks to make sure the message panel exists in
     * the frame and appends a message to it using the message panel
     * AppendMessage() method.
     *
     * @param textUpper - The message upper text.
     * @param textLower - The message lower text.
     * @param color - A color ID from the Kicad color list (see colors.h).
     * @param pad - Number of spaces to pad between messages (default = 4).
     */
    void AppendMsgPanel( const wxString& textUpper, const wxString& textLower,
                         int color, int pad = 6 );

    /**
     * Clear all messages from the message panel.
     */
    void ClearMsgPanel( void );

    /** Virtual function PrintPage
     * used to print a page
     * Print the page pointed by current screen, set by the calling print function
     * @param aDC = wxDC given by the calling print function
     * @param aPrintMask = not used here
     * @param aPrintMirrorMode = not used here (Set when printing in mirror mode)
     * @param aData = a pointer on an auxiliary data (not always used, NULL if not used)
     */
    virtual void PrintPage( wxDC* aDC, int aPrintMask, bool aPrintMirrorMode, void* aData = NULL );

    DECLARE_EVENT_TABLE()
};


/*********************************************************
*   class WinEDA_MsgPanel : this is a panel to display various infos
*   and messages on items in eeschema an pcbnew
*********************************************************/

/**
 * Struct MsgItem
 * is used privately by WinEDA_MsgPanel as the item type of its vector.
 * These items are the pairs of text strings shown in the MsgPanel.
 */
struct MsgItem
{
    int      m_X;
    int      m_UpperY;
    int      m_LowerY;
    wxString m_UpperText;
    wxString m_LowerText;
    int      m_Color;

    /**
     * Function operator=
     * overload the assignment operator so that the wxStrings get copied
     * properly when copying a MsgItem.
     * No, actually I'm not sure this needed, C++ compiler may auto-generate it.
     */
    MsgItem& operator=( const MsgItem& rv )
    {
        m_X = rv.m_X;
        m_UpperY    = rv.m_UpperY;
        m_LowerY    = rv.m_LowerY;
        m_UpperText = rv.m_UpperText;   // overloaded operator=()
        m_LowerText = rv.m_LowerText;   // overloaded operator=()
        m_Color     = rv.m_Color;

        return * this;
    }
};


class WinEDA_MsgPanel : public wxPanel
{
protected:
    std::vector<MsgItem>    m_Items;
    int                     m_last_x;      ///< the last used x coordinate
    wxSize                  m_fontSize;


    void showItem( wxDC& dc, const MsgItem& aItem );

    void erase( wxDC* DC );

    /**
     * Function getFontSize
     * computes the height and width of a 'W' in the system font.
     */
    static wxSize computeFontSize();

    /**
     * Calculate the width and height of a text string using the system UI font.
     */
    wxSize computeTextSize( const wxString& text );

public:
    EDA_DRAW_FRAME* m_Parent;
    int m_BgColor;

public:
    WinEDA_MsgPanel( EDA_DRAW_FRAME* parent, int id, const wxPoint& pos, const wxSize& size );
    ~WinEDA_MsgPanel();


    /**
     * Function GetRequiredHeight
     * returns the required height (in pixels) of a WinEDA_MsgPanel.  This takes
     * into consideration the system gui font, wxSYS_DEFAULT_GUI_FONT.
     */
    static int GetRequiredHeight();

    void OnPaint( wxPaintEvent& event );
    void EraseMsgBox();
    void Affiche_1_Parametre( int pos_X, const wxString& texte_H,
                              const wxString& texte_L, int color );

    /**
     * Append a message to the message panel.
     *
     * This method automatically adjusts for the width of the text string.
     * Making consecutive calls to AppendMessage will append each message
     * to the right of the last message.  This message is not compatible
     * with Affiche_1_Parametre.
     *
     * @param textUpper - The message upper text.
     * @param textLower - The message lower text.
     * @param color - A color ID from the Kicad color list (see colors.h).
     * @param pad - Number of spaces to pad between messages (default = 4).
     */
    void AppendMessage( const wxString& textUpper, const wxString& textLower,
                        int color, int pad = 6 );

    DECLARE_EVENT_TABLE()
};

/*************************/
/* class WinEDA_Toolbar */
/*************************/

class WinEDA_Toolbar : public wxAuiToolBar
{
public:
    wxWindow*       m_Parent;
    id_toolbar      m_Ident;
    bool            m_Horizontal;       // some auxiliary TB are horizontal, others vertical

public:
    WinEDA_Toolbar( id_toolbar type, wxWindow* parent, wxWindowID id, bool horizontal );

    bool GetToolState( int toolId ) { return GetToolToggled(toolId); };

    void AddRadioTool( int             toolid,
                       const wxString& label,
                       const wxBitmap& bitmap,
                       const wxBitmap& bmpDisabled = wxNullBitmap,
                       const wxString& shortHelp = wxEmptyString,
                       const wxString& longHelp = wxEmptyString,
                       wxObject*       data = NULL )
    {
       AddTool( toolid, label, bitmap, bmpDisabled, wxITEM_CHECK,
                shortHelp, longHelp, data );
    };

    void SetToolNormalBitmap( int id, const wxBitmap& bitmap ) {};
    void SetRows( int nRows ) {};

    /**
     * Function GetDimension
     * @return the dimension of this toolbar (Height if horizontal, Width if vertical.
     */
    int GetDimension( );
};


#endif  /* WXSTRUCT_H */
