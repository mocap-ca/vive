using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;

public class MidiPlugin : MonoBehaviour {

	// Use this for initialization

	[DllImport("UnityDllTest",  EntryPoint = "getPorts", CallingConvention = CallingConvention.Cdecl) ]
	private static extern uint getPorts ();

	[DllImport("UnityDllTest",  EntryPoint = "getPortName", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void getPortName (uint port, byte[] data, uint buflen);

	[DllImport("UnityDllTest",  EntryPoint = "setPort", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void setPort (uint port);

	[DllImport("UnityDllTest",  EntryPoint = "noteOn", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void noteOn (char chan, char note, char velocity);

	[DllImport("UnityDllTest",  EntryPoint = "noteOff", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void noteOff (char chan, char note, char velocity);

	[DllImport("UnityDllTest",  EntryPoint = "progChange", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void progChange (char chan, char prog);

	[DllImport("UnityDllTest",  EntryPoint = "ctrlChange", CallingConvention = CallingConvention.Cdecl) ]
	private static extern void ctrlChange (char chan, char ctrl, char val);

	private bool isOn;

	public void Start()
	{
	 	Debug.Log (getPorts ());
		setPort (1);
		isOn = false;
	}


	public void OnGUI() {
		if (!isOn && Event.current.type == EventType.KeyDown) {
			Debug.Log ("ON");
			noteOn ((char)1, (char)66, (char)90);
			isOn = true;
		}
		if (Event.current.type == EventType.KeyUp) {
			Debug.Log ("OFF");
			noteOff ((char)1, (char)66, (char)40);
			isOn = false;
		}
	}
}
