// This file is part of Fleeting Password Manager (Fleetingpm).
// Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
//
// Fleetingpm is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// Fleetingpm is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Fleetingpm. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef CONFIG_H
#define CONFIG_H

//! Miscellaneous config data.
class Config
{
public:

    //! Program name.
    static const char * NAME;

    //! Program version.
    static const char * VERSION;

    //! Company name used in QSettings.
    static const char * COMPANY;

    //! Software name used in QSettings.
    static const char * SOFTWARE;
};

#endif // CONFIG_H
