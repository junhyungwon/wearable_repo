package model

// Current status of a camera media
type MediaState struct {
	CameraId string `json:"cameraId"`
	// - UNAVAILABLE - PREPARING - IDLE - STREAMING
	Rtsp string `json:"rtsp,omitempty"`
	// - UNAVAILABLE - PREPARING - IDLE - PUBLISHING
	Rtmp string `json:"rtmp,omitempty"`
}
