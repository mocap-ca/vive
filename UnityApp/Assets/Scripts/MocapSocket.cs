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

using System.Runtime.InteropServices;

class SegmentItem
{
	public SegmentItem(string _name, float[] _tr, float[] _ro, bool _isJoint)
	{
		name = _name;
		isJoint = _isJoint;
		tr = new float[3];
		tr = _tr;
		ro = new float[4];
		ro = _ro;
	}

	public string name;
	public float[] tr;
	public float[] ro;
	public bool isJoint;

};

public class MocapSocket : MonoBehaviour {

	[DllImport("UnityDllTest",  EntryPoint = "getData", CallingConvention = CallingConvention.Cdecl) ]
	private static extern int getData(byte[] data, int len);

	[DllImport("UnityDllTest") ]
	private static extern int test();

	[DllImport("UnityDllTest") ]
	private static extern bool connect();

	[DllImport("UnityDllTest") ]
	private static extern bool disconnect();

	[DllImport("UnityDllTest") ]
	private static extern bool isConnected();

	[DllImport("UnityDllTest") ]
	private static extern int peek();
	
	private Socket clientSocket;
	private string buffer;
	private string infoMessage = "";
	private bool   isActive = false;

	const string BODY_OBJECT       = "BetaVicon";
	const string OCULUS_OBJECT     = "Oculus";
	const string LEFT_HAND_OBJECT  = "LeftHandRigid";
	const string RIGHT_HAND_OBJECT = "RightHandRigid";
	const string LEFT_FOOT_OBJECT  = "LeftFootRigid";
	const string RIGHT_FOOT_OBJECT = "RightFootRigid";
	const string TEST_OBJECT       = "TEST";


	public string      hostIp        = "127.0.0.1";
	public int         port          = 4001;
	public bool        useLocal      = false;  // connect using a named pipe ( via dll )
	public bool        lockNavigator = true;  // Set to true to disable the camera when connected
	public float       markerSize    = 0.3f;  // Size of markers when displayed
	private Boolean    toggleDisplay = false; // Show debug text when true
	private Boolean    toggleHelp    = false; // Show help scren
	private Boolean    showMarkers   = false; // Show markers when true
	private GameObject markerGroup;
	public string      prefix        = "";

	List<GameObject> skeletonObjects = new List<GameObject>();
	List<GameObject> rigidObjects = new List<GameObject>();
	Dictionary< string, GameObject >     objectDict         = new Dictionary<string, GameObject> ();
	Dictionary< GameObject, Quaternion > rotationOffsets    = new Dictionary<GameObject, Quaternion> ();
	Dictionary< GameObject, Quaternion > initialOrientation = new Dictionary<GameObject, Quaternion> ();
	Dictionary< string, GameObject >     markerDict         = new Dictionary< string, GameObject >();

	int frameCount    = 0;
	double dt         = 0.0;
	double fps        = 0.0;
	double updateRate = 4.0;  // 4 updates per sec.

	bool captureOriOculus = false;
	bool captureOriBody   = false;
	bool captureOriHands  = false;
	bool captureOriFeet   = false;


	void OnGUI()
	{

		int wid1 = 70;
		int wid2 = 120;
		int wid3 = 120;
		int x1 = 5;
		int x2 = x1 + wid1 + 5;
		int x3 = x2 + wid2 + 5;
		int y = 0;
		int rowh = 23;

		// Display Things

		GUI.skin.GetStyle ("Label").alignment = TextAnchor.UpperLeft;

		if(Time.fixedTime < 2)
		{
			GUIStyle st = new GUIStyle(GUI.skin.GetStyle("Label"));
			st.alignment = TextAnchor.MiddleCenter;
			int w = 200;
			GUI.Label (new Rect (Screen.width/2 - w/2, Screen.height * 0.382f, w, 40), "Press P for help", st);
		}


		if(toggleDisplay)
		{
			if (isActive) 
			{
				String infoFps = "Connected " + fps.ToString ("0.00") + " fps\n";
				GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), infoFps);
			}
			else
			{
				GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), "Not Connected");
			}

			y+=10;

			// Rows
			GUI.Label(	            new Rect(x1, y, wid1, rowh), "Host:");
			GUI.SetNextControlName("IPField");
			hostIp = GUI.TextField (new Rect(x2, y, wid2, rowh), hostIp, rowh);
			useLocal = GUI.Toggle ( new Rect(x3	, y,  wid3, rowh), useLocal, "Local");	

			y+=25;

			GUI.Label(	            new Rect(x1, y, wid1, rowh), "Port:");
			GUI.SetNextControlName("PortField");
			port = int.Parse (GUI.TextField (new Rect(x2, y, wid2, rowh), port.ToString(), rowh));
			if(isActive)
			{
				if(GUI.Button ( new Rect(x3	, y,  wid3, rowh), "Disconnect")) DisconnectStream();
			}
			else
			{
				if( GUI.Button ( new Rect(x3	, y,  wid3, rowh), "Connect")) ConnectStream	();
			}

			y+=25;

			GUI.Label(	            new Rect(x1, y, wid1, rowh), "Prefix:");
			GUI.SetNextControlName("PrefixField");
			prefix = GUI.TextField (new Rect(x2, y, wid2, rowh), prefix, rowh);
			y+=25;

			GUI.Label(	new Rect(x1, y, Screen.width-10, Screen.height-y-5), infoMessage);

			MouseNavigator.topCrop = y;
		}

		if(toggleHelp)
		{
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


		// Don't check keys if a field is activated

		if (GUI.GetNameOfFocusedControl ().Equals ("PrefixField")) return;
		if (GUI.GetNameOfFocusedControl ().Equals ("IPField")) return;
		if (GUI.GetNameOfFocusedControl ().Equals ("PortField")) return;


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
				if(e.keyCode == KeyCode.D && clientSocket.Connected) DisconnectStream();
			}

			if(e.keyCode == KeyCode.M)
			{
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
				}
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
			}
		}
	}
	
	void SetInitialState(GameObject o)
	{
		objectDict[o.name.ToLower ()] = o;
		rotationOffsets.Add (o, Quaternion.identity);
		initialOrientation.Add (o, o.transform.localRotation);
	}
		
	// Use this for initialization
	void Start ()
	{


		toggleDisplay = false;
		infoMessage   = "INIT";

		string[] skeletonList =  new string[] {"Hips",  
			"LeftUpLeg", "LeftLeg", "LeftFoot", "LeftToeBase", 
			"RightUpLeg", "RightLeg", "RightFoot", "RightToeBase", 
			"Spine", "Spine1", "Spine2", "Spine3",
			"Neck", "Neck1", "Head",     
			"LeftShoulder", "LeftArm", "LeftForeArm", "LeftHand", 
			"RightShoulder", "RightArm", "RightForeArm", "RightHand",
			BODY_OBJECT
		};

		GameObject[] mocapTagged = GameObject.FindGameObjectsWithTag ("Mocap");

		if(mocapTagged != null)
		{
			foreach (GameObject o in mocapTagged)
			{
				Debug.Log ("Adding Rigid Object: " + o.name);
				rigidObjects.Add (o);
				SetInitialState(o);
				if(o.name.ToLower() != OCULUS_OBJECT.ToLower()) o.SetActive (false);
			}
		}

		foreach(string s in skeletonList) 
		{
			GameObject o = GameObject.Find(s);
			if (o == null) continue;
			skeletonObjects.Add (o);
			SetInitialState(o);

			//if(s == BODY_OBJECT) o.SetActive (false);
		}

		markerGroup = new GameObject ("MOCAP_MARKERS");

		ConnectStream();
	}

	void ConnectStream()
	{
		if (useLocal)
		{
			Debug.Log ("Connect Local");
			if( isConnected())
			{
				Debug.Log ("Local Connection Exists");
				return;
			}
			if( !connect())
			{
				Debug.Log ("Connection Failed");
				return;
			}
		}
		else
		{			
			Debug.Log("Connecting to network socket");
			IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Parse(hostIp), port);
			AddressFamily family  = AddressFamily.InterNetwork;
			SocketType    sokType = SocketType.Stream;
			ProtocolType  proType = ProtocolType.Tcp;
			clientSocket = new Socket(family, sokType, proType);

			try
			{
				clientSocket.Connect (ipEndPoint);
				if (clientSocket.Connected == false) return;
				clientSocket.Blocking = false;
			}
			catch( SocketException e)
			{
				//Debug.Log ("Socket Exception: " + e.ToString ());
				return;
			}
		}

		if(lockNavigator) MouseNavigator.hasControl = false;
		isActive = true;
	}

	void DisconnectStream()
	{
		isActive = false;
		if(useLocal)
		{
			disconnect();
		}
		else
		{
			if (!clientSocket.Connected) return;
			clientSocket.Disconnect (false);
		}
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


	// Get data from a local pipe (via a dll)
	bool GetLocalData(out string outData)
	{
		outData = "";
 
		int avail = peek ();
		if (avail == 0) return false;
		infoMessage += "Bytes avail: " + avail + "\n";
		byte[] socketData = new byte[1024*16];
		int ret = getData (socketData, 1024*16);

		if(ret == 1) return false;  // No data
		if(ret == 100)
		{	 
			infoMessage += "Pipe Error reading local data\n";
			Debug.Log ("Pipe Error");
			return false;
		}
		if(ret == 101)
		{
			infoMessage += "Read Error getting local data\n";
			infoMessage += Encoding.ASCII.GetString(socketData) + "\n";
			Debug.Log ("Read Error: " + Encoding.ASCII.GetString(socketData));
			return false;
		}
		outData = Encoding.ASCII.GetString(socketData);

		return true;
	}

	// Get data from a TCP socket
	bool GetSocketData(out string outData)
	{
		outData = "";

		if (clientSocket == null || !clientSocket.Connected)
		{
			infoMessage += "Not Connected while trying to get packet\n";
			return false;
		}

		try
		{
			// Get some data, append to current buffer
			byte[] socketData = new byte[1024*16];
			int ret = clientSocket.Receive (socketData, 1024*16, 0);
			buffer += Encoding.ASCII.GetString(socketData, 0, ret);
			infoMessage += "Size: " + ret;
						
			// find split point
			int splitHere = buffer.LastIndexOf ("END\r\n");
			if(splitHere == -1) return false;
			
			// everything after last split point gets buffered
			string extracted = buffer.Substring(0, splitHere);
			if(buffer.Length == splitHere + 5)
				buffer = "";
			else
				buffer = buffer.Substring (splitHere+5);
			
			// split packets
			string[] splitsPackets = { "END\r\n" };
			string[] packetItems = extracted.Split(splitsPackets, StringSplitOptions.RemoveEmptyEntries);
			if(packetItems.Length == 0) return false;

			outData = packetItems[packetItems.Length -1];
			return true;
		}
		catch(SocketException e)
		{
			if(e.ErrorCode == 10035) return false;
			infoMessage += "Socket Error: " + e.ToString() + " (" + e.ErrorCode + ")\n";
		}

		return false;
	}

	// Get data, parse and return as a subject list
	bool GetData(out Dictionary< string, SegmentItem[] > subjectList)
	{
		subjectList = new Dictionary< string, SegmentItem[] > ();
		string packet = "";

		if (useLocal)
		{
			if (!GetLocalData (out packet)) return false;
		}
		else
		{
			if (!GetSocketData( out packet)) return false;
		}
			
		string[] lineItems = packet.Split ('\n');
		if(lineItems.Length == 0)
		{
			infoMessage += "No data error\n";
			return false;
		}
			
		int subjects = 0;

		if(!int.TryParse ( lineItems[0], out subjects))
		{
			infoMessage += "Invalid data while parsing subjects\n";
			return false;
		}

		int line = 1;

		for(int i=0; i < subjects && line < lineItems.Length; i++)
		{
			string[] subjectSplit = lineItems[line++].Split ('\t');
			string subjectName = subjectSplit[0].ToLower ();
			// Strip prefix
			if(prefix.Length > 0 && prefix.Length < subjectName.Length)
			{
				if(subjectName.Substring(0, prefix.Length) == prefix.ToLower ())
				{
					subjectName = subjectName.Substring (prefix.Length);
				}
			}
			int    noSegments  = Convert.ToInt32 (subjectSplit[1]);
			int    noMarkers   = Convert.ToInt32 (subjectSplit[2]);
			infoMessage += subjectName + "  " + noSegments + " segments   " + noMarkers + " markers\n";

			SegmentItem[] items = new SegmentItem[noSegments + noMarkers];

			// Segments	
			int item_i=0;
			for(int j=0; j < noSegments && line < lineItems.Length; j++)
			{
				string[] segmentSplit = lineItems[line++].Split('\t');
					
				if(segmentSplit.Length != 8)
				{
					infoMessage += "Segment Error: " + segmentSplit.Length;
					continue;
				}
				float[] tr = new float[3];
				float[] ro = new float[4];
				for(int k=0; k < 3; k++) tr[k] = float.Parse (segmentSplit[k+1]);
				for(int k=0; k < 4; k++) ro[k] = float.Parse (segmentSplit[k+4]);
				items[item_i++] = new SegmentItem(segmentSplit[0], tr, ro, true);
			}

			float[] zero = new float[4];
			for(int j=0; j < 4; j++) zero[j] = 0.0f;


			// Markers
			for(int j=0; j < noMarkers && line < lineItems.Length; j++)
			{
				string[] segmentSplit = lineItems[line++].Split('\t');
				
				if(segmentSplit.Length != 4)
				{
					Debug.Log ("Marker Error" + segmentSplit.Length);
					infoMessage += "Marker Error: " + segmentSplit.Length;
					continue;
				}
				float[] tr = new float[3];
				for(int k=0; k < 3; k++) tr[k] = float.Parse (segmentSplit[k+1]);
				items[item_i++] = new SegmentItem(segmentSplit[0], tr, zero, false);
			}
			if(!subjectList.ContainsKey(subjectName))
				subjectList.Add (subjectName, items);
		}

		return true;
	}
	
	// Update is called once per frame
	void FixedUpdate()
	{

		if (!isActive)
		{
			return;
		}
		if (!useLocal && clientSocket == null)// || clientSocket.Available == 0)
		{
			infoMessage = "NO CONNECTION";
			return;
		}

		Dictionary< string, SegmentItem[] > subjectList;

		if(!GetData(out subjectList)) { return; }

		// Frame rate calculator
		TimeClick ();

		infoMessage = "Subjects: " + subjectList.Keys.Count + "\n";

		// For each subject
		foreach(KeyValuePair<String, SegmentItem[]> entry in subjectList)
		{
			string subject = entry.Key.ToLower();

			infoMessage += "SUBJECT: " + subject + "\n";

			bool frameDone = false;

			// For each segment in subject
			foreach(SegmentItem item in entry.Value)
			{
				if(item == null)
				{
					infoMessage += "Error: skipping null item\n";
					continue;
				}

				infoMessage += item.name;

				if(item.isJoint)
				{
					infoMessage += " -  joint";

					// JOINT
					//Quaternion localOrientation  =  new Quaternion(item.ro[0], item.ro[2], -item.ro[1], item.ro[3]);
					Quaternion localOrientation  =  new Quaternion(item.ro[0], item.ro[1], item.ro[2], item.ro[3]);
					GameObject o = null;

					bool isHands  = (subject.ToLower() == LEFT_HAND_OBJECT.ToLower()  || subject.ToLower() == RIGHT_HAND_OBJECT.ToLower());
					bool isTest   = (subject.ToLower() == TEST_OBJECT.ToLower() );
					bool isFeet   = (subject.ToLower() == RIGHT_FOOT_OBJECT.ToLower() || subject.ToLower() == LEFT_FOOT_OBJECT.ToLower());
					bool isOculus = (subject.ToLower() == OCULUS_OBJECT.ToLower());

					if ( isHands || isFeet || isOculus || isTest )
					{
						infoMessage += " rigid";

						// Rigidbody joint
						if(item.name.ToLower () == "root" || item.name.ToLower () == subject.ToLower ())
						{
							// Rigid objects match on subject name, not segment
							if ( !objectDict.ContainsKey( subject.ToLower () ) )
							{
								infoMessage += "- Rigid Missing\n";
								continue;
							}
							infoMessage += "\n";
							o = objectDict[subject.ToLower()];
						}
					}
					else
					{
						infoMessage += " body";

						// Body Joint
						if ( !objectDict.ContainsKey( item.name.ToLower () ) )
						{
							infoMessage += " body missing\n";
							continue;
						}
						infoMessage += "\n";
						o = objectDict[item.name.ToLower ()];
					}


					if( o == null)
					{
						infoMessage += " - SKIPPING\n";
					}

					// Zero off the rotations
					bool doHands  = isHands && captureOriHands;
					bool doFeet   = isFeet && captureOriFeet;
					bool doBody   = (!isOculus && !isHands && !isFeet && captureOriBody);
					bool doOculus = isOculus && captureOriOculus;
					if (  doHands || doFeet || doBody || doOculus ) 
					{
						//PlayerPrefs.SetFloat(segmentName, inputOrientation.ToString());						
						rotationOffsets[o] =  Quaternion.AngleAxis(0, Vector3.right) * Quaternion.Inverse ( localOrientation );
					}

					// Set object visibility
					o.SetActive (true);

					// Set Translation and rotation
					//o.transform.localPosition = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);
					o.transform.localPosition = new Vector3(item.tr[0], item.tr[1], item.tr[2]);
					o.transform.localRotation = localOrientation * rotationOffsets[o];

					frameDone = true;
				}
				else
				{
					if(showMarkers)
					{
						// Marker

						//Vector3 pos = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);
						Vector3 pos = new Vector3(item.tr[0], item.tr[1], item.tr[2]);

						if(markerDict.ContainsKey(item.name))
						{
							markerDict[item.name].transform.localPosition = pos;
						}
						else
						{
							GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
							cube.transform.parent = markerGroup.transform;
							cube.name = "marker_" + item.name;
							cube.transform.localScale = new Vector3(markerSize, markerSize, markerSize);
							markerDict.Add (item.name, cube);
							cube.transform.localPosition = pos;
						}
						frameDone = true;
					}
				}
			}

			infoMessage += "\n";

			if(frameDone) frameCount++;
		}

		captureOriOculus = false;
		captureOriHands = false;
		captureOriFeet = false;
		captureOriBody = false;
	}


}
