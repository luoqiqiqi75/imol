"""Exercise the real Qt client against wire fixtures matching the archived SDK.

This validates the wire contract, not an integrated realtime controller build.
Usage: python tests/xservice_wire.py path/to/imol_xservice_client.exe
Qt's bin directory must be in PATH.
"""
import json
import socket
import subprocess
import sys
import threading
import time


def main():
    failures = []
    operations = []
    with socket.socket() as listener:
        listener.bind(("127.0.0.1", 0))
        listener.listen(1)
        listener.settimeout(15)
        port = listener.getsockname()[1]

        def serve():
            try:
                with listener.accept()[0] as connection:
                    connection.settimeout(10)
                    cache = b""
                    value = 12

                    def send(message, fragmented=False):
                        frame = json.dumps(message, separators=(",", ":")).encode() + b"\r"
                        if fragmented:
                            middle = len(frame) // 2
                            connection.sendall(frame[:middle])
                            time.sleep(0.01)
                            connection.sendall(frame[middle:])
                        else:
                            connection.sendall(frame)

                    while True:
                        data = connection.recv(65536)
                        if not data:
                            break
                        cache += data
                        while b"\r" in cache:
                            raw, cache = cache.split(b"\r", 1)
                            if not raw:
                                continue
                            request = json.loads(raw)
                            response = {"id": request["id"]}
                            if "imol" in request:
                                assert request["id"] == "@handshake"
                                operations.append("handshake")
                                response["imol"] = {"version": {"ix.sdk": 9}, "data": ["example"], "command": ["echo"]}
                            elif "g" in request:
                                operations.append("get")
                                response["g"] = {key: {"value": value} if key == "example" else -0x0102 for key in request["g"]}
                            elif "s" in request:
                                operations.append("set")
                                value = request["s"]["example"]["value"]
                                response["s"] = {"example": 0}
                            elif "w" in request:
                                watching = request["w"]["example"]
                                operations.append("watch" if watching else "unwatch")
                                send(response)
                                if watching:
                                    value = 100
                                    send({"id": request["id"], "n": {"example": {"value": value}}}, fragmented=True)
                                continue
                            elif "c" in request:
                                operations.append("command")
                                response["c"] = {"echo": {"data": request["c"]["echo"]}}
                            else:
                                raise AssertionError("unexpected request: " + str(request))
                            send(response, fragmented=True)
            except Exception as error:
                failures.append(repr(error))

        server = threading.Thread(target=serve, daemon=True)
        server.start()
        result = subprocess.run([sys.argv[1], str(port)], timeout=25, capture_output=True, text=True)
        server.join(timeout=2)
        print(result.stdout, end="")
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        assert result.returncode == 0, (result.returncode, operations, failures)
        assert not failures, failures
        assert operations == ["handshake", "get", "set", "get", "watch", "unwatch", "command", "get"], operations
        print("CR framing, fragmented replies, SDK scalar results and notifications verified")


if __name__ == "__main__":
    main()
