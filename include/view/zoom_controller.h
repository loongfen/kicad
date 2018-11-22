/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013-2018 KiCad Developers, see AUTHORS.txt for contributors.
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
 *
 */

/**
 * @file zoom_control.h
 * @brief ZOOM_CONTROLLER class definition.
 */

#ifndef __ZOOM_CONTROLLER_H
#define __ZOOM_CONTROLLER_H

#include <memory>

namespace KIGFX
{

/**
 * Class that handles the response of the zoom scale to external inputs
 */
class ZOOM_CONTROLLER
{
public:
    virtual ~ZOOM_CONTROLLER() = default;

    /**
     * Gets the scale factor produced by a given mousewheel rotation
     * @param  aRotation rotation of the mouse wheel (this comes from
     *                    wxMouseEvent::GetWheelRotation())
     * @return            the scale factor to scroll by
     */
    virtual double GetScaleForRotation( int aRotation ) = 0;
};

class ACCELERATING_ZOOM_CONTROLLER : public ZOOM_CONTROLLER
{
public:
    ACCELERATING_ZOOM_CONTROLLER( unsigned aAccTimeout );

    double GetScaleForRotation( int aRotation ) override;

private:
    double getTimeStamp() const;

    double   m_lastTimeStamp;
    unsigned m_accTimeout;
};

class CONSTANT_ZOOM_CONTROLLER : public ZOOM_CONTROLLER
{
public:
    CONSTANT_ZOOM_CONTROLLER( double aScale );

    double GetScaleForRotation( int aRotation ) override;

private:
    double m_scale;
};

} // namespace KIGFX

#endif
