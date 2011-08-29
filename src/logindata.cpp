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

#include "logindata.h"

LoginData::LoginData()
: m_url("")
, m_userName("")
, m_passwordLength(0)
{}

LoginData::LoginData(QString url, QString userName, int passwordLength)
: m_url(url)
, m_userName(userName)
, m_passwordLength(passwordLength)
{}

void LoginData::setUrl(QString url)
{
    m_url = url;
}

QString LoginData::url() const
{
    return m_url;
}

void LoginData::setUserName(QString userName)
{
    m_userName = userName;
}

QString LoginData::userName() const
{
    return m_userName;
}

void LoginData::setPasswordLength(int passwordLength)
{
    m_passwordLength = passwordLength;
}

int LoginData::passwordLength() const
{
    return m_passwordLength;
}
