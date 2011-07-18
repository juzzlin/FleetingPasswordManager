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

#include "engine.h"
#include <QCryptographicHash>

QString Engine::generate(const QString & master,
                         const QString & url,
                         const QString & user,
                         unsigned int length)
{
    // Generate a base64 encoding of md5
    QByteArray combined = (master + url + user).toLatin1();
    QByteArray hashed = QCryptographicHash::hash(combined, QCryptographicHash::Md5);
    hashed = hashed.toHex().toBase64();

    // Replace possible +, / and = with A, B and C, respectively.
    hashed = hashed.replace('+', 'A').replace('/', 'B').replace('=', 'C');

    // Remove '\n' and return n first chars
    return hashed.simplified().left(length);
}

