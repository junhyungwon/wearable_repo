package model

// Defined error codes. To be added continuously. - 40000 Unknown error - 40001 Parsing error - 40002 Device busy - 40003 Invalid parameter - 40004 Data not found - 40005 Not allowed - 40006 Previous command in progress - 40007 Unsupported command type - 40008 Unsupported behavior
type ModelError struct {
	// The error code
	Code string `json:"code,omitempty"`
	// The error message
	Message string `json:"message,omitempty"`
}
