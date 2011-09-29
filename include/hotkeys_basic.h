/**
 * @file hotkeys_basic.h
 */

/* Some functions to handle hotkeys in kicad
 */

#ifndef  HOTKEYS_BASIC_H
#define  HOTKEYS_BASIC_H

#define DEFAULT_HOTKEY_FILENAME_EXT wxT( "key" )


class EDA_DRAW_FRAME;


/* Class to handle hotkey commands. hotkeys have a default value
 *  This class allows the real key code changed by user(from a key code list file)
 */
class EDA_HOTKEY
{
public:
    int      m_KeyCode;      // Key code (ascii value for ascii keys or wxWidgets code for function key
    wxString m_InfoMsg;      // info message.
    int      m_Idcommand;    // internal id for the corresponding command (see hotkey_id_commnand list)
    int      m_IdMenuEvent;  // id to call the corresponding event (if any) (see id.h)

public:
    EDA_HOTKEY( const wxChar* infomsg, int idcommand, int keycode, int idmenuevent = 0 );
    EDA_HOTKEY( const EDA_HOTKEY* base);
};


/**
 * Structure EDA_HOTKEY_CONFIG
 * contains the information required to save hot key information to a configuration file.
 * a Section name and the corresponding list of hotkeys (EDA_HOTKEY list)
 * hotkeys are grouped by section.
 * a section is a list of hotkey infos ( a EDA_HOTKEY list).
 * A full list of hotkeys can used one or many sections
 * for instance:
 *    the schematic editor uses a common section (zoom hotkeys list ..) and a specific section
 *    the library editor uses the same common section and a specific section
 * this feature avoid duplications and made hotkey file config easier to understand and edit
 */
struct EDA_HOTKEY_CONFIG
{
public:
    wxString*       m_SectionTag;     // The configuration file section name.
    EDA_HOTKEY**    m_HK_InfoList;    // List of EDA_HOTKEY pointers
    const wchar_t*  m_Comment;        // Will be printed in the config file only.
};

/* Identifiers (tags) in key code configuration file (or section names)
 *  .m_SectionTag member of a EDA_HOTKEY_CONFIG
 */
extern wxString g_CommonSectionTag;
extern wxString g_SchematicSectionTag;
extern wxString g_LibEditSectionTag;
extern wxString g_BoardEditorSectionTag;
extern wxString g_ModuleEditSectionTag;


/* Functions:
 */
void            AddHotkeyConfigMenu( wxMenu* menu );
void            HandleHotkeyConfigMenuSelection( EDA_DRAW_FRAME* frame, int id );

/**
 * Function ReturnKeyNameFromKeyCode
 * return the key name from the key code
 * Only some wxWidgets key values are handled for function key ( see
 * s_Hotkey_Name_List[] )
 * @param aKeycode = key code (ascii value, or wxWidgets value for function keys)
 * @param aIsFound = a pointer to a bool to return true if found, or false. an be NULL default)
 * @return the key name in a wxString
 */
wxString        ReturnKeyNameFromKeyCode( int aKeycode, bool * aIsFound = NULL );

/**
 * Function ReturnKeyNameFromCommandId
 * return the key name from the Command id value ( m_Idcommand member value)
 * @param aList = pointer to a EDA_HOTKEY list of commands
 * @param aCommandId = Command Id value
 * @return the key name in a wxString
 */
wxString        ReturnKeyNameFromCommandId( EDA_HOTKEY** aList, int aCommandId );

/**
 * Function ReturnKeyCodeFromKeyName
 * return the key code from its key name
 * Only some wxWidgets key values are handled for function key
 * @param keyname = wxString key name to find in s_Hotkey_Name_List[],
 *   like F2 or space or an usual (ascii) char.
 * @return the key code
 */
int ReturnKeyCodeFromKeyName( const wxString& keyname );

/* An helper enum for AddHotkeyName function
 * In menus we can an a hot key, or an accelerator , or sometimes just a comment
 * Hot keys can perform actions using the current mouse cursor position
 * Accelerators performs the same action as the associated menu
 * A comment is used in tool tips for some tools (zoom ..)
 *    to show the hot key that performs this action
 */
enum HOTKEY_ACTION_TYPE
{
    IS_HOTKEY,
    IS_ACCELERATOR,
    IS_COMMENT
};

/**
 * Function AddHotkeyName
 * Add the key name from the Command id value ( m_Idcommand member value)
 * @param aText = a wxString. returns aText + key name
 * @param aList = pointer to a EDA_HOTKEY list of commands
 * @param aCommandId = Command Id value
 * @param aShortCutType The #HOTKEY_ACTION_TYPE of the shortcut.
 * @return a wxString (aTest + key name) if key found or aText without modification
 */
wxString AddHotkeyName( const wxString& aText, EDA_HOTKEY** aList, int aCommandId,
                        HOTKEY_ACTION_TYPE aShortCutType = IS_HOTKEY);

/**
 * Function AddHotkeyName
 * Add the key name from the Command id value ( m_Idcommand member value)
 * @param aText = a wxString. returns aText + key name
 * @param aDescrList = pointer to a EDA_HOTKEY_CONFIG DescrList of commands
 * @param aCommandId = Command Id value
 * @param aShortCutType The #HOTKEY_ACTION_TYPE of the shortcut.
 * @return a wxString (aTest + key name) if key found or aText without modification
 */
wxString AddHotkeyName( const wxString&           aText,
                        struct EDA_HOTKEY_CONFIG* aDescrList,
                        int                       aCommandId,
                        HOTKEY_ACTION_TYPE        aShortCutType = IS_HOTKEY );

/**
 * Function DisplayHotkeyList
 * Displays the current hotkey list
 * @param aFrame = current active frame
 * @param aList = pointer to a EDA_HOTKEY_CONFIG list (Null terminated)
 */
void DisplayHotkeyList( EDA_DRAW_FRAME* aFrame, struct EDA_HOTKEY_CONFIG* aList );

/**
 * Function GetDescriptorFromHotkey
 * Return a EDA_HOTKEY * pointer from a key code for OnHotKey() function
 * @param aKey = key code (ascii value, or wxWidgets value for function keys
 * @param aList = pointer to a EDA_HOTKEY list of commands
 * @return the corresponding EDA_HOTKEY pointer from the EDA_HOTKEY List
 */
EDA_HOTKEY*  GetDescriptorFromHotkey( int aKey, EDA_HOTKEY** aList );

/**
 * Function ReadHotkeyConfig
 * Read hotkey configuration for a given app,
 * possibly before the frame for that app has been created
 * @param Appname = the value of the app's m_FrameName
 * @param aDescList = the hotkey data
*/
void ReadHotkeyConfig( const wxString& Appname, struct EDA_HOTKEY_CONFIG* aDescList );

void ParseHotkeyConfig( const wxString& data, struct EDA_HOTKEY_CONFIG* aDescList );


// common hotkeys event id
// these hotkey ID are used in many files, so they are define here only once.
enum common_hotkey_id_commnand {
    HK_NOT_FOUND = 0,
    HK_RESET_LOCAL_COORD,
    HK_HELP,
    HK_ZOOM_IN,
    HK_ZOOM_OUT,
    HK_ZOOM_REDRAW,
    HK_ZOOM_CENTER,
    HK_ZOOM_AUTO,
    HK_UNDO,
    HK_REDO,
    HK_COMMON_END
};

#endif // HOTKEYS_BASIC_H
