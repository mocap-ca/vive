using System;
using System.Net;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using UnityEngine;

namespace Vive
{

	class PipeConnector : Connector
	{
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

		public PipeConnector( SubjectList sl ) : base (sl) {}

		public override void DisconnectData()
		{
			disconnect ();
		}

		public override void ConnectData()
		{
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

		public override bool IsConnected()
		{
			return isConnected ();
		}

		// Get data from a local pipe (via a dll)
		public override void GetData(out string outData)
		{
			outData = "";
			
			int avail = peek ();
			if (avail == 0) return;

			byte[] socketData = new byte[1024*16];
			int ret = getData (socketData, 1024*16);
			
			if(ret == 1) return;  // No data
			if(ret == 100) throw new InfoException("Pipe Error reading local data");
			if(ret == 101)
			{
				string infoMessage = "Read Error getting local data\n";
				infoMessage += Encoding.ASCII.GetString(socketData) + "\n";
				throw new InfoException ("Read Error: " + Encoding.ASCII.GetString(socketData));
			}
			outData = Encoding.ASCII.GetString(socketData);
		}

	}

} // namespave Vive