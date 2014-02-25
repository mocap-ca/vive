using UnityEngine;
using System.Collections;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections.Generic;



public class MocapSocket : MonoBehaviour {

	Byte[] data;
	Socket clientSocket;
	string buffer;
	string infoMessage;

	public string hostIp;
	public int port;
	Boolean toggleDisplay;

	string[] transformList;
	Dictionary< string, GameObject > objects = new Dictionary< string, GameObject> ();
	Dictionary< string, Quaternion > rotationOffsets = new Dictionary<string, Quaternion> ();
	Dictionary< string, Quaternion > initialOrientation = new Dictionary<string, Quaternion> ();

	int frameCount = 0;
	double dt = 0.0;
	double fps = 0.0;
	double updateRate = 4.0;  // 4 updates per sec.

	
	// Use this for initialization
	void Start () {


		toggleDisplay = false;

		transformList =  new string[] {"Hips",  
			"LeftUpLeg", "LeftLeg", "LeftFoot", "LeftToeBase", 
			"RightUpLeg", "RightLeg", "RightFoot", "RightToeBase", 
			"Spine", "Spine1", "Spine2", "Spine3",
			"Neck", "Neck1", "Head",     
			"LeftShoulder", "LeftArm", "LeftForeArm", "LeftHand", 
			"RightShoulder", "RightArm", "RightForeArm", "RightHand", "root"
		};

		foreach (string s in transformList) 
		{
			GameObject o = GameObject.Find (s);

			if (o == null)
			{
				Debug.Log ("Could not find object: " + s);
			}
			else
			{
				objects.Add (s, o);
				rotationOffsets.Add ( s, Quaternion.identity );
				initialOrientation.Add ( s, o.transform.localRotation );
			}
		}

		data = new Byte[1024];

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
		
		Debug.Log("Connecting to localhost");
		clientSocket.Connect (ipEndPoint);
		
		// arriving here means the operation completed asyncConnect.IsCompleted = true)
		// but not necessarily successfully
		if (clientSocket.Connected == false)
		{
			Debug.Log(".client is not connected.");
			return;
		}
		
		Debug.Log(".client is connected.");
		
		clientSocket.Blocking = false;


	}

	void Disconnect()
	{
		if (clientSocket.Connected) clientSocket.Disconnect (false);
	}
	 
	 
	void OnGUI()
	{
		if (!clientSocket.Connected)
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

			
	// Update is called once per frame
	void Update ()
	{
		if (Input.GetKeyDown ("z"))
						toggleDisplay = !toggleDisplay;

		if (clientSocket.Available == 0)
						return;

		// Frame rate calculator

		dt += Time.deltaTime;
		if (dt > 1.0/updateRate)
		{
			fps = frameCount / dt ;
			frameCount = 0;
			dt -= 1.0/updateRate;
		}



		try
		{
			// Get some data, append to current buffer
			int ret = clientSocket.Receive (data, 1024, 0);
			buffer += Encoding.ASCII.GetString(data, 0, ret);

			// find split point
			int splitHere = buffer.LastIndexOf ("END\r\n");
			if(splitHere == -1) return;

			// everything after last split point gets buffered
			string extracted = buffer.Substring(0, splitHere);
			if(buffer.Length == splitHere + 5)
				buffer = "";
			else
				buffer = buffer.Substring (splitHere+5);

			// split packets
			string[] splitsPackets = { "END\r\n" };
			string[] packetItems = extracted.Split(splitsPackets, StringSplitOptions.RemoveEmptyEntries);
			if(packetItems.Length == 0) return;

			infoMessage = "";

			// get last packet
			string last = packetItems[packetItems.Length -1];

			//Debug.Log ( "\"" + packetItems.Length + "\"");

			string[] lineItems = last.Split ('\n');
			if(lineItems.Length == 0)
			{
				infoMessage = "No data error";
				return;
			}

			int subjects = 0;
			if(!int.TryParse ( lineItems[0], out subjects))
			{
				infoMessage = "Invalid data while parsing";
				return;
			}
			int line = 1;
			bool frameDone = false;
			for(int i=0; i < subjects && line < lineItems.Length; i++)
			{
				string[] subjectSplit = lineItems[line++].Split ('\t');
				string subjectName = subjectSplit[0];
				int    noSegments  = Convert.ToInt32 (subjectSplit[1]);
				infoMessage += "SUB: " + subjectName + "\n";


				for(int j=0; j < noSegments && line < lineItems.Length; j++)
				{
					string[] segmentSplit = lineItems[line++].Split('\t');

					if(segmentSplit.Length != 16)
					{
						infoMessage += "Segments: " + segmentSplit.Length;
						continue;
					}

					string segmentName = segmentSplit[0];
					float[] tr = new float[3];
					float[] so = new float[4];
					float[] lo = new float[4];
					float[] go = new float[4];
					for(int k=0; k < 3; k++) tr[k] = float.Parse (segmentSplit[k+1]);
					for(int k=0; k < 4; k++) so[k] = float.Parse (segmentSplit[k+4]);
					for(int k=0; k < 4; k++) lo[k] = float.Parse (segmentSplit[k+8]);
					for(int k=0; k < 4; k++) go[k] = float.Parse (segmentSplit[k+12]);

					if(objects.ContainsKey (segmentName))
					{
					
						//Quaternion inputOrientation =  Quaternion.AngleAxis(90, Vector3.left) * new Quaternion(rx, ry, rz, rw);
						Quaternion staticOrientation =  new Quaternion(so[0], so[1], so[2], so[3]);
						Quaternion localOrientation  =  new Quaternion(lo[0], lo[2], -lo[1], lo[3]);
						Quaternion globalOrientation =  new Quaternion(go[0], go[1], go[2], go[3]);

						// Zero off the rotation
						if (Input.GetKeyDown ("1"))
						{
							//PlayerPrefs.SetFloat(segmentName, inputOrientation.ToString());						
							rotationOffsets[segmentName] = Quaternion.Inverse ( localOrientation );
						}

						GameObject o = objects[segmentName];
						o.transform.localRotation = localOrientation * rotationOffsets[segmentName]; 

						//if(segmentName.ToLower() == "root" || segmentName.ToLower() == "hips")
						//{
						// Here x is inverted because unity has a left handed coordinate system
						// Also we need to swap y and z and the data from the vicon feed comes
						// in as z up (even though in the blade interface it shows it as y up)
							o.transform.localPosition = new Vector3(-tr[0] / 100, -tr[2] / 100, tr[1] / 100);
						//	infoMessage += "*";
						//}


						String dbg = "SEG:" + segmentName + "\n";

						Vector3    eus = staticOrientation.eulerAngles;
						Vector3    eul = localOrientation.eulerAngles;
						Vector3    eug = globalOrientation.eulerAngles;
						Vector3    ofs = rotationOffsets[segmentName].eulerAngles;
						Vector3    ofi = initialOrientation[segmentName].eulerAngles;
						Vector3    eu2 = o.transform.localRotation.eulerAngles;
						Vector3    pos = o.transform.localPosition;
						
						float px = pos[0] * 1000;
						float py = pos[1] * 1000;
						float pz = pos[2] * 1000;
						
						dbg += "POS:" + px.ToString ("0.00") + "\t\t" + py.ToString ("0.00") + "\t\t" + pz.ToString("0.00") + "\n";
						//dbg += "S:  " + eus.x.ToString ("0.00") + "\t\t" + eus.y.ToString ("0.00") + "\t\t" + eus.z.ToString("0.00") + "\n";
						dbg += "L:  " + eul.x.ToString ("0.00") + "\t\t" + eul.y.ToString ("0.00") + "\t\t" + eul.z.ToString("0.00") + "\n";
						//dbg += "G:  " + eug.x.ToString ("0.00") + "\t\t" + eug.y.ToString ("0.00") + "\t\t" + eug.z.ToString("0.00") + "\n";
						//dbg += "OUT:" + eu2.x.ToString ("0.00") + "\t\t" + eu2.y.ToString ("0.00") + "\t\t" + eu2.z.ToString("0.00") + "\n";
						//dbg += "OST:" + ofs.x.ToString ("0.00") + "\t\t" + ofs.y.ToString ("0.00") + "\t\t" + ofs.z.ToString("0.00") + "\n";
						//dbg += "INI:" + ofi.x.ToString ("0.00") + "\t\t" + ofi.y.ToString ("0.00") + "\t\t" + ofi.z.ToString("0.00") + "\n";

						Debug.Log (dbg);

						string seg = segmentName.ToLower();
						if ( seg == "hips" || seg == "spine" || seg == "spine1" || seg == "spine2")
						{
							infoMessage += dbg;
						}
						


						frameDone = true;
					}

				}
			}

			infoMessage += "FPS:" + fps;

			if(frameDone) frameCount++;


			/*
			float f = float.Parse(last, System.Globalization.CultureInfo.InvariantCulture);
			Debug.Log (f);

			if(objects["LeftArm"] != null)
			{
				objects["LeftArm"].transform.eulerAngles = new Vector3(0, f * 180, 0);
			}*/


		}
		catch( SocketException e)	
		{
			Debug.Log ("Socket Error: " + e.Message);
		}

	}
}
