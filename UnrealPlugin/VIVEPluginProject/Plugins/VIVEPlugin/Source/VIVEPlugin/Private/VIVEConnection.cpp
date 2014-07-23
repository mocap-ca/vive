#include "VIVEPluginPrivatePCH.h"
#include "VIVEConnection.h"


UVIVEConnection::UVIVEConnection(const FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	
}

UMocapSocket* UVIVEConnection::Connect(AActor* actor, FString ipAddr, int32 port, bool& isConnected)
{
	UMocapSocket* mocap = NewObject<UMocapSocket>(actor, UMocapSocket::StaticClass());
	mocap->m_actor = actor;
	isConnected = mocap->Connect(ipAddr, port);
	return mocap;
}

bool UVIVEConnection::ReadData(UMocapSocket* socket)
{
	if (socket)
	{
		return socket->ReadData();
	}
	return NULL;
}

FTransform UVIVEConnection::GetSegmentTransform(UMocapSocket* socket, FString name)
{
	return socket->GetSegmentTransform(name);
}

FTransform UVIVEConnection::GetMarkerTransform(UMocapSocket* socket, FString name)
{
	return socket->GetMarkerTransform(name);
}

FTransform UVIVEConnection::GetSegmentTransformFromSubject(UMocapSocket* socket, FString subjectName, FString name)
{
	return socket->GetSegmentTransform(subjectName, name);
}

FTransform UVIVEConnection::GetMarkerTransformFromSubject(UMocapSocket* socket, FString subjectName, FString name)
{
	return socket->GetMarkerTransform(subjectName, name);
}