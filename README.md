# Dependencies

- CMake v3.0 (or greater)
- boost v1.76

# Instructions

```bash
mkdir build
cd build
cmake .. && make
./bin/server
```

It will sit there waiting for you to send it a request, I'd recommend curl:

```bash
curl -d "message body" -X POST "http://localhost:3000/" -H "From: test@test.com" -H "To: other@test.com" -H "Cc: someone@test.com"
```

You'll have to manually shut it down, but it should receive several requests that way.
It has some minor error handling, but it's not perfect.
