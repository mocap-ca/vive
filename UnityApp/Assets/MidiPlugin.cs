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
	public int note = 66;
	public int channel = 1;
	public int velocity = 100;

	public void Start()
	{
	 	Debug.Log (getPorts ());
		setPort (1);
		isOn = false;
	}

	public void  OnCollisionEnter( Collision c)
	{
		if(channel == -1 && note == -1)
		{
			// All note off
			for(int ch='\0'; ch<16; ch++)
			{
				for(int i=0; i< 128; i++)
				{
					noteOff((char)ch, (char)note, '\0');
				}
			}
		}

		if(isOn) return;
		noteOn ((char)channel, (char)note, (char)velocity);
		isOn = true;
	}

	public void OnCollisionExit( Collision c )
	{
		noteOff ((char)channel, (char)note, (char)velocity);
		isOn = false;
	}





}
