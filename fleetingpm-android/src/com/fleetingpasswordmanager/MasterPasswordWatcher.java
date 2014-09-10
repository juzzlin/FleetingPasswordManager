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

import android.os.Handler;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.EditText;

public class MasterPasswordWatcher implements TextWatcher {

	private EditText masterPasswordEdit;
	
	private EditText passwordEdit;
	
	private Handler handler = new Handler();
	
	public MasterPasswordWatcher(EditText masterPasswordEdit, EditText passwordEdit) {
		this.masterPasswordEdit = masterPasswordEdit;
		this.passwordEdit = passwordEdit;
	}
	
    public void onTextChanged(CharSequence s, int start, int before, int count) {

    	final TextWatcher watcher = this;

    	handler.removeCallbacksAndMessages(null);
    	
    	// Hack to run in UI thread. I miss you, Qt!
		handler.postDelayed(new Runnable() {
            public void run() {
				masterPasswordEdit.removeTextChangedListener(watcher);
				masterPasswordEdit.setText("");
				masterPasswordEdit.addTextChangedListener(watcher);
            }
        }, 1000 * 60);
		
		passwordEdit.setText("");
    }
    
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {
    }

    public void afterTextChanged(Editable s) {
    }
}
