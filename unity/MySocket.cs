using UnityEngine;
using System.Collections;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections.Generic;


public class MySocket : MonoBehaviour {

	Byte[] data;
	Socket clientSocket;
	string buffer;
	string infoMessage;

	public string hostIp;
	public int port;
	Boolean toggleDisplay;

	string[] transformList;
	Dictionary< string, GameObject > objects = new Dictionary< string, GameObject> ();

	
	// Use this for initialization
	void Start () {

		toggleDisplay = false;

		transformList =  new string[] {"Hips",  
			"LeftUpLeg", "LeftLeg", "LeftFoot", "LeftToeBase", 
			"RightUpLeg", "RightLeg", "RightFoot", "RightToeBase", 
			"Spine", "Spine1", "Spine2", 
			"Neck", "Neck1", "Head",     
			"LeftShoulder", "LeftArm", "LeftForeArm", "LeftHand", 
			"RightShoulder", "RightArm", "RightForeArm", "RightHand", "ROOT"
		};

		foreach (string s in transformList) 
		{
			GameObject o = GameObject.Find (s);

			if (o == null)
				Debug.Log ("Could not find object: " + s);
			else
				objects.Add (s, o);
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

		if(toggleDisplay)
			GUI.Label(	new Rect(5, 5, 	Screen.width, Screen.height), infoMessage);
		//if(GUI.Button (new Rect(120,10,100,20), "disconnect"))
		//{
		//	currentState = 2;
		//}
	}
	
			
	// Update is called once per frame
	void Update ()
	{
		if (Input.GetKeyDown ("z"))
						toggleDisplay = !toggleDisplay;

		if (clientSocket.Available == 0)
						return;


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
			if (clientSocket.Connected) infoMessage = "Connected\n";


			// get last packet
			string last = packetItems[packetItems.Length -1];

			//Debug.Log ( "\"" + last + "\"");

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
			for(int i=0; i < subjects && line < lineItems.Length; i++)
			{
				string[] subjectSplit = lineItems[line++].Split ('\t');
				string subjectName = subjectSplit[0];
				int    noSegments  = Convert.ToInt32 (subjectSplit[1]);
				infoMessage += "Subject: " + subjectName + "\n";

				for(int j=0; j < noSegments && line < lineItems.Length; j++)
				{
					string[] segmentSplit = lineItems[line++].Split('\t');

					if(segmentSplit.Length != 7) continue;

					string segmentName = segmentSplit[0];
					float tx = float.Parse (segmentSplit[1]);
					float ty = float.Parse (segmentSplit[2]);
					float tz = float.Parse (segmentSplit[3]);
					float rx = float.Parse (segmentSplit[4]);
					float ry = float.Parse (segmentSplit[5]);
					float rz = float.Parse (segmentSplit[6]);

					infoMessage += "\t" + segmentName + "\t";
					infoMessage += "\t" + tx + "\t" + ty + "\t" + tz;
					infoMessage += "\t" + rx + "\t" + ry + "\t" + rz + "\n";

					if(objects.ContainsKey (segmentName))
					{
						GameObject o = objects[segmentName];
						o.transform.localPosition = new Vector3(tx, ty, tz);
					}

				}
			}


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
