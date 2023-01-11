package model

// The user's web ID and password
type Admin struct {
	// Currently set web ID
	Id string `json:"id"`
	// Currently set web password
	Password string `json:"password"`
}
