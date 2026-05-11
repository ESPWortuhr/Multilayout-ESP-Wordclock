#!/usr/bin/env python3
"""Build and serve the Wordclock web UI locally."""

from __future__ import annotations

import argparse
import functools
import http.server
import socketserver
import subprocess
import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parents[1]
WEB_BUILD_DIR = REPO_ROOT / ".pio" / "build" / "webpage"
INDEX_HTML = WEB_BUILD_DIR / "index.html"


class QuietHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    """Keep request logging compact while still showing served paths."""

    def log_message(self, format: str, *args: object) -> None:
        sys.stderr.write(f"{self.address_string()} - {format % args}\n")


class ReusableTcpServer(socketserver.TCPServer):
    allow_reuse_address = True


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Build the web UI and serve .pio/build/webpage via HTTP.",
    )
    parser.add_argument(
        "--host",
        default="127.0.0.1",
        help="Host/interface to bind to. Default: 127.0.0.1",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=8080,
        help="Port to bind to. Default: 8080",
    )
    parser.add_argument(
        "--no-build",
        action="store_true",
        help="Serve the existing build output without running grunt build first.",
    )
    return parser.parse_args()


def run_build() -> None:
    print("Building web UI with `npm exec grunt -- build` ...", flush=True)
    subprocess.run(
        ["npm", "exec", "grunt", "--", "build"],
        cwd=REPO_ROOT,
        check=True,
    )


def ensure_build_output_exists() -> None:
    if INDEX_HTML.exists():
        return

    raise SystemExit(
        f"Missing {INDEX_HTML}. Run this script without --no-build first.",
    )


def serve(host: str, port: int) -> None:
    handler = functools.partial(
        QuietHttpRequestHandler,
        directory=str(WEB_BUILD_DIR),
    )

    try:
        with ReusableTcpServer((host, port), handler) as server:
            print(f"Serving Wordclock web UI at http://{host}:{port}/", flush=True)
            print("Press Ctrl+C to stop.", flush=True)
            server.serve_forever()
    except OSError as error:
        raise SystemExit(
            f"Could not bind to {host}:{port}: {error}. Try --port 8081.",
        ) from error
    except KeyboardInterrupt:
        print("\nServer stopped.", flush=True)


def main() -> None:
    args = parse_args()

    if not args.no_build:
        run_build()

    ensure_build_output_exists()
    serve(args.host, args.port)


if __name__ == "__main__":
    main()
