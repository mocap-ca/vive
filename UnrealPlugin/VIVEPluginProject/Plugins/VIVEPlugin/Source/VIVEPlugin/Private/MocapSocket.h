#pragma once

#include "VIVEPluginPrivatePCH.h"
#include "MocapSocket.generated.h"

USTRUCT()
struct FSubjectDataStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString m_subjectName;

	TMap<FString, FTransform> m_segments;
	TMap<FString, FTransform> m_markers;
	FSubjectDataStruct()
	{
		m_subjectName = NULL;
	}
};

/**
*
*/
UCLASS(BlueprintType)
class UMocapSocket : public UObject
{
	GENERATED_UCLASS_BODY()

	FSocket* m_socket;
	UPROPERTY()
		FString m_buffer;
	UPROPERTY()
		AActor* m_actor;
	UPROPERTY()
		FTransform markerTransform;
	UPROPERTY()
		FTransform segmentTransform;

	UPROPERTY()
	TArray<FSubjectDataStruct> m_subjects;
	TMap<FString, FTransform> m_segments;
	TMap<FString, FTransform> m_markers;


	bool m_isVicon = false;


public:
	bool Connect(FString ipAddr, int32 port);
	bool ReadData();
	bool IsConnected();
	void AddSegment(FString name, float rx, float ry, float rz, float rw, float tx, float ty, float tz);
	void AddMarker(FString name, float tx, float ty, float tz);
	void AddSegment(FString subjectName, FString name, float rx, float ry, float rz, float rw, float tx, float ty, float tz);
	void AddMarker(FString subjectName, FString name, float tx, float ty, float tz);
	FTransform GetSegmentTransform(const FString name);
	FTransform GetMarkerTransform(const FString name);
	FTransform GetSegmentTransform(const FString subjectName, const FString name);
	FTransform GetMarkerTransform(const FString subjectName, const FString name);

private:
	bool GetOnePacket(FString& packet);

};

