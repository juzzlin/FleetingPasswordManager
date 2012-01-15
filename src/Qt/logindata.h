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

#ifndef LOGINDATA_H
#define LOGINDATA_H

#include <QString>

//! Login data that includes url, username, and password length.
//! Only this data can be saved.
class LoginData
{
public:

    //! Default constructor.
    LoginData();

    //! Constructor.
    LoginData(QString url, QString userName, int passwordLength);

    //! Set URL/ID.
    void setUrl(QString url);

    //! Get URL/ID.
    QString url() const;

    //! Set user name.
    void setUserName(QString userName);

    //! Get user name.
    QString userName() const;

    //! Set password length.
    void setPasswordLength(int passwordLength);

    //! Get password length.
    int passwordLength() const;

private:

    //! URL/ID
    QString m_url;

    //! User name
    QString m_userName;

    //! Password length
    int m_passwordLength;
};

#endif // LOGINDATA_H
