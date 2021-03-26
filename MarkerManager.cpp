#include "MarkerManager.h"

MarkerManager::MarkerManager()
{
}

void MarkerManager::InitMarkers()
{
	for (int i = 0; i < 6; i++)
	{
		Marker* marker = new Marker();
		markers_.push_back(marker);
	}
}

void MarkerManager::SetMarkerTransform(int marker, gef::Matrix44 transform)
{
	//	If this marker is valid then update its transform.
	if (markers_.at(marker - 1))
	{
		markers_.at(marker - 1)->SetTransform(transform);
	}
}

void MarkerManager::SetMarkerDetected(int marker, bool marker_detected)
{
	//	If this marker is valid then update it's detection flag.
	if (markers_.at(marker - 1))
	{
		markers_.at(marker - 1)->SetDetected(marker_detected);
	}
}

//	Test is marker 1 is visible.
bool MarkerManager::IsPlayerReady()
{
	if (IsMarkerDetected(1))
	{
		return true;
	}

	return false;
}

MarkerManager::~MarkerManager()
{
	for (int i = 0; i < markers_.size(); i++)
	{
		if (markers_[i])
		{
			delete markers_[i];
			markers_[i] = nullptr;
		}
	}
}
