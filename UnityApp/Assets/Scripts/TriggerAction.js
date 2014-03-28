#pragma strict

var bloop : AudioClip;

function Start () {

}

function Update () {

}


function OnTriggerEnter(trigger: Collider)
{

	//Debug.Log("Trigger: ");
}

function OnCollisionEnter(col : Collision)
{
audio.volume = 1.5;
audio.clip = bloop;
audio.Play();
	Debug.Log(this.name + " collide event: " + col.gameObject.name);
}