package service

import (
	"lfapi/pkg/model"
	"log"

	"github.com/gofiber/fiber/v2"
)

func (cfg *Config) Index(c *fiber.Ctx) error {
	return c.SendString("lfapi server")
}

func (cfg *Config) GetAuthToken(c *fiber.Ctx) error {
	// fixme validate Admin
	return c.SendString("lfapi server")
}

func (cfg *Config) ExecuteCommand(c *fiber.Ctx) error {
	return c.SendString("lfapi server")
}

func (cfg *Config) GetDeviceInfo(c *fiber.Ctx) error {
	device := model.Device{DeviceId: `id`, Model: `model`, Version: `version`}
	return c.JSON(device)
}

func (cfg *Config) GetDeviceState(c *fiber.Ctx) error {
	return c.JSON(model.DeviceState{})
}

func (cfg *Config) GetCameraSettings(c *fiber.Ctx) error {
	return c.SendString("lfapi server")
}

func (cfg *Config) GetRTMPInfo(c *fiber.Ctx) error {
	log.Printf("cameraId: %s\n", c.Params("cameraId"))
	return c.JSON(model.Rtmp{})
}

func (cfg *Config) GetRTSPInfo(c *fiber.Ctx) error {
	log.Printf("cameraId: %s\n", c.Params("cameraId"))
	return c.SendString("lfapi server")
}

func (cfg *Config) GetSpecifiedCameraSetting(c *fiber.Ctx) error {
	log.Printf("cameraId: %s\n", c.Params("cameraId"))

	camera := model.Camera{
		CameraId:   `1`,
		Recording:  &model.CameraRecording{},
		Streaming:  &model.CameraStreaming{},
		Adjustment: &model.CameraAdjustment{},
		Option:     &model.CameraOption{},
	}
	log.Printf("%v\n", camera)
	return c.JSON(camera)
}

func (cfg *Config) ListRecordingFiles(c *fiber.Ctx) error {
	log.Printf("cameraId: %s\n", c.Params("cameraId"))
	files := make([]model.File, 1)
	return c.JSON(files)
}

func (cfg *Config) SetSpecifiedCameraSetting(c *fiber.Ctx) error {
	log.Printf("cameraId: %s\n", c.Params("cameraId"))
	return c.SendString("lfapi server")
}
