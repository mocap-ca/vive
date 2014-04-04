using UnityEngine;
using System.Collections;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections.Generic;

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

	Socket clientSocket;
	string buffer;
	string infoMessage1 = "";
	string infoMessage2 = "";

	const string BODY_OBJECT       = "BetaVicon";
	const string OCULUS_OBJECT     = "Oculus";
	const string LEFT_HAND_OBJECT  = "LeftHandRigid";
	const string RIGHT_HAND_OBJECT = "RightHandRigid";
	const string LEFT_FOOT_OBJECT  = "LeftFootRigid";
	const string RIGHT_FOOT_OBJECT = "RightFootRigid";


	public string hostIp = "127.0.0.1";
	public int port = 1234;
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
		toggleDisplay = false;

		infoMessage1 = "INIT1";
		infoMessage1 = "INIT2";

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



		data = new Byte[1024 * 16];

		Connect();

	}

	void Connect()
	{
		Debug.Log ("Connect");

		//if (clientSocket != null && clientSocket.Connected) Disconnect();

		//var ipAddress = Dns.GetHostAddresses ("localhost");
		//IPEndPoint ipEndPoint = new IPEndPoint(ipAddress[0], 1234);
		IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Parse(hostIp), port);
		
		// Create Socket
		AddressFamily family  = AddressFamily.InterNetwork;
		SocketType    sokType = SocketType.Stream;
		ProtocolType  proType = ProtocolType.Tcp;
		clientSocket = new Socket(family, sokType, proType);

		//GameObject mainCamera = GameObject.Find("Oculus");
		//LoadLevelTest tester = null;
		//if(mainCamera) tester = mainCamera.GetComponent<LoadLevelTest>();


		try
		{
			Debug.Log("Connecting to localhost");
			clientSocket.Connect (ipEndPoint);
			
			// arriving here means the operation completed asyncConnect.IsCompleted = true)
			// but not necessarily successfully
			if (clientSocket.Connected == false)
			{
				Debug.Log(".client is not connected.");
				//if(tester) tester.SetMessage( "Not connected" );
				return;
			}
			
			Debug.Log(".client is connected.");
			
			clientSocket.Blocking = false;
			MouseNavigator.hasControl = false;
		}
		catch( SocketException e)
		{
			Debug.Log ("Socket Exception: " + e.ToString ());
			//if(tester) tester.SetMessage( "Not connected" );
		}


	}

	void Disconnect()
	{
		if (!clientSocket.Connected) return;
		clientSocket.Disconnect (false);
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
			if(e.keyCode == KeyCode.Alpha1) captureOriOculus = true;
			if(e.keyCode == KeyCode.B) captureOriBody = true;
			if(e.keyCode == KeyCode.H) captureOriHands = true;
			if(e.keyCode == KeyCode.F) captureOriFeet = true;
			if(e.keyCode == KeyCode.C) Connect();
			if(e.keyCode == KeyCode.D && clientSocket.Connected) Disconnect();
		}

		if (clientSocket == null || !clientSocket.Connected)
			GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), "Not Connected");
		
		if(toggleDisplay)
		{
			GUI.Label(	new Rect(5, 5, 	Screen.width, Screen.height), infoMessage1 + infoMessage2);
			Debug.Log (infoMessage1);
			Debug.Log (infoMessage2);
		}
		else
			GUI.Label(	new Rect(5, 5, 	Screen.width, Screen.height), "ACTIVE");


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


	bool GetOnePacket(out string outData)
	{
		outData = "";

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
			infoMessage1 += "Socket Error: " + e.ToString();
		}

		return false;
	}

	bool GetSocketData(out Dictionary< string, SegmentItem[] > subjectList)
	{
		subjectList = new Dictionary< string, SegmentItem[] > ();
		string packet = "";
		if (!GetOnePacket (out packet)) return false;

		infoMessage1 = "DATA\n";
			
		string[] lineItems = packet.Split ('\n');
		if(lineItems.Length == 0)
		{
			infoMessage1 = "No data error";
			return false;
		}
			
		int subjects = 0;

		if(!int.TryParse ( lineItems[0], out subjects))
		{
			infoMessage1 = "Invalid data while parsing subjects";
			return false;
		}

		int line = 1;

		for(int i=0; i < subjects && line < lineItems.Length; i++)
		{
			string[] subjectSplit = lineItems[line++].Split ('\t');
			string subjectName = subjectSplit[0];
			int    noSegments  = Convert.ToInt32 (subjectSplit[1]);
			int    noMarkers   = Convert.ToInt32 (subjectSplit[2]);
			infoMessage1 += subjectName + "  " + noSegments + " segments   " + noMarkers + " markers\n";

			SegmentItem[] items = new SegmentItem[noSegments + noMarkers];

			// Segments	
			int item_i=0;
			for(int j=0; j < noSegments && line < lineItems.Length; j++)
			{
				string[] segmentSplit = lineItems[line++].Split('\t');
					
				if(segmentSplit.Length != 8)
				{
					infoMessage1 += "Segment Error: " + segmentSplit.Length;
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
					infoMessage1 += "Marker Error: " + segmentSplit.Length;
					continue;
				}
				float[] tr = new float[3];
				for(int k=0; k < 3; k++) tr[k] = float.Parse (segmentSplit[k+1]);
				items[item_i++] = new SegmentItem(segmentSplit[0], tr, zero, false);
			}
			subjectList.Add (subjectName, items);
			infoMessage1 += "Adding " + items.Length + " items";

		}

		return true;
	}


	
		
		
	// Update is called once per frame
	void FixedUpdate()
	{
		infoMessage2 = "";
		if (clientSocket == null)// || clientSocket.Available == 0)
		{
			infoMessage1 = "No Socket";
			return;
		}

		// Frame rate calculator
		TimeClick ();

		Dictionary< string, SegmentItem[] > subjectList;

		if(!GetSocketData(out subjectList)) { infoMessage2 = "no data"; return; }

		infoMessage2 = "Subjects: " + subjectList.Keys.Count + "\n";

		// For each subject
		foreach(KeyValuePair<String, SegmentItem[]> entry in subjectList)
		{
			string subject = entry.Key;

			infoMessage2 += "LOADING: " + subject + "\n";

			bool frameDone = false;

			// For each segment in subject
			foreach(SegmentItem item in entry.Value)
			{
				if(item == null)
				{
					infoMessage2 += "!skipping null\n";
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
								infoMessage2 += "RIGID MISSING:" + subject + "\n";
								continue;
							}
							infoMessage2 += "RIGID: " + subject + "\n";
							o = objectDict[subject];
						}
					}
					else
					{
						// Body Joint
						if ( !objectDict.ContainsKey( item.name) )
						{
							infoMessage2 += "BODY MISSING:" + item.name + "\n";
							continue;
						}
						infoMessage2 += "BODY: " + item.name + "\n";
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
					if(isHands || isFeet)
					{
						o.SetActive (true);
					}

					// Set Translation and rotation
					o.transform.localPosition = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);
					o.transform.localRotation = localOrientation * rotationOffsets[o];
					frameDone = true;
				}
				else
				{
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
					}
					frameDone = true;
				}
			}

			if(frameDone) frameCount++;
		}

		infoMessage2 += "FPS:" + fps.ToString ("0.00");


		captureOriOculus = false;
		captureOriHands = false;
		captureOriFeet = false;
		captureOriBody = false;
	}

}
