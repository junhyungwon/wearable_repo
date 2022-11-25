package model

// The configuration a specific camera
type Camera struct {
	// Unique ID of the camera. Fixed at 1 for single lens cameras. In the case of a multi-lens camera and a virtual camera combined with images, individual definitions are required.
	CameraId string `json:"cameraId"`

	Recording *CameraRecording `json:"recording,omitempty"`

	Streaming *CameraStreaming `json:"streaming,omitempty"`

	Adjustment *CameraAdjustment `json:"adjustment,omitempty"`

	Option *CameraOption `json:"option,omitempty"`
}
