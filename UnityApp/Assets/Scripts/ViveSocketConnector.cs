using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Text;

namespace Vive
{
		
	class SocketConnector : Connector
	{
		private Socket clientSocket;
		private string buffer;

		public string  hostIp;
		public int     port;

		public SocketConnector( SubjectList sl, string ip, int p )
		: base (sl)
		{
			hostIp = ip;
			port   = p;
		}

		public override void DisconnectData()
		{
			if (!clientSocket.Connected) return;
			clientSocket.Disconnect (false);
		}

		public override bool IsConnected()
		{
			return clientSocket.Connected;
		}

		public override void ConnectData()
		{
			IPEndPoint ipEndPoint = new IPEndPoint (IPAddress.Parse (hostIp), port);
			AddressFamily family  = AddressFamily.InterNetwork;
			SocketType sokType    = SocketType.Stream;
			ProtocolType proType  = ProtocolType.Tcp;
			clientSocket          = new Socket (family, sokType, proType);
			
			try
			{
				clientSocket.Connect (ipEndPoint);
				if (clientSocket.Connected == false) throw new InfoException("Could not connect");
				clientSocket.Blocking = false;
			}
			catch (SocketException e)
			{
				throw new InfoException("Could not connect: " + e.ToString());
			}
		}

		// Get data from a TCP socket
		public override void GetData(out string outData)
		{
			outData = "";
			
			if (clientSocket == null || !clientSocket.Connected)
				throw new InfoException("Not Connected while trying to get packet\n");
			
			try
			{
				// Get some data, append to current buffer
				byte[] socketData = new byte[1024*16];
				int ret = clientSocket.Receive (socketData, 1024*16, 0);
				buffer += Encoding.ASCII.GetString(socketData, 0, ret);
				
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
				
				outData = packetItems[packetItems.Length -1];
			}
			catch(SocketException e)
			{
				if(e.ErrorCode == 10035) return;
				throw new InfoException("Socket Error: " + e.ToString() + " (" + e.ErrorCode + ")\n");
			}
		}
	}
}// namespace Vive
