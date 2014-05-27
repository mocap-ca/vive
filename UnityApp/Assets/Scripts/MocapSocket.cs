/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Emily Carr University

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


	public string hostIp = "127.0.0.1";
	public int port = 4001;
	public bool useLocal = true;
	Boolean toggleDisplay;

	List<GameObject> skeletonObjects = new List<GameObject>();
	List<GameObject> rigidObjects = new List<GameObject>();
	Dictionary< string, GameObject >     objectDict         = new Dictionary<string, GameObject> ();
	Dictionary< GameObject, Quaternion > rotationOffsets    = new Dictionary<GameObject, Quaternion> ();
	Dictionary< GameObject, Quaternion > initialOrientation = new Dictionary<GameObject, Quaternion> ();
	Dictionary< string, GameObject >     markerDict         = new Dictionary< string, GameObject >();

	int frameCount = 0;
	double dt = 0.0;
	double fps = 0.0;
	double updateRate = 4.0;  // 4 updates per sec.

	bool captureOriOculus = false;
	bool captureOriBody = false;
	bool captureOriHands = false;
	bool captureOriFeet = false;
	
	void SetInitialState(GameObject o)
	{
		objectDict[o.name] = o;
		rotationOffsets.Add (o, Quaternion.identity);
		initialOrientation.Add (o, o.transform.localRotation);

	}
		
		
	// Use this for initialization
	void Start ()
	{

		int testValue = test ();
		Debug.Log ("Test stays: " + testValue);

		toggleDisplay = false;

		infoMessage = "INIT";

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
				if(o.name != OCULUS_OBJECT) o.SetActive (false);
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

		ConnectStream();
	}

	void ConnectStream()
	{
		if (useLocal)
		{
			Debug.Log ("Connect Local");
			if( !isConnected())
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
				
				if (clientSocket.Connected == false)
				{
					Debug.Log("Client is not connected.");
					return;
				}
				clientSocket.Blocking = false;
			}
			catch( SocketException e)
			{
				Debug.Log ("Socket Exception: " + e.ToString ());
				return;
			}
		}

		Debug.Log("Connected.");
		MouseNavigator.hasControl = false;
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
		MouseNavigator.hasControl = true;

	}

	public static Quaternion QuaternionFromMatrix(Matrix4x4 m) {
		// Adapted from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
		Quaternion q = new Quaternion();
		q.w = Mathf.Sqrt( Mathf.Max( 0, 1 + m[0,0] + m[1,1] + m[2,2] ) ) / 2; 
		q.x = Mathf.Sqrt( Mathf.Max( 0, 1 + m[0,0] - m[1,1] - m[2,2] ) ) / 2; 
		q.y = Mathf.Sqrt( Mathf.Max( 0, 1 - m[0,0] + m[1,1] - m[2,2] ) ) / 2; 
		q.z = Mathf.Sqrt( Mathf.Max( 0, 1 - m[0,0] - m[1,1] + m[2,2] ) ) / 2; 
		q.x *= Mathf.Sign( q.x * ( m[2,1] - m[1,2] ) );
		q.y *= Mathf.Sign( q.y * ( m[0,2] - m[2,0] ) );
		q.z *= Mathf.Sign( q.z * ( m[1,0] - m[0,1] ) );
		return q;
	}

	void OnGUI()
	{
		Event e = Event.current;

		if (e.type == EventType.KeyUp)
		{
			if(e.keyCode == KeyCode.Y) toggleDisplay = !toggleDisplay;
			if(e.keyCode == KeyCode.X) Application.LoadLevelAsync ("Entrance");
			if(e.keyCode == KeyCode.C) ConnectStream();
			if(isActive)
			{
				if(e.keyCode == KeyCode.Alpha1) captureOriOculus = true;
				if(e.keyCode == KeyCode.B) captureOriBody = true;
				if(e.keyCode == KeyCode.H) captureOriHands = true;
				if(e.keyCode == KeyCode.F) captureOriFeet = true;
				if(e.keyCode == KeyCode.D && clientSocket.Connected) DisconnectStream();
			}
		}

		if (isActive)
			GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), "Connected");
		else
			GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), "Not Connected");

		if(toggleDisplay)
		{
			String infoFps = "FPS:" + fps.ToString ("0.00") + "\n";
			GUI.Label(	new Rect(5, 5, 	Screen.width, Screen.height), infoFps + infoMessage);
		}
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
			string subjectName = subjectSplit[0];
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
			subjectList.Add (subjectName, items);
			infoMessage += "Adding " + items.Length + " items";


		}

		return true;
	}


	
		
		
	// Update is called once per frame
	void FixedUpdate()
	{
		infoMessage = "";
		if (!isActive)
		{
			return;
		}
		if (!useLocal && clientSocket == null)// || clientSocket.Available == 0)
		{
			infoMessage += "NO CONNECTION";
			return;
		}

		Dictionary< string, SegmentItem[] > subjectList;

		if(!GetData(out subjectList)) { return; }

		// Frame rate calculator
		TimeClick ();

		infoMessage += "Subjects: " + subjectList.Keys.Count + "\n";

		// For each subject
		foreach(KeyValuePair<String, SegmentItem[]> entry in subjectList)
		{
			string subject = entry.Key;

			infoMessage += "LOADING: " + subject + "\n";

			bool frameDone = false;

			// For each segment in subject
			foreach(SegmentItem item in entry.Value)
			{
				if(item == null)
				{
					infoMessage += "!skipping null\n";
					continue;
				}
				if(item.isJoint)
				{
					// JOINT
					Quaternion localOrientation  =  new Quaternion(item.ro[0], item.ro[2], -item.ro[1], item.ro[3]);
					GameObject o = null;

					bool isHands  = (subject == LEFT_HAND_OBJECT  || subject == RIGHT_HAND_OBJECT);
					bool isFeet   = (subject == RIGHT_FOOT_OBJECT || subject == LEFT_FOOT_OBJECT);
					bool isOculus = (subject == OCULUS_OBJECT);

					if ( isHands || isFeet || isOculus )
					{
						// Rigidbody joint
						if(item.name == "root")
						{
							// Rigid objects match on subject name, not segment
							if ( !objectDict.ContainsKey( subject ) )
							{
								infoMessage += "RIGID MISSING:" + subject + "\n";
								continue;
							}
							infoMessage += "RIGID: " + subject + "\n";
							o = objectDict[subject];
						}
					}
					else
					{
						// Body Joint
						if ( !objectDict.ContainsKey( item.name) )
						{
							infoMessage += "BODY MISSING:" + item.name + "\n";
							continue;
						}
						infoMessage += "BODY: " + item.name + "\n";
						o = objectDict[item.name];
					}

					if( o == null) continue;

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
					o.transform.localPosition = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);
					o.transform.localRotation = localOrientation * rotationOffsets[o];

					frameDone = true;
				}
				else
				{
					/*
					// Marker
					Vector3 pos = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);

					if(markerDict.ContainsKey(item.name))
					{
						markerDict[item.name].transform.localPosition = pos;
					}
					else
					{
						GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
						markerDict.Add (item.name, cube);
						cube.transform.localPosition = pos;
					}*/
					frameDone = true;
				}
			}

			if(frameDone) frameCount++;
		}




		captureOriOculus = false;
		captureOriHands = false;
		captureOriFeet = false;
		captureOriBody = false;
	}


}
