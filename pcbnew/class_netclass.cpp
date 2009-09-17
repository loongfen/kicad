/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2009 SoftPLC Corporation, Dick Hollenbeck <dick@softplc.com>
 * Copyright (C) 2009 Jean-Pierre Charras, jean-pierre.charras@inpg.fr
 * Copyright (C) 2009 Kicad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */


#include "fctsys.h"

#include "common.h"
#include "kicad_string.h"
#include "pcbnew.h"


// This will get mapped to "kicad_default" in the specctra_export.
const wxString NETCLASS::Default = wxT("Default");


NETCLASS::NETCLASS( BOARD* aParent, const wxString& aName, const NETCLASS* initialParameters ) :
    m_Parent( aParent ),
    m_Name( aName )
{
    // use initialParameters if not NULL, else set the initial
    // parameters from g_DesignSettings
    SetParams( initialParameters );
}


void NETCLASS::SetParams( const NETCLASS* defaults )
{
    if( defaults )
    {
        SetClearance( defaults->GetClearance() );
        SetTrackWidth( defaults->GetTrackWidth() );
        SetViaDiameter( defaults->GetViaDiameter() );
        SetViaDrill( defaults->GetViaDrill() );
        SetuViaDiameter( defaults->GetuViaDiameter() );
        SetuViaDrill( defaults->GetuViaDrill() );
    }
    else
    {
        const EDA_BoardDesignSettings& g = g_DesignSettings;

        SetClearance(   g.m_TrackClearance );
        SetTrackWidth(  g.m_TrackMinWidth );
        SetViaDiameter( g.m_ViasMinSize );
        SetViaDrill(    g.m_ViaDrill );
        SetuViaDiameter(g.m_MicroViasMinSize );
        SetuViaDrill(   g.m_MicroViaDrill );
    }
}


NETCLASS::~NETCLASS()
{
}


NETCLASSES::NETCLASSES( BOARD* aParent ) :
    m_Parent( aParent ),
    m_Default( aParent, NETCLASS::Default )
{
}


NETCLASSES::~NETCLASSES()
{
    Clear();
}


void NETCLASSES::Clear()
{
    // Although std::map<> will destroy the items that it contains, in this
    // case we have NETCLASS* (pointers) and "destroying" a pointer does not
    // delete the object that the pointer points to.

    // this NETCLASSES is owner of its NETCLASS pointers,
    // so delete NETCLASSes pointed to by them.
    for( iterator i = begin();  i!=end();  )
    {
        // http://www.sgi.com/tech/stl/Map.html says:
        // "Erasing an element from a map also does not invalidate any iterators,
        // except, of course, for iterators that actually point to the element that
        // is being erased."

        iterator e = i++;       // copy, then advance.

        delete e->second;       // delete the NETCLASS, which 'second' points to.

        m_NetClasses.erase( e );
    }
}


bool NETCLASSES::Add( NETCLASS* aNetClass )
{
    const wxString& name = aNetClass->GetName();

    if( name == NETCLASS::Default )
    {
        // invoke operator=(), which is currently generated by compiler.
        m_Default = *aNetClass;

        delete aNetClass;   // we own aNetClass, must delete it since we copied it.

        return true;
    }

    // Test for an existing netclass:
    if( !Find( name ) )
    {
        // name not found, take ownership
        m_NetClasses[name] = aNetClass;
        return true;
    }
    else
    {
        // name already exists
        // do not "take ownership" and return false telling caller such.
        return false;
    }
}


NETCLASS* NETCLASSES::Remove( const wxString& aNetName )
{
    NETCLASSMAP::iterator found = m_NetClasses.find( aNetName );

    if( found != m_NetClasses.end() )
    {
        NETCLASS*   netclass = found->second;
        m_NetClasses.erase( found );
        return netclass;
    }

    return NULL;
}


NETCLASS* NETCLASSES::Find( const wxString& aName ) const
{
    if( aName == NETCLASS::Default )
        return (NETCLASS*) &m_Default;

    NETCLASSMAP::const_iterator found = m_NetClasses.find( aName );

    if( found == m_NetClasses.end() )
        return NULL;
    else
        return found->second;
}


void BOARD::SynchronizeNetsAndNetClasses()
{
    // D(printf("start\n");)       // simple performance/timing indicator.

    // set all NETs to the default NETCLASS, then later override some
    // as we go through the NETCLASSes.

    int count = m_NetInfo->GetCount();
    for( int i=0;  i<count;  ++i )
    {
        NETINFO_ITEM* net = FindNet( i );
        if( net )
            net->SetClass( m_NetClasses.GetDefault() );
    }

    // Add netclass name and pointer to nets.  If a net is in more than one netclass,
    // set the net's name and pointer to only the first netclass.  Subsequent
    // and therefore bogus netclass memberships will be deleted in logic below this loop.
    for( NETCLASSES::iterator clazz=m_NetClasses.begin();  clazz!=m_NetClasses.end();  ++clazz )
    {
        NETCLASS* netclass = clazz->second;

        for( NETCLASS::iterator member = netclass->begin();  member!=netclass->end();  ++member )
        {
            const wxString&  netname = *member;

            // although this overall function seems to be adequately fast,
            // FindNet( wxString ) uses now a fast binary search and is fast
            // event for large net lists
            NETINFO_ITEM* net = FindNet( netname );

            if( net && net->GetClassName() == NETCLASS::Default )
            {
                net->SetClass( netclass );
            }
        }
    }

    // Finally, make sure that every NET is in a NETCLASS, even if that
    // means the Default NETCLASS.  And make sure that all NETCLASSes do not
    // contain netnames that do not exist, by deleting all netnames from
    // every netclass and re-adding them.

    for( NETCLASSES::iterator clazz=m_NetClasses.begin();  clazz!=m_NetClasses.end();  ++clazz )
    {
        NETCLASS* netclass = clazz->second;

        netclass->Clear();
    }

    m_NetClasses.GetDefault()->Clear();

    for( int i=0;  i<count;  ++i )
    {
        NETINFO_ITEM* net = FindNet( i );
        if( net )
        {
            const wxString& classname = net->GetClassName();

            // because of the std:map<> this should be fast, and because of
            // prior logic, netclass should not be NULL.
            NETCLASS* netclass = m_NetClasses.Find( classname );

            wxASSERT( netclass );

            netclass->Add( net->GetNetname() );
        }
    }

    // D(printf("stop\n");)
}


bool NETCLASSES::Save( FILE* aFile ) const
{
    bool result;

    // save the default first.
    result = m_Default.Save( aFile );

    if( result )
    {
        // the rest will be alphabetical in the *.brd file.
        for( const_iterator i = begin();  i!=end();  ++i )
        {
            NETCLASS*   netclass = i->second;

            result = netclass->Save( aFile );
            if( !result )
                break;
        }
    }

    return result;
}


bool NETCLASS::Save( FILE* aFile ) const
{
    bool result = true;

    fprintf( aFile, "$" BRD_NETCLASS "\n" );
    fprintf( aFile, "Name \"%s\"\n", CONV_TO_UTF8( m_Name ) );
    fprintf( aFile, "Desc \"%s\"\n", CONV_TO_UTF8( GetDescription() ) );

    // Write parameters

    fprintf( aFile, "Clearance %d\n",       GetClearance() );
    fprintf( aFile, "TrackWidth %d\n",      GetTrackWidth() );

    fprintf( aFile, "ViaDia %d\n",          GetViaDiameter() );
    fprintf( aFile, "ViaDrill %d\n",        GetViaDrill() );

    fprintf( aFile, "uViaDia %d\n",         GetuViaDiameter() );
    fprintf( aFile, "uViaDrill %d\n",       GetuViaDrill() );

    // Write members:
    for( const_iterator i = begin();  i!=end();  ++i )
        fprintf( aFile, "AddNet \"%s\"\n", CONV_TO_UTF8( *i ) );

    fprintf( aFile, "$End" BRD_NETCLASS "\n" );

    return result;
}


#if defined(DEBUG)

void NETCLASS::Show( int nestLevel, std::ostream& os )
{
    // for now, make it look like XML:
    //NestedSpace( nestLevel, os )

    os << '<' << GetClass().Lower().mb_str() << ">\n";

    for( const_iterator i = begin();  i!=end();  ++i )
    {
        // NestedSpace( nestLevel+1, os ) << *i;
        os << *i;
    }

    // NestedSpace( nestLevel, os )
    os << "</" << GetClass().Lower().mb_str() << ">\n";
}

#endif



bool NETCLASS::ReadDescr( FILE* aFile, int* aLineNum )
{
    bool        result = false;
    char        Line[1024];
    char        Buffer[1024];
    wxString    netname;

    while( GetLine( aFile, Line, aLineNum, 1024 ) != NULL )
    {
        if( strnicmp( Line, "AddNet", 6 ) == 0 )
        {
            ReadDelimitedText( Buffer, Line + 6, sizeof(Buffer) );
            netname = CONV_FROM_UTF8( Buffer );
            Add( netname );
            continue;
        }

        if( strnicmp( Line, "$end" BRD_NETCLASS, sizeof( "$end" BRD_NETCLASS)-1) == 0 )
        {
            result = true;
            break;
        }

        if( strnicmp( Line, "Clearance", 9 ) == 0 )
        {
            SetClearance( atoi( Line + 9 ) );
            continue;
        }
        if( strnicmp( Line, "TrackWidth", 10 ) == 0 )
        {
            SetTrackWidth( atoi( Line + 10 ) );
            continue;
        }
        if( strnicmp( Line, "ViaDia", 6 ) == 0 )
        {
            SetViaDiameter( atoi( Line + 6 ) );
            continue;
        }
        if( strnicmp( Line, "ViaDrill", 8 ) == 0 )
        {
            SetViaDrill( atoi( Line + 8 ) );
            continue;
        }

        if( strnicmp( Line, "uViaDia", 7 ) == 0 )
        {
            SetuViaDiameter( atoi( Line + 7 ) );
            continue;
        }
        if( strnicmp( Line, "uViaDrill", 9 ) == 0 )
        {
            SetuViaDrill( atoi( Line + 9 ) );
            continue;
        }

        if( strnicmp( Line, "Name", 4 ) == 0 )
        {
            ReadDelimitedText( Buffer, Line + 4, sizeof(Buffer) );
            m_Name = CONV_FROM_UTF8( Buffer );
            continue;
        }
        if( strnicmp( Line, "Desc", 4 ) == 0 )
        {
            ReadDelimitedText( Buffer, Line + 4, sizeof(Buffer) );
            SetDescription( CONV_FROM_UTF8( Buffer ) );
            continue;
        }
    }

    return result;
}
