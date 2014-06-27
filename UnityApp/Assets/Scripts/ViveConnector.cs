using Vive;
using System.Collections.Generic;
using System;
using UnityEngine;

namespace Vive
{

	public abstract class Connector 
	{
		private SubjectList subjectList;

		public Connector( SubjectList sl )
		{
			subjectList = sl;
		}


		// subclass must provide the functionality to get the data, e.g. by tcp/udp/named-pipe connection.
		public abstract void GetData (out string outData);

		public abstract void ConnectData();

		public abstract void DisconnectData();

		public abstract bool IsConnected();

		// Get data, parse and return as a subject list
		public void GetData()
		{
			string packet = "";

			subjectList.Clear ();

			// Get the data from repeater
			try	{ GetData (out packet); }
			catch( InfoException e)	{ throw; }
		
			// Split the data in to lines
			string[] lineItems = packet.Split ('\n');
			if (lineItems.Length == 0) throw new InfoException("No data");
			if (lineItems.Length == 1 && lineItems[0].Length == 0) return;
			
			int subjects = 0;
		
			// First line should be the number of subjects
			if (!int.TryParse (lineItems [0], out subjects)) 
					throw new InfoException("Invalid data while parsing subjects\n");

			if( subjects == 0) return;
		
			int line = 1;
		
			// For each line
			for (int i=0; i < subjects && line < lineItems.Length; i++)
			{
				string[] subjectSplit = lineItems [line++].Split ('\t');
				string   subjectName  = subjectSplit [0].ToLower ();

				int noJoints  = Convert.ToInt32 (subjectSplit [1]);
				int noMarkers = Convert.ToInt32 (subjectSplit [2]);
				//infoMessage += subjectName + "  " + noJoints + " segments   " + noMarkers + " markers\n";
			
				Subject subject = new Subject(subjectName);
			
				// Joints	
				for (int j=0; j < noJoints && line < lineItems.Length; j++)
				{
					string[] segmentSplit = lineItems [line++].Split ('\t');
				
					if (segmentSplit.Length != 8) throw new InfoException("Segment Error: " + segmentSplit.Length);

					float[] tr = new float[3];
					float[] ro = new float[4];
					for (int k=0; k < 3; k++)
						tr [k] = float.Parse (segmentSplit [k + 1]);
					for (int k=0; k < 4; k++)
						ro [k] = float.Parse (segmentSplit [k + 4]);
					subject.AddJoint(new Joint (segmentSplit [0], tr, ro));
				}
			
				float[] zero = new float[4];
				for (int j=0; j < 4; j++) zero [j] = 0.0f;
			
				// Markers
				for (int j=0; j < noMarkers && line < lineItems.Length; j++)
				{
					string[] segmentSplit = lineItems [line++].Split ('\t');
				
					if (segmentSplit.Length != 4)
					{
						Debug.Log ("Marker Error" + segmentSplit.Length);
						continue;
					}
					float[] tr = new float[3];
					for (int k=0; k < 3; k++)
						tr [k] = float.Parse (segmentSplit [k + 1]);
					subject.AddMarker(new Marker (segmentSplit [0], tr));
				}
				subjectList.Update(subject);
			}
		}

	}

}// namespace Vive
