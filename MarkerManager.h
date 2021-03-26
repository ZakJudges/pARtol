#pragma once

#include <vector>
#include "Marker.h"

//	Class that keeps track of markers from the real world.
class MarkerManager
{
public:
	MarkerManager();
	void InitMarkers();
	void SetMarkerTransform(int marker, gef::Matrix44 transform);
	inline gef::Matrix44 GetMarkerTransform(int marker) { return markers_[marker-1]->GetTransform(); }
	inline bool IsMarkerDetected(int marker) { return markers_[marker-1]->IsDetected(); }
	inline int GetNumberOfMarkers() { return markers_.size(); }
	void SetMarkerDetected(int marker, bool marker_detected);
	bool IsPlayerReady();
	~MarkerManager();
	
private:
	std::vector<Marker*> markers_;
};