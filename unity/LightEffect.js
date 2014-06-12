#pragma strict

var inLight : Light;
var inPosition : Transform;

var minColor : float = 0.6;
var maxColor : float = 0.8;

var minPoint : Vector3;
var maxPoint : Vector3;

var transformSize : float = 0.05;
var colorSize : float = 0.05;

private var colorDirection : boolean = false;
private var transformDirection : Vector3;

private var colorValue : float = minColor;
private var transformValue : Vector3 = minPoint;




function Start () {

}

function OnGUI()
{
	//GUI.Label( Rect( 20, 20, 100, 100 ), String.Format("{0}", value));
} 

function Update () {

if(Random.Range(0.0, 1.0) > 0.5) colorDirection = !colorDirection;

transformDirection.x += Random.Range( -transformSize, transformSize );
transformDirection.y += Random.Range( -transformSize, transformSize );
transformDirection.z += Random.Range( -transformSize, transformSize );

transformValue += transformDirection;

if(transformValue.x < minPoint.x) { transformValue.x = minPoint.x; transformDirection.x = transformSize/2; }
if(transformValue.y < minPoint.y) { transformValue.y = minPoint.y; transformDirection.y = transformSize/2; }
if(transformValue.z < minPoint.z) { transformValue.z = minPoint.z; transformDirection.z = transformSize/2; }

if(transformValue.x > maxPoint.x) { transformValue.x = maxPoint.x; transformDirection.x = -transformSize/2; }
if(transformValue.y > maxPoint.y) { transformValue.y = maxPoint.y; transformDirection.y = -transformSize/2; }
if(transformValue.z > maxPoint.z) { transformValue.z = maxPoint.z; transformDirection.z = -transformSize/2; }

inPosition.position = transformValue;

if(colorDirection)
	colorValue += colorSize;
else 
	colorValue -= colorSize;
	
if(colorValue < minColor) colorValue = minColor;
if(colorValue > maxColor) colorValue = maxColor;
	

inLight.color = Color.red * colorValue;
}