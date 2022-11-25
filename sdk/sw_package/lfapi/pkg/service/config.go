package service

type Config struct {
	Port  string
	Token string

	Tls            bool
	TlsCertPath    string
	TlsPrivatePath string

	Monitor           bool
	Limiter           bool
	LimiterMax        int
	LimiterExpiration int
}
