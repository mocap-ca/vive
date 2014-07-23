#pragma once

#include "VIVEPluginPrivatePCH.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MocapSocket.h"
#include "VIVEConnection.generated.h"

UCLASS()
class UVIVEConnection : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Connect to Mocap Socket", Category = Network))
		static UMocapSocket* Connect(AActor* actor, FString ipAddr, int32 port, bool& isConnected);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Read Mocap Data", Keywords = "Mocap Read Frame Socket Network", Category = Network))
		static  bool ReadData(UMocapSocket* socket);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Segment Transform", Category = Network))
		static FTransform GetSegmentTransform(UMocapSocket* socket, FString segmentName);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Marker Transform", Category = Network))
		static FTransform GetMarkerTransform(UMocapSocket* socket, FString markerName);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Segment Transform", Category = Network))
		static FTransform GetSegmentTransformFromSubject(UMocapSocket* socket, FString subjectName, FString segmentName);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Get Marker Transform", Category = Network))
		static FTransform GetMarkerTransformFromSubject(UMocapSocket* socket, FString subjectName, FString markerName);
};
