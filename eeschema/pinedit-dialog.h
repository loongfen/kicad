/////////////////////////////////////////////////////////////////////////////
// Name:        pinedit-dialog.h
// Purpose:     
// Author:      jean-pierre Charras
// Modified by: 
// Created:     11/02/2006 13:30:59
// RCS-ID:      
// Copyright:   License GNU
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 11/02/2006 13:30:59

#ifndef _PINEDIT_DIALOG_H_
#define _PINEDIT_DIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "pinedit-dialog.h"
#endif

/*!
 * Includes
 */
#include "fctsys.h"
#include "gr_basic.h"

#include "common.h"
#include "program.h"
#include "libcmp.h"
#include "general.h"
#include "id.h"

#include "protos.h"

////@begin includes
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10000
#define ID_TEXTCTRL 10001
#define ID_TEXTCTRL1 10002
#define ID_SPINCTRL 10003
#define ID_CHECKBOX 10004
#define ID_CHECKBOX1 10005
#define ID_CHECKBOX2 10006
#define ID_TEXTCTRL2 10007
#define ID_TEXTCTRL3 10008
#define ID_RADIOBOX 10009
#define ID_RADIOBOX1 10012
#define ID_RADIOBOX2 10013
#define SYMBOL_WINEDA_PINPROPERTIESFRAME_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|MAYBE_RESIZE_BORDER
#define SYMBOL_WINEDA_PINPROPERTIESFRAME_TITLE _("Pin properties")
#define SYMBOL_WINEDA_PINPROPERTIESFRAME_IDNAME ID_DIALOG
#define SYMBOL_WINEDA_PINPROPERTIESFRAME_SIZE wxSize(400, 300)
#define SYMBOL_WINEDA_PINPROPERTIESFRAME_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * WinEDA_PinPropertiesFrame class declaration
 */

class WinEDA_PinPropertiesFrame: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( WinEDA_PinPropertiesFrame )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    WinEDA_PinPropertiesFrame( );
    WinEDA_PinPropertiesFrame( WinEDA_LibeditFrame* parent, wxWindowID id = SYMBOL_WINEDA_PINPROPERTIESFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_PINPROPERTIESFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_PINPROPERTIESFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_PINPROPERTIESFRAME_SIZE, long style = SYMBOL_WINEDA_PINPROPERTIESFRAME_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WINEDA_PINPROPERTIESFRAME_IDNAME, const wxString& caption = SYMBOL_WINEDA_PINPROPERTIESFRAME_TITLE, const wxPoint& pos = SYMBOL_WINEDA_PINPROPERTIESFRAME_POSITION, const wxSize& size = SYMBOL_WINEDA_PINPROPERTIESFRAME_SIZE, long style = SYMBOL_WINEDA_PINPROPERTIESFRAME_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin WinEDA_PinPropertiesFrame event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end WinEDA_PinPropertiesFrame event handler declarations

////@begin WinEDA_PinPropertiesFrame member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end WinEDA_PinPropertiesFrame member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

	void PinPropertiesAccept(wxCommandEvent& event);
	void SetPinName(const wxString & newname, int newsize);
	void SetPinNum(const wxString & newnum, int newsize);
	void NewSizePin(int newsize);
	void SetPinShape( int newshape);
	void SetPinType(int newtype);
	void SetPinOrient(int neworient);
	void SetAttributsPin(bool draw, bool unit, bool convert);

////@begin WinEDA_PinPropertiesFrame member variables
    wxTextCtrl* m_PinNameCtrl;
    wxTextCtrl* m_PinNumCtrl;
    wxSpinCtrl* m_PinSize;
    wxCheckBox* m_CommonUnit;
    wxCheckBox* m_CommonConvert;
    wxCheckBox* m_NoDraw;
    wxStaticText* m_PinNameSizeText;
    wxTextCtrl* m_PinNameSizeCtrl;
    wxStaticText* m_PinNumSizeText;
    wxTextCtrl* m_PinNumSizeCtrl;
    wxRadioBox* m_PinOrient;
    wxButton* m_btClose;
    wxRadioBox* m_PinShape;
    wxRadioBox* m_PinElectricalType;
////@end WinEDA_PinPropertiesFrame member variables

	WinEDA_LibeditFrame * m_Parent;
};

#endif
    // _PINEDIT_DIALOG_H_
