/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2018 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SYMBOL_PREVIEW_WIDGET_H
#define SYMBOL_PREVIEW_WIDGET_H

#include <wx/panel.h>
#include <kiway.h>
#include <gal/gal_display_options.h>


class LIB_ID;
class LIB_ALIAS;
class EDA_DRAW_PANEL_GAL;
class wxStaticText;
class wxSizer;


class SYMBOL_PREVIEW_WIDGET: public wxPanel
{
public:

    /**
     * Construct a symbol preview widget.
     *
     * @param aParent - parent window
     * @param aKiway - an active Kiway instance
     */
    SYMBOL_PREVIEW_WIDGET( wxWindow* aParent, KIWAY& aKiway );

    ~SYMBOL_PREVIEW_WIDGET() override;

    /**
     * Set the contents of the status label and display it.
     */
    void SetStatusText( wxString const& aText );

    /**
     * Set the currently displayed symbol.
     */
    void DisplaySymbol( const LIB_ID& aSymbolID, int aUnit );

private:
    KIWAY&                     m_kiway;

    KIGFX::GAL_DISPLAY_OPTIONS m_galDisplayOptions;
    EDA_DRAW_PANEL_GAL*        m_preview;

    wxStaticText*              m_status;
    wxSizer*                   m_sizer;

    LIB_ALIAS*                 m_previewItem;
};


#endif // SYMBOL_PREVIEW_WIDGET_H