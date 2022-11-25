package model

// A bearer access token
type Token struct {
	// Authentication token used when making API calls
	AccessToken string `json:"access_token"`
}
