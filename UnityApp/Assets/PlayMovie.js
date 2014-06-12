#pragma strict

var t : MovieTexture;

function Start () {

	t = renderer.material.mainTexture;

	t.loop = true;
	t.Play();
}

function Update () {


	
}