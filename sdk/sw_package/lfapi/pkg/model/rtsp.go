package model

// The RTSP access point information
type Rtsp struct {
	// Sub-address value used when connecting. Append after IP and port value when connecting.
	Path string `json:"path"`
	// The port number used when connecting. The value appended to the IP value when connecting.
	Port int32 `json:"port"`
}
