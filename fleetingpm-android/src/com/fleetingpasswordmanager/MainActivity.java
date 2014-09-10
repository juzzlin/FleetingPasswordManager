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

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import com.example.fleetingpasswordmanager.R;

public class MainActivity extends Activity {

	public final static String MASTER_PASSWORD = "com.fleetingpasswordmanager.MASTER_PASSWORD";

	private MasterPasswordWatcher masterPasswordWatcher;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		Spinner lengthSpinner = (Spinner)findViewById(R.id.length_spinner);
		
		Integer[] items = new Integer[] {8, 9, 10, 11, 12, 13, 14, 15, 16};
		ArrayAdapter<Integer[]> ad = new ArrayAdapter(this, android.R.layout.simple_spinner_item, items);
		lengthSpinner.setAdapter(ad);

		EditText masterPasswordEdit = (EditText)findViewById(R.id.master_password_edit);
		EditText passwordEdit = (EditText)findViewById(R.id.password_edit);
		masterPasswordWatcher = new MasterPasswordWatcher(masterPasswordEdit, passwordEdit);
		masterPasswordEdit.addTextChangedListener(masterPasswordWatcher);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	/** Called when the user clicks the show password button */
	public void showPassword(View view) {
		
	    EditText masterPasswordText = (EditText)findViewById(R.id.master_password_edit);
	    final String masterPassword = masterPasswordText.getText().toString();

	    EditText urlText = (EditText)findViewById(R.id.url_edit);
	    final String url = urlText.getText().toString();

	    EditText userNameText = (EditText)findViewById(R.id.user_name_edit);
	    final String userName = userNameText.getText().toString();
	    
	    EditText passwordText = (EditText)findViewById(R.id.password_edit);
	    
	    Spinner lengthSpinner = (Spinner)findViewById(R.id.length_spinner);
	    final Integer length = (Integer)lengthSpinner.getItemAtPosition(lengthSpinner.getSelectedItemPosition());
	    passwordText.setText(PasswordGenerator.generate(masterPassword, url, userName, length));
	}
}
