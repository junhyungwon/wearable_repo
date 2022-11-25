package model

// Camera Streaming Settings
type CameraStreaming struct {
	// The resolution value of the camera. - enums: 1280x720, 1920x1080
	Resolution string `json:"resolution,omitempty"`
	// The video frames per second
	Framerate int32 `json:"framerate,omitempty"`
	// The bitrates of the video - enums : 0.5Mbps, 1Mbps, 2Mbps, 4Mbps
	Bitrate string `json:"bitrate,omitempty"`
	// The bitrate mode of video : Variable / Constant - enums : VBR, CBR
	BitrateMode string `json:"bitrateMode,omitempty"`
}
