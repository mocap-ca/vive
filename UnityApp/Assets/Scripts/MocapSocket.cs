/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr University

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

using UnityEngine;
using System.Collections;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections.Generic;
using Vive;



public class MocapSocket : MonoBehaviour
{
	private bool   isActive = false;

	public string      hostIp        = "127.0.0.1";
	public int         port          = 4001;
	public bool        lockNavigator = true;  // Set to true to disable the camera when connected
	public float       markerSize    = 0.3f;  // Size of markers when displayed
	private Boolean    toggleDisplay = false; // Show debug text when true
	private Boolean    toggleHelp    = false; // Show help scren
	//private Boolean    showMarkers   = false; // Show markers when true
	private GameObject markerGroup;
	private GameObject[] mocapTagged;
	public string      prefix        = "";

	int frameCount    = 0;
	double dt         = 0.0;
	double fps        = 0.0;
	double updateRate = 4.0;  // 4 updates per sec.

	bool captureOriOculus = false;
	bool captureOriBody   = false;
	bool captureOriHands  = false;
	bool captureOriFeet   = false;

	private SubjectList subjectList;

	private SocketConnector connector;
	
	private SubjectCombo subjCombo;

	// Use this for initialization
	void Start ()
	{
		toggleDisplay = false;

		subjectList = new SubjectList();

		Debug.Break ();

		Rect r = new Rect (50, 100, 100, 20);
		subjCombo = new SubjectCombo (subjectList, r);

		Debug.Log ("Subject Combo: " + subjCombo);
		
		string[] skeletonList =  new string[] {"Hips",  
			"LeftUpLeg", "LeftLeg", "LeftFoot", "LeftToeBase", 
			"RightUpLeg", "RightLeg", "RightFoot", "RightToeBase", 
			"Spine", "Spine1", "Spine2", "Spine3",
			"Neck", "Neck1", "Head",     
			"LeftShoulder", "LeftArm", "LeftForeArm", "LeftHand", 
			"RightShoulder", "RightArm", "RightForeArm", "RightHand"
		};
		
		mocapTagged = GameObject.FindGameObjectsWithTag ("Mocap");
		
		markerGroup = new GameObject ("MOCAP_MARKERS");
		 
		ConnectStream ();
	}
	
	void OnGUI()
	{



		// Display Things

		GUI.skin.GetStyle ("Label").alignment = TextAnchor.UpperLeft;

		if(Time.fixedTime < 2)
		{
			// Show "Press P for help" for first 2 seconds
			GUIStyle st = new GUIStyle(GUI.skin.GetStyle("Label"));
			st.alignment = TextAnchor.MiddleCenter;
			int w = 200;
			GUI.Label (new Rect (Screen.width/2 - w/2, Screen.height * 0.382f, w, 40), "Press P for help", st);
		}

		if(toggleDisplay)
		{
			// Info display toggle (Y - key)
			String msg;

			int selectedItemIndex = subjCombo.Show();
			//GUI.Label( new Rect(50, 70, 400, 21), "You picked " + comboBoxList[selectedItemIndex].text + "!" ); 


			if (isActive) msg = "Connected " + fps.ToString ("0.00") + " fps";
			else          msg = "Not Connected";

			GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), msg);
			GUI.Label(	new Rect(5, 50, Screen.width-10, Screen.height-55), subjectList.Info ());

			MouseNavigator.topCrop = 55;
		}

		if(toggleHelp)
		{
			// Help toggle (? - key)
			String text = "VIVE - Very Imersive Virtual Experience\n";
			text += "Alastair Macleod, Emily Carr University, The Sawmill\n";
			text += "Version 0.1\n";
			text += "1 - Reset Oculus\n";
			text += "B - Zero Body\n";
			text += "H - Zero Hands\n";
			text += "F - Zero Feet\n";
			text += "M - Show Markers\n";
			text += "Y - Debug Data\n";
			text += "C - Connect\n";
			text += "D - Disconnect\n";
			GUI.Label ( new Rect(5, 5, 	Screen.width, Screen.height), text );
		}

		// Key events

		Event e = Event.current;
		
		if (e.type == EventType.KeyUp)
		{

			if(e.keyCode == KeyCode.X) Application.LoadLevelAsync ("Entrance");
			if(e.keyCode == KeyCode.C) ConnectStream();

			if(isActive)
			{
				if(e.keyCode == KeyCode.Alpha1) captureOriOculus  = true;
				if(e.keyCode == KeyCode.B)      captureOriBody    = true;
				if(e.keyCode == KeyCode.H)      captureOriHands   = true;
				if(e.keyCode == KeyCode.F)      captureOriFeet    = true;
				if(e.keyCode == KeyCode.D)      DisconnectStream();
			}

			if(e.keyCode == KeyCode.M)
			{
				/*
				if(showMarkers)
				{

					foreach(string sMarker in markerDict.Keys)
					{
						Destroy(markerDict[sMarker]);
					}
					markerDict.Clear ();
					showMarkers = false;
				}
				else
				{
					showMarkers = true;
				}*/
			}

			if(e.keyCode == KeyCode.P)
			{
				toggleDisplay = false;
				toggleHelp = !toggleHelp;
			}

			if(e.keyCode == KeyCode.Y)
			{
				toggleHelp = false;
				toggleDisplay = !toggleDisplay;
				if(toggleDisplay) subjCombo.Update ();
			}
		}
	}

		


	void ConnectStream()
	{
		//if (useLocal) connector = new VivePipeConnector ();	else

		connector = new SocketConnector(subjectList, hostIp, port);

		connector.ConnectData();

		if(lockNavigator) MouseNavigator.hasControl = false;
		isActive = true;
	}

	void DisconnectStream()
	{
		isActive = false;
		connector.DisconnectData ();
		if(lockNavigator) MouseNavigator.hasControl = true;

	}
	
	void TimeClick()
	{
		dt += Time.deltaTime;
		if (dt > 1.0/updateRate)
		{
			fps = frameCount / dt ;
			frameCount = 0;
			dt -= 1.0/updateRate;
		}
	}



	// Update is called once per frame
	void OnPreRender()
	{
		if (!isActive) return;
		if ( !connector.IsConnected() ) return;

		try	{ connector.GetData(); }
		catch( InfoException e) { Debug.Log ( e.GetInfo() ); }

		// Frame rate calculator
		TimeClick ();

		foreach( Vive.Subject s in subjectList.subjects ) s.UpdateScene();

		captureOriOculus = false;
		captureOriHands = false;
		captureOriFeet = false;
		captureOriBody = false;
	}


}
