#pragma strict

var outCamera : Transform;
var rot = new Vector2( 0.0f, 0.5f);
var distance : float = 10.0f;
var interest = new Vector3( 0.0f, 1.0f, 0.0f );
var orbitScale : float = 5.0f;
var trackingScale : float = 100.0f;
var zoomScale : float = 200.0f;

private var offset : Vector2;
private var track : Vector2;
private var zoom : float = 0.0f;
private var initialOrbit : Vector2;
private var initialTrack : Vector2;
private var initialZoom  : Vector2;
private var tracking : boolean;
private var orbiting : boolean;
private var zooming  : boolean;


function Update () {

    // Orbiting
    if(Input.GetMouseButtonDown(0))
    {
        orbiting = true;
        offset = Vector2(0.0f, 0.0f);
        initialOrbit = Input.mousePosition;
    }

    if(Input.GetMouseButtonUp(0))
    {
        if(orbiting)
        {
            rot += offset;
            offset = Vector2(0.0f, 0.0f);
            orbiting =  false;
        }
    } 
    
    if(orbiting)
    {
        offset = (Input.mousePosition - initialOrbit) / orbitScale;
    }
    
    // Tracking
    if(Input.GetMouseButtonDown(2))
    {
        tracking = true;
        track = Vector2(0.0f, 0.0f);
        initialTrack = Input.mousePosition;
    }
        
    if(tracking)
    {
        track = (initialTrack - Input.mousePosition ) / trackingScale;
    }
    
    // Zooming
    if(Input.GetMouseButtonDown(1))
    {
        zooming = true;
        zoom = 0;
        initialZoom = Input.mousePosition;
    }
    
    if(Input.GetMouseButtonUp(1))
    {
        if(zooming)
        {
            distance += zoom;
            zoom = 0;
            zooming =  false;
        }
    }     
    
    if(zooming)
    {
        var diff = Input.mousePosition.y - initialZoom.y;
        zoom = diff / zoomScale;    
        if( distance + zoom < 0.1) zoom = 0.1 - distance;
    }
        
    var urot = Quaternion.AngleAxis( rot.x + offset.x, Vector3.up );
    var vrot = Quaternion.AngleAxis( rot.y + offset.y, Vector3.left );
        
    outCamera.position = interest;
    outCamera.rotation = urot * vrot;
    outCamera.Translate( Vector3( track.x, track.y, 0) );
    
    if(Input.GetMouseButtonUp(2))
    {
        if(tracking)
        {
            tracking = false;
            interest = outCamera.position;
            track = Vector2(0.0f, 0.0f);
        }
    }
    
    outCamera.Translate( Vector3( 0.0f, 0.0f, -distance - zoom ) );
    
}