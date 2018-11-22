/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012 Torsten Hueter, torstenhtr <at> gmx.de
 * Copyright (C) 2013-2015 CERN
 * Copyright (C) 2012-2016 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * @author Maciej Suminski <maciej.suminski@cern.ch>
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

#include <view/zoom_controller.h>

// For timestamping events
#include <wx/time.h>

using namespace KIGFX;


ACCELERATING_ZOOM_CONTROLLER::ACCELERATING_ZOOM_CONTROLLER( unsigned aAccTimeout )
        : m_lastTimeStamp( getTimeStamp() ), m_accTimeout( aAccTimeout )
{
}


double ACCELERATING_ZOOM_CONTROLLER::GetScaleForRotation( int aRotation )
{
    // The minimal step value when changing the current zoom level
    const double zoomLevelScale = 1.2;

    const auto timeStamp = getTimeStamp();
    auto       timeDiff = timeStamp - m_lastTimeStamp;

    m_lastTimeStamp = timeStamp;

    double zoomScale;

    // Set scaling speed depending on scroll wheel event interval
    if( timeDiff < m_accTimeout && timeDiff > 0 )
    {
        zoomScale = 2.05 - timeDiff / m_accTimeout;

        // be sure zoomScale value is significant
        zoomScale = std::max( zoomScale, zoomLevelScale );

        if( aRotation < 0 )
            zoomScale = 1.0 / zoomScale;
    }
    else
    {
        zoomScale = ( aRotation > 0 ) ? zoomLevelScale : 1 / zoomLevelScale;
    }

    return zoomScale;
}


double ACCELERATING_ZOOM_CONTROLLER::getTimeStamp() const
{
    return wxGetLocalTimeMillis().ToDouble();
}


CONSTANT_ZOOM_CONTROLLER::CONSTANT_ZOOM_CONTROLLER( double aScale ) : m_scale( aScale )
{
}


double CONSTANT_ZOOM_CONTROLLER::GetScaleForRotation( int aRotation )
{
    aRotation = ( aRotation > 0 ) ? std::min( aRotation, 100 ) : std::max( aRotation, -100 );

    double dscale = aRotation * m_scale;

    double zoom_scale = ( aRotation > 0 ) ? ( 1 + dscale ) : 1 / ( 1 - dscale );

    return zoom_scale;
}