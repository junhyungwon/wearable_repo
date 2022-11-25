package service

import (
	"context"
	"log"
	"time"

	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/limiter"
	"github.com/gofiber/fiber/v2/middleware/logger"
	"github.com/gofiber/fiber/v2/middleware/monitor"
)

func (cfg *Config) validateAuth(ctx *fiber.Ctx) error {
	if val, ok := ctx.GetReqHeaders()["Authorization"]; ok {
		// fixme : check token. and big/small heaer letters
		if val == (`Bearer ` + cfg.Token) {
			return ctx.Next()
		}
	}

	return fiber.ErrUnauthorized
}

func (cfg *Config) WebRunServer(ctx context.Context) error {
	app := fiber.New()
	app.Use(logger.New(logger.Config{
		// Format: "${locals:requestid} [${ip}]:${port} ${status} - ${method} ${path}\n",
		TimeFormat: "2006-01-02 15:04:05.000000",
	}))

	if cfg.Limiter {
		log.Println(`enable feature - limiter. 429 Too Many Requests`)
		app.Use(limiter.New(limiter.Config{
			Max:               cfg.LimiterMax,
			Expiration:        time.Duration(cfg.LimiterExpiration) * time.Second,
			LimiterMiddleware: limiter.SlidingWindow{},
		}))
	}

	// for debug usage
	if cfg.Monitor {
		log.Println(`enable feature - monitor`)
		app.Get("/monitor", monitor.New(monitor.Config{Title: "Metrics Page"}))
	}

	// do not validate the auth header
	app.Get("/", cfg.Index)
	v1 := app.Group("/LFAPI/v1")
	v1.Post("/auth/token", cfg.GetAuthToken)

	// only check the auth followings
	app.Use(cfg.validateAuth)

	v1.Get("/", cfg.Index)
	v1.Get("/cameras/", cfg.GetCameraSettings)
	v1.Get("/device", cfg.GetDeviceInfo)
	v1.Get("/device/state", cfg.GetDeviceState)
	v1.Post("/device/command", cfg.ExecuteCommand)
	v1.Get("/cameras/:cameraId/media/rtmp", cfg.GetRTMPInfo)
	v1.Get("/cameras/:cameraId/media/rtsp", cfg.GetRTSPInfo)
	v1.Get("/cameras/:cameraId", cfg.GetSpecifiedCameraSetting)
	v1.Get("/cameras/:cameraId/files/", cfg.ListRecordingFiles)
	v1.Put("/cameras/:cameraId", cfg.SetSpecifiedCameraSetting)

	if cfg.Tls {
		return app.ListenTLS(":"+cfg.Port, cfg.TlsCertPath, cfg.TlsPrivatePath)
	} else {
		return app.Listen(":" + cfg.Port)
	}
}
