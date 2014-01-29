using UnityEngine;
using System.Collections;
using ViconDataStreamSDK;
using ViconDataStreamSDK.DotNET;



public class ViconData : MonoBehaviour {

	private bool mConnected;

	private ViconDataStreamSDK.DotNET.Client pClient;

	void Start () {

		Debug.Log ("Starting Vicon");
		mConnected = false;
		pClient = new ViconDataStreamSDK.DotNET.Client();
		Debug.Log ("Connecting to Vicon");
		Output_Connect connect_result = pClient.Connect ("localhost:801");
		if (connect_result.Result == Result.Success)
		{
			Debug.Log ("Connected");
			mConnected = true;
		} else {
			Debug.Log ("Not Connected");
		}

		pClient.EnableSegmentData();
		pClient.SetStreamMode (StreamMode.ClientPull);
		pClient.SetAxisMapping (Direction.Forward, Direction.Left, Direction.Up);



	}

	void Destory() {
		pClient.Disconnect ();
		}

	
	// Update is called once per frame
	void Update () {
		if (!mConnected)
						return;

		Output_GetFrame frame_result = pClient.GetFrame ();
		if (frame_result.Result != Result.Success)
						return;

		Output_GetSubjectCount subjectCountResult = pClient.GetSubjectCount ();
		uint count = subjectCountResult.SubjectCount;
		if (count == 0)	return;

		bool subjectSet = false;
		uint oculusSubject = 0;

		for (uint i=0; i < count; i++) {

			Output_GetSubjectName name = pClient.GetSubjectName ( i );
			Debug.Log ( name.SubjectName);

			if( name.SubjectName.Equals ("OCULUS" ))
			{
				oculusSubject = i;
				subjectSet = true;
			}

		}

		if (!subjectSet) return;


		Output_GetSubjectRootSegmentName rsn = pClient.GetSubjectRootSegmentName ("OCULUS");
		Output_GetSegmentCount sg = pClient.GetSegmentCount ("OCULUS");

		Output_GetSegmentGlobalTranslation grans = pClient.GetSegmentGlobalTranslation ("OCULUS", rsn.SegmentName);
		Vector3 ret;
		ret.x = (float)grans.Translation [0];
		ret.y = (float)grans.Translation [1];
		ret.z = (float)grans.Translation [2];

		outTransform.localPosition = ret;
	
	}
}
