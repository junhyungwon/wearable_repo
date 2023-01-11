package model

// The device's static information
type Device struct {
	// Unique ID of the device
	DeviceId string `json:"deviceId"`
	// Model name of the device
	Model string `json:"model"`
	// Firmware version
	Version string `json:"version"`
}
