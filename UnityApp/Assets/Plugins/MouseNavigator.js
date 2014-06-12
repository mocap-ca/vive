/*
VIVE - Very Immersive Virtual Experience
Copyright (C) 2014 Alastair Macleod, Emily Carr University

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma strict

var rot = new Vector2( 0.0f, 0.5f);
var distance : float = 10.0f;
var interest = new Vector3( 0.0f, 1.0f, 0.0f );
var orbitScale : float = 5.0f;
var trackingScale : float = 100.0f;
var zoomScale : float = 2000.0f;

private var offset       : Vector2;
private var track        : Vector2;
private var zoom         : float = 0.0f;
private var initialOrbit : Vector2;
private var initialTrack : Vector2;
private var initialZoom  : Vector2;
private var tracking     : boolean = false;
private var orbiting     : boolean = false;
private var zooming      : boolean = false;

static var hasControl : boolean = true;

function Start()
{
	var o : GameObject;
	o = GameObject.FindWithTag("MainCamera");
    var t : Transform = o.transform;
    t.Translate( Vector3( 0.0f, 0.0f, distance + zoom ) );
	interest = t.localPosition;
}

function Update ()
{
	if(!hasControl) return;


    /* Orbiting */
    
    if(Input.GetMouseButtonDown(0))
    {
        orbiting = true;
        initialOrbit = Input.mousePosition;
    }

	if(Input.GetMouseButtonUp(0))
    {
	    rot += offset;
	    offset = Vector2(0.0f, 0.0f);
	    orbiting =  false;
    } 
    
    if(orbiting)
    {
		offset = (Input.mousePosition - initialOrbit) / orbitScale;
	}
	
    
    /* Tracking */
    
    if(Input.GetMouseButtonDown(2))
    {
        tracking = true;
        track = Vector2(0.0f, 0.0f);
        initialTrack = Input.mousePosition;
    }
        
    if(tracking)
    {
        track = (initialTrack - Input.mousePosition ) * Mathf.Exp((distance + zoom) / 150) / trackingScale;
    }
    
    
    /* Zooming */
    
    if(Input.GetMouseButtonDown(1))
    {
        zooming = true;
        zoom = 0;
        initialZoom = Input.mousePosition;
    }
    
    if(Input.GetMouseButtonUp(1))
    {
        distance += zoom;
        zoom = 0;
        zooming =  false;
    }     
    
    if(zooming)
    {
        zoom = Input.mousePosition.y - initialZoom.y;
    }
   	
    var urot = Quaternion.AngleAxis( rot.x + offset.x, Vector3.up );
    var vrot = Quaternion.AngleAxis( rot.y + offset.y, Vector3.left );
       
    transform.position = interest;
    transform.rotation = urot * vrot;
    transform.Translate( Vector3( track.x, track.y, 0) );
    
    if(Input.GetMouseButtonUp(2) && tracking)
    {
	    tracking = false;
	    interest = transform.position;
	    track = Vector2(0.0f, 0.0f);
    }
    
    transform.Translate( Vector3( 0.0f, 0.0f, -Mathf.Exp((distance + zoom) / 100) ) );
    
}