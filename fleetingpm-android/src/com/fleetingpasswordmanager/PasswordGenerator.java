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

package com.fleetingpasswordmanager;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import android.util.Base64;

public final class PasswordGenerator {
	
	public static String generate(String master, String url, String user, int length) {
		
		byte[] combined = null;
		
		try {
			combined = (master + url + user).getBytes("ISO-8859-1");
			String base64 = Base64.encodeToString(PasswordGenerator.md5(new String(combined)).getBytes(), Base64.DEFAULT); 
			return postProcess(base64).replace("\n", "").substring(0, length);
		} catch (UnsupportedEncodingException e) {
	        e.printStackTrace();
		}

		return "";
	}

	private static final String postProcess(final String s) {
		return s.replace('+', 'A').replace('/', 'B').replace('=', 'C');
	}
	
	private static final String md5(final String s) {
	    try {
	        // Create MD5 Hash
	        MessageDigest digest = java.security.MessageDigest
	                .getInstance("MD5");
	        digest.update(s.getBytes());
	        byte messageDigest[] = digest.digest();
	 
	        // Create Hex String
	        StringBuffer hexString = new StringBuffer();
	        for (int i = 0; i < messageDigest.length; i++) {
	            String h = Integer.toHexString(0xFF & messageDigest[i]);
	            while (h.length() < 2)
	                h = "0" + h;
	            hexString.append(h);
	        }
	        
	        return hexString.toString();
	 
	    } catch (NoSuchAlgorithmException e) {
	        e.printStackTrace();
	    }

	    return "";
	}
}
