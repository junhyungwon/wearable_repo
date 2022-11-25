* how to run
```bash
go get -u
go mod tity

go run cmd/server/main.go \
--port 8080
```

* test
```
curl -vv -X GET -H 'Authorization: Bearer token' http://localhost:8000/LFAPI/v1/device
```

* build for nexx
```
GOGC=100 \
GOARM=7 GOARCH=arm go build -o lfapi cmd/server/main.go \
--monitor  --limiter --limiter-max 1 --limiter-expiration 10

python3 -m http.server 8000


nexx$ cd
nexx$ curl http://192.168.0.109:8000/lfapi -o lfapi
nexx$ chmod a+x lfapi
nexx$ ./lfapi
```