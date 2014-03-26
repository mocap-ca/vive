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
	public string name;
	public float[] tr;
	public float[] so;
	public float[] lo;
	public float[] go;
};

public class MocapSocket : MonoBehaviour {

	Byte[] data;
	Socket clientSocket;
	string buffer;
	string infoMessage;

	const string BODY_OBJECT       = "BetaVicon";
	const string OCULUS_OBJECT     = "Oculus";
	const string LEFT_HAND_OBJECT  = "LeftHandRigid";
	const string RIGHT_HAND_OBJECT = "RightHandRigid";


	public string hostIp = "127.0.0.1";
	public int port = 1234;
	Boolean toggleDisplay;

	GameObject[] skeletonObjects;
	GameObject[] rigidObjects;
	Dictionary< string, GameObject >     objectDict         = new Dictionary<string, GameObject> ();
	Dictionary< GameObject, Quaternion > rotationOffsets    = new Dictionary<GameObject, Quaternion> ();
	Dictionary< GameObject, Quaternion > initialOrientation = new Dictionary<GameObject, Quaternion> ();

	int frameCount = 0;
	double dt = 0.0;
	double fps = 0.0;
	double updateRate = 4.0;  // 4 updates per sec.

	bool captureOri = false;

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

		string[] skeletonList =  new string[] {"Hips",  
			"LeftUpLeg", "LeftLeg", "LeftFoot", "LeftToeBase", 
			"RightUpLeg", "RightLeg", "RightFoot", "RightToeBase", 
			"Spine", "Spine1", "Spine2", "Spine3",
			"Neck", "Neck1", "Head",     
			"LeftShoulder", "LeftArm", "LeftForeArm", "LeftHand", 
			"RightShoulder", "RightArm", "RightForeArm", "RightHand",
			BODY_OBJECT
		};


		rigidObjects = GameObject.FindGameObjectsWithTag ("Mocap");


		foreach (GameObject o in rigidObjects) SetInitialState(o);

		int i = 0;
		foreach(string s in skeletonList) 
		{
			GameObject o = GameObject.Find(s);
			if (o == null) continue;
			skeletonObjects[i++] = o;
			SetInitialState(o);
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

		GameObject mainCamera = GameObject.Find("Oculus");
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
	 
	 
	void OnGUI()
	{
		if (clientSocket == null || !clientSocket.Connected)
			GUI.Label (new Rect (Screen.width - 150, 5, 150, 40), "Not Connected");



		if(toggleDisplay)
			GUI.Label(	new Rect(5, 5, 	Screen.width, Screen.height), infoMessage);

		//if(GUI.Button (new Rect(120,10,100,20), "disconnect"))
		//{
		//	currentState = 2;
		//}
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

	void Update()

	{
		if (Input.GetKeyDown ("z"))
				toggleDisplay = !toggleDisplay;

		if (Input.GetKeyDown ("1"))
						captureOri = true;

		if (Input.GetKeyDown ("x"))
			Application.LoadLevelAsync ("Entrance");

		if (Input.GetKeyDown ("b"))
		{
			objectDict[BODY_OBJECT].SetActive (! objectDict[BODY_OBJECT].activeInHierarchy );
		}

		if (Input.GetKeyDown ("h"))
		{
			objectDict[LEFT_HAND_OBJECT].SetActive (! objectDict[LEFT_HAND_OBJECT].activeInHierarchy);
			objectDict[RIGHT_HAND_OBJECT].SetActive (! objectDict[RIGHT_HAND_OBJECT].activeInHierarchy);
		}

		if (Input.GetKeyDown ("c") && clientSocket.Connected) Connect();

		if (Input.GetKeyDown ("d") && clientSocket.Connected) Disconnect();


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
			infoMessage += "Socket Error: " + e.ToString();
		}

		return false;
	}

	bool GetSocketData(out Dictionary< string, SegmentItem[] > subjectList)
	{
		subjectList = new Dictionary< string, SegmentItem[] > ();
		string packet = "";
		if (!GetOnePacket (out packet)) return false;
			
		infoMessage = "";
			
		string[] lineItems = packet.Split ('\n');
		if(lineItems.Length == 0)
		{
			infoMessage = "No data error";
			return false;
		}
			
		int subjects = 0;
		if(!int.TryParse ( lineItems[0], out subjects))
		{
			infoMessage = "Invalid data while parsing";
			return false;
		}

		int line = 1;

		for(int i=0; i < subjects && line < lineItems.Length; i++)
		{
			string[] subjectSplit = lineItems[line++].Split ('\t');
			string subjectName = subjectSplit[0];
			int    noSegments  = Convert.ToInt32 (subjectSplit[1]);
			infoMessage += "SUB: " + subjectName + "\n";

			SegmentItem[] items = new SegmentItem[noSegments];

				
			for(int j=0; j < noSegments && line < lineItems.Length; j++)
			{
				string[] segmentSplit = lineItems[line++].Split('\t');
					
				if(segmentSplit.Length != 16)
				{
					infoMessage += "Segment Error: " + segmentSplit.Length;
					continue;
				}
				SegmentItem item = new SegmentItem();
				item.name = segmentSplit[0];
				for(int k=0; k < 3; k++) item.tr[k] = float.Parse (segmentSplit[k+1]);
				for(int k=0; k < 4; k++) item.so[k] = float.Parse (segmentSplit[k+4]);
				for(int k=0; k < 4; k++) item.lo[k] = float.Parse (segmentSplit[k+8]);
				for(int k=0; k < 4; k++) item.go[k] = float.Parse (segmentSplit[k+12]);
				items[j] = item;
			}
			subjectList.Add (subjectName, items);
		}

		return true;
	}


	
		
		
	// Update is called once per frame
	void FixedUpdate ()
	{

		if (clientSocket == null || clientSocket.Available == 0) return;

		// Frame rate calculator
		TimeClick ();

		Dictionary< string, SegmentItem[] > subjectList;

		if(!GetSocketData(out subjectList)) return;

		/*if(subjectName != LEFT_HAND_OBJECT && subjectName != RIGHT_HAND_OBJECT && subjectName != OCULUS_OBJECT)
		{
			if ( !objects.ContainsKey( segmentName) ) continue;
		}*/

		foreach(KeyValuePair<String, SegmentItem[]> entry in subjectList)
		{
			string subject = entry.Key;

			bool frameDone = false;

			foreach(SegmentItem item in entry.Value)
			{

				//Quaternion inputOrientation =  Quaternion.AngleAxis(90, Vector3.left) * new Quaternion(rx, ry, rz, rw);
				Quaternion staticOrientation =  new Quaternion(item.so[0], item.so[1], item.so[2], item.so[3]);
				Quaternion localOrientation  =  new Quaternion(item.lo[0], item.lo[2], -item.lo[1], item.lo[3]);
				//Quaternion localOrientation  =  new Quaternion(lo[0], lo[3], -lo[1], lo[2]);
				Quaternion globalOrientation =  new Quaternion(item.go[0], item.go[1], item.go[2], item.go[3]);

				// Zero off the rotations
				if (captureOri)
				{
					//PlayerPrefs.SetFloat(segmentName, inputOrientation.ToString());						
					//rotationOffsets[segmentName] =  Quaternion.AngleAxis(0, Vector3.right) * Quaternion.Inverse ( localOrientation );
				}
			

				GameObject o = null;

				if (item.name == OCULUS_OBJECT || item.name == LEFT_HAND_OBJECT || item.name == RIGHT_HAND_OBJECT)
				{
					if(item.name == "root")
					{
						if ( !objectDict.ContainsKey( item.name) ) continue;
						o = objectDict[item.name];
						o.transform.localRotation = localOrientation;// * rotationOffsets[segmentName]; 
						//object = "Oculus";
						infoMessage += "RIGID: " + item.name + "\n";
						o.transform.localRotation = localOrientation;// * rotationOffsets[segmentName]; 
						//o.transform.localPosition = new Vector3(-tr[0] / 100, -tr[1] / 100, -tr[2] / 100)
					}
				}
				else
				{
					if ( !objectDict.ContainsKey( item.name) ) continue;
					o = objectDict[item.name];
					o.transform.localRotation = localOrientation;// * rotationOffsets[segmentName]; */
				}

				if( o == null) continue;


				o.transform.localPosition = new Vector3(-item.tr[0] / 100, -item.tr[2] / 100, item.tr[1] / 100);

				String dbg = "SEG:" + item.name + "\n";

				Vector3    eus = staticOrientation.eulerAngles;
				Vector3    eul = localOrientation.eulerAngles;
				Vector3    eug = globalOrientation.eulerAngles;
				//Vector3    ofs = rotationOffsets[segmentName].eulerAngles;
				//Vector3    ofi = initialOrientation[segmentName].eulerAngles;
				Vector3    eu2 = o.transform.localRotation.eulerAngles;
				Vector3    pos = o.transform.localPosition;
				
				float px = pos[0] * 1000;
				float py = pos[1] * 1000;
				float pz = pos[2] * 1000;
				
				//dbg += "POS:" + px.ToString ("0.00") + "\t\t" + py.ToString ("0.00") + "\t\t" + pz.ToString("0.00") + "\n";
				//dbg += "S:  " + eus.x.ToString ("0.00") + "\t\t" + eus.y.ToString ("0.00") + "\t\t" + eus.z.ToString("0.00") + "\n";
				//dbg += "L:  " + eul.x.ToString ("0.00") + "\t\t" + eul.y.ToString ("0.00") + "\t\t" + eul.z.ToString("0.00") + "\n";
				//dbg += "G:  " + eug.x.ToString ("0.00") + "\t\t" + eug.y.ToString ("0.00") + "\t\t" + eug.z.ToString("0.00") + "\n";
				//dbg += "OUT:" + eu2.x.ToString ("0.00") + "\t\t" + eu2.y.ToString ("0.00") + "\t\t" + eu2.z.ToString("0.00") + "\n";
				//dbg += "OST:" + ofs.x.ToString ("0.00") + "\t\t" + ofs.y.ToString ("0.00") + "\t\t" + ofs.z.ToString("0.00") + "\n";
				//dbg += "INI:" + ofi.x.ToString ("0.00") + "\t\t" + ofi.y.ToString ("0.00") + "\t\t" + ofi.z.ToString("0.00") + "\n";

				//Debug.Log (dbg);

				string seg = item.name.ToLower();
				if ( seg == "hips" || seg == "spine" || seg == "spine1" || seg == "spine2")
				{
					infoMessage += dbg;
				}

				frameDone = true;
			
			}

			infoMessage += "FPS:" + fps.ToString ("0.00");

			if(frameDone) frameCount++;


			/*
			float f = float.Parse(last, System.Globalization.CultureInfo.InvariantCulture);
			Debug.Log (f);

			if(objects["LeftArm"] != null)
			{
				objects["LeftArm"].transform.eulerAngles = new Vector3(0, f * 180, 0);
			}*/


		}


		captureOri = false;
	}

}
