package model

// Detailed image quality settings of the camera
type CameraAdjustment struct {
	Brightness int32 `json:"brightness,omitempty"`

	Contrast int32 `json:"contrast,omitempty"`

	BacklightCompensation int32 `json:"backlightCompensation,omitempty"`

	Saturation int32 `json:"saturation,omitempty"`

	RedGain int32 `json:"redGain,omitempty"`

	BlueGain int32 `json:"blueGain,omitempty"`

	Sharpness int32 `json:"sharpness,omitempty"`

	PowerLine int32 `json:"powerLine,omitempty"`

	NoiseReduction int32 `json:"noiseReduction,omitempty"`
}
