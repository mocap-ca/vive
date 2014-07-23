#include "VIVEPluginPrivatePCH.h"
#include "MocapSocket.h"
#define display(text) GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::White, text);


UMocapSocket::UMocapSocket(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

bool UMocapSocket::Connect(FString ipAddr, int32 port)
{
	m_socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("Mocap Socket"), false);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool isValid;
	addr->SetIp(*ipAddr, isValid);
	addr->SetPort(port);
	m_socket->SetNonBlocking(true);
	return m_socket->Connect(*addr);
}

bool UMocapSocket::IsConnected()
{
	return m_socket->GetConnectionState() == ESocketConnectionState::SCS_Connected;
}

bool UMocapSocket::ReadData()
{
	FString packet;
	if (GetOnePacket(packet))
	{
		if (packet.IsEmpty()) return false;
		TArray <FString> lineItems;
		int numItems = packet.ParseIntoArray(&lineItems, TEXT("\n"), true); 
		
		if (numItems == 0)
		{
			return false;
		}
		auto lineItr = lineItems.CreateConstIterator();
		{
			if (!lineItr) return false;
			FString s = *lineItr++;
			int subjects = uint32(FCString::Atoi(*s));
			for (int i=0; i < subjects; ++i)
			{
				TArray <FString> header;
				if (!lineItr) return false;
				FString hStr = *lineItr++;
				hStr.ParseIntoArray(&header, TEXT("\t"), true);
				auto headerItr = header.CreateConstIterator();
				FString name = *headerItr++;

				int32 numSegments = FCString::Atoi(*(*headerItr++));
				int32 numMarkers = FCString::Atoi(*(*headerItr++));
				//FString headerMsg = FString::Printf(TEXT("%s Num Segments:%i Num Markers:%i"), *name, numSegments, numMarkers);
				//display(headerMsg);

				for (int i = 0; i < numSegments; ++i)
				{
					TArray <FString> segmentStr;
					if (!lineItr) return false;
					FString sStr = *lineItr++;
					sStr.ParseIntoArray(&segmentStr, TEXT("\t"), true);
					auto segmentItr = segmentStr.CreateConstIterator();
					FString segName = *segmentItr++;
					float st[3];
					float sr[4];
					for (int k = 0; k < 3; k++)
					{
						st[k] = FCString::Atof(*(*segmentItr++));
					}
					for (int k = 0; k < 4; k++)
					{
						sr[k] = FCString::Atof(*(*segmentItr++));
					}

					AddSegment(name, segName, sr[0], sr[1], sr[2], sr[3], st[0], st[1], st[2]);
					FString segmentMsg = FString::Printf(TEXT("Segment Name: %s tx: %f ty: %f tz: %f rx: %f ry: %f rz: %f rw: %f"),
						*segName, st[0], st[1], st[2], sr[0], sr[1], sr[2], sr[3]);
				}
				for (int i = 0; i < numMarkers; i++)
				{
					TArray <FString> markerStr;
					if (!lineItr) return false;
					FString mStr = *lineItr++;
					mStr.ParseIntoArray(&markerStr, TEXT("\t"), true);
					auto markerItr = markerStr.CreateConstIterator();
					FString marName = *markerItr++;
					float mt[3];
					for (int k = 0; k < 3; k++)
					{
						mt[k] = FCString::Atof(*(*markerItr++));
					}
					AddMarker(name, marName, mt[0], mt[1], mt[2]);
					FString markerMsg = FString::Printf(TEXT("Marker Name: %s tx: %f ty: %f tz: %f"),
						*marName, mt[0], mt[1], mt[2]);
				}
			}
		}
		return true;
	}
	return false;
}

bool UMocapSocket::GetOnePacket(FString& packet)
{
	uint32 numBytes;
	if(!m_socket->HasPendingData(numBytes))
	{
		return false;
	}
	uint8* data = new uint8[1024 * 16];
	int32 bytesRead;
	m_socket->Recv(data, numBytes, bytesRead);
	if (bytesRead == 0)
	{
		return false;
	}
	// Convert data to String and strip trailing whitespace
	m_buffer += FString(UTF8_TO_TCHAR(data)).Left(bytesRead);
	int splitHere = m_buffer.Find(TEXT("END\r\n"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (splitHere == -1)
	{
		return false;
	}
	packet = m_buffer.Left(splitHere);
	// add any leftover data to buffer
	if (m_buffer.Len() == splitHere + 5)
	{
		m_buffer = TEXT("");
	}
	else
	{
		m_buffer = m_buffer.Right(splitHere + 5);
	}
	return true;
}

void UMocapSocket::AddSegment(FString name, float rx, float ry, float rz, float rw, float tx, float ty, float tz)
{
	if (m_isVicon)
	{
		// vicon z-up configuration -- need to move this so that it happens in the repeater
		m_segments.Add(name, FTransform(FQuat(-ry, rx, rz, rw), FVector(-ty, tx, tz), FVector(1.0f, 1.0f, 1.0f)));
	}
	else
	{
		m_segments.Add(name, FTransform(FQuat(rz, rx, ry, rw), FVector(tz, tx, ty), FVector(1.0f, 1.0f, 1.0f)));
	}
}

void UMocapSocket::AddMarker(FString name, float tx, float ty, float tz)
{
	if (m_isVicon)
	{
		// vicon z-up configuration -- need to move this so that it happens in the repeater
		m_markers.Add(name, FTransform(FQuat::Identity, FVector(-ty, tx, tz), FVector(1.0f, 1.0f, 1.0f)));
	}
	else
	{
		m_markers.Add(name, FTransform(FQuat::Identity, FVector(tz, tx, ty), FVector(1.0f, 1.0f, 1.0f)));
	}
}

void UMocapSocket::AddSegment(FString subjectName, FString name, float rx, float ry, float rz, float rw, float tx, float ty, float tz)
{
	FTransform segTrans;
	if (m_isVicon) {
		segTrans = FTransform(FQuat(-ry, rx, rz, rw), FVector(-ty, tx, tz), FVector(1.0f, 1.0f, 1.0f));
	}
	else
	{
		segTrans = FTransform(FQuat(rz, rx, ry, rw), FVector(tz, tx, ty), FVector(1.0f, 1.0f, 1.0f));
	}
	for (auto subj = m_subjects.CreateIterator(); subj; ++subj)
	{
		if ((*subj).m_subjectName.Equals(subjectName))
		{
			(*subj).m_segments.Add(name, segTrans);
			return;
		}
	}
	struct FSubjectDataStruct subject;
	subject.m_subjectName = subjectName;
	subject.m_segments.Add(name, segTrans);
	m_subjects.Add(subject);

}

void UMocapSocket::AddMarker(FString subjectName, FString name, float tx, float ty, float tz)
{

}

FTransform UMocapSocket::GetSegmentTransform(const FString name)
{
	if (m_segments.Contains(name))
	{
		return *m_segments.Find(name);
	}
	return FTransform(FQuat::Identity, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));
}

FTransform UMocapSocket::GetMarkerTransform(const FString name)
{
	if (m_markers.Contains(name))
	{
		return *m_markers.Find(name);
	}
	return FTransform(FQuat::Identity, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));
}

FTransform UMocapSocket::GetSegmentTransform(const FString subjectName, const FString name)
{
	for (FSubjectDataStruct subj:  m_subjects)
	{
		if (subj.m_subjectName.Equals(subjectName))
		{
			if (subj.m_segments.Contains(name))
			{
				return *subj.m_segments.Find(name);
			}
		}
	}
	return FTransform(FQuat::Identity, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));
}

FTransform UMocapSocket::GetMarkerTransform(const FString subjectName, const FString name)
{

	return FTransform(FQuat::Identity, FVector::ZeroVector, FVector(1.0f, 1.0f, 1.0f));
}


