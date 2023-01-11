package model

// Current status of a camera
type CameraState struct {
	// Camera's unique ID
	CameraId string `json:"cameraId"`
	// Current state of a camera - UNKNOWN - OK - ERROR
	State string `json:"state"`
}
