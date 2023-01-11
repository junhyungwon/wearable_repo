package cmd

import (
	"context"
	"flag"
	"os"
	"strconv"

	"lfapi/pkg/service"
)

func getEnv(key, fallback string) string {
	if value, ok := os.LookupEnv(key); ok {
		return value
	}
	return fallback
}

func getEnvInt(key string, fallback int) int {
	if value, ok := os.LookupEnv(key); ok {
		if i, err := strconv.Atoi(value); err != nil {
			return i
		}
	}
	return fallback
}

func getEnvBool(key string, fallback bool) bool {
	if value, ok := os.LookupEnv(key); ok {
		if i, err := strconv.ParseBool(value); err != nil {
			return i
		}
	}
	return fallback
}

func RunServer() error {
	ctx := context.Background()

	// get configuration
	var cfg service.Config
	flag.StringVar(&cfg.Port, "port", getEnv("PORT", "8000"), "web port to bind")
	flag.StringVar(&cfg.Token, "token", getEnv("TOKEN", "token"), "auth token") // fixme

	flag.BoolVar(&cfg.Tls, "tls", getEnvBool("TLS", false), "enable tls")
	flag.StringVar(&cfg.TlsCertPath, "tls-cert-path", getEnv("TLS_CERT_PATH", "/ssl/fullchain.pem"), "tls cert")
	flag.StringVar(&cfg.TlsPrivatePath, "tls-private-path", getEnv("TLS_PRIVATE_PATH", "/ssl/private.pem"), "tls private")

	flag.BoolVar(&cfg.Monitor, "monitor", getEnvBool("MONITOR", false), "enable '/monitor'")

	flag.BoolVar(&cfg.Limiter, "limiter", getEnvBool("LIMITER", false), "enable limiter(Sliding Window)")
	flag.IntVar(&cfg.LimiterMax, "limiter-max", getEnvInt("LIMITER_MAX", 10), "max request per expiration")
	flag.IntVar(&cfg.LimiterExpiration, "limiter-expiration", getEnvInt("LIMITER_EXPIRATION", 1), "tls cert")
	flag.Parse()

	// create temporary path
	_ = os.MkdirAll("temp", os.ModePerm)

	return cfg.WebRunServer(ctx)
}
