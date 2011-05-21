# This file is part of Fleeting Password Manager (FleetingPM).
# Copyright (C) 2011 Jussi Lind <jussi.lind@iki.fi>
#
# FleetingPM is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# FleetingPM is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FleetingPM. If not, see <http://www.gnu.org/licenses/>.

import hashlib, base64

class Engine(object):
    def generate(self, base = u'', url = u'', user = u'', n = 8):

        # Generate a base64 encoding of md5
        combined = (base + url + user).encode('ascii', 'ignore')
        hashed   = base64.encodestring(hashlib.md5(combined).hexdigest())

        # Replace possible +, / and = to A, B and C, respectively.
        hashed = hashed.replace('+', 'A').replace('/', 'B').replace('=', 'C')

        # Remove '\n' and return n first chars
        return hashed.strip()[:n]
