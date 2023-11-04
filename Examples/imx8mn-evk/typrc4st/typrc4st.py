"""
Copyright (c) 2023 Andre Ribeiro Claudio
Author: Andre Ribeiro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
"""

# pylint:disable=line-too-long

# Build-in libraries
import argparse
import logging
import socket

HOST = "127.0.0.1"
PORT = 5071
BACKLOG = 5
BUFFER_SIZE = 1024


def server() -> None:
    """
    TODO: Description ...

    :param : None
    :return : None
    """

    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((HOST, PORT))
        server_socket.listen(BACKLOG)

        logging.info("Server is listening on %s:%s", HOST, PORT)

        while True:

            client_socket, client_address = server_socket.accept()
            logging.info("Accepted connection from %s", client_address)

            try:
                client_handler(client_socket)

            except socket.error as e:
                logging.exception(
                    "An error occurred while handling the client: %s", e, exc_info=False)
                client_socket.close()

    except socket.error as e:
        logging.exception("An error occurred: %s", e, exc_info=False)


def client_handler(client_socket) -> None:
    """
     TODO: Description ...

    :param client_socket: Socket object.
    :return : None   
    """

    while True:

        message = "Pong"

        try:
            data = client_socket.recv(BUFFER_SIZE)
            if not data:
                break
            logging.info("Received: %s", data.decode('utf-8'))
            client_socket.send(message.encode('utf-8'))

        except socket.error as e:
            logging.exception(
                "An error occurred while handling the client: %s", e)
            client_socket.close()


def client(target_ip) -> None:
    """
    TODO: Description ...

    :param target_ip: The server's IP address (only used in Client mode).
    :return : None
    """

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((target_ip, PORT))

        logging.info("Connected to %s:%s", target_ip, PORT)

        message = "Ping"

        try:
            client_socket.send(message.encode('utf-8'))
            response = client_socket.recv(BUFFER_SIZE)
            logging.info("Received: %s", response.decode('utf-8'))

        except socket.error as e:
            logging.exception("An error occurred: %s", e, exc_info=False)

        finally:
            client_socket.close()

    except socket.error as e:
        logging.exception("An error occurred: %s", e, exc_info=False)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Tiny Yet Powerful Remote Client \
                                    for Stats that can operate as both a Client or Server.",
                                     usage="\n[Client mode] $ python3 %(prog)s client --target-ip A.B.C.D\n[Server mode] $ python3 %(prog)s")

    # Define the mode argument as an optional positional argument with a default of "server"
    parser.add_argument("mode",
                        nargs="?",
                        choices=["server", "client"],
                        default="server",  # "server" is the default mode
                        help="Select the mode (server or client).")

    # Define the IP argument for the client option
    parser.add_argument("--target-ip",
                        type=str,
                        help="The server's IP address, which the client will communicate with (Client mode only).")

    args = parser.parse_args()

    # Check if client mode is selected and if the target IP is provided
    if args.mode == "client" and args.target_ip is None:
        parser.error(
            "When using client mode (client), you must provide the target server IP.")

    # Initialize logging here
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s]: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
        # filename="%(prog)s.log"
    )

    if args.mode == "server":
        # Run the scrip as Server
        logging.info("Starting script as Server.")
        server()

    elif args.mode == "client":
        # Run the scrip as Client
        logging.info(
            "Starting script as Client. The target server IP is: %s", args.target_ip)
        client(args.target_ip)
