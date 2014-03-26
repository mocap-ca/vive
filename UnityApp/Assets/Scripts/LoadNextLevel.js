#pragma strict

var levelToLoad : String;


function OnTriggerEnter ()
{
	Application.LoadLevel (levelToLoad);
}