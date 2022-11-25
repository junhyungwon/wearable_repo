package model

// Current device state, includes - media stack(streaming) status - camera status - recording status - current camera context
type DeviceState struct {
	// Corresponding to media
	Media []MediaState `json:"media,omitempty"`
	// Corresponding to camera
	Cameras []CameraState `json:"cameras"`
	// Whether the current recording is in progress
	Recording bool `json:"recording"`
	// The main camera ID of the current context. 1 fixed for 1 eye camera. For multi-lens cameras, API user's own definition and implementation is required for each camera lens and combination lens.
	CurrentCameraId string `json:"currentCameraId"`
}
