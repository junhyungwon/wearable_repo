/*
 * LFAPI (prototype)
 *
 * This is APIs for a RESTful based interface to a specific camera (private). Project details such as author, requester and purpose of use are not disclosed. It is composed of basic APIs that are expected to be needed in this project, and designed based on multi-lens products for expandability. Advanced APIs will be added through mutual consultation. This is a draft version, and there will be many changes in the specification and naming of the APIs, so please take it into account.
 *
 * API version: 0.1
 * Contact: aiden@linkflow.co.kr
 * Generated by: Swagger Codegen (https://github.com/swagger-api/swagger-codegen.git)
 */
package model

// TBD. A parameter object for when detailed parameters must be entered in the command.
type CommandParam struct {
	Name string `json:"name,omitempty"`

	Value interface{} `json:"value,omitempty"`
}
