#!/usr/bin/env python3
# pylint: disable=line-too-long
# indent = tab
# tab-size = 4

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

# Build-in libraries
import argparse
import logging
import time
import paho.mqtt.client as mqtt

# Initialize logging here
logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s]: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )


class MQTTConnectionError(Exception):
    """
    Custom exception for MQTT connection errors.

    This exception is raised when there is a failure to establish a connection
    with an MQTT broker.

    Args:
        message (str): An optional custom error message to describe the specific
        connection issue. If not provided, a default message is used.

    Attributes:
        message (str): The error message describing the connection issue.

    Example:
        You can raise this exception with a custom message when handling MQTT
        connection errors to provide more context about the issue.

    """

    def __init__(self, message="MQTT connection failed"):
        self.message = message
        super().__init__(self.message)


class AgnesMqttClient(object):
    """
    AgnesMqttClient class for handling MQTT communication.

    This class encapsulates an MQTT client with specific configurations for the Agnes
    application. It provides methods and attributes for setting up and interacting
    with the MQTT broker.

    Attributes:
        TOPIC (str): The MQTT topic to subscribe to ("agnes.mqtt").
        CLIENT_ID (str): The client ID for the MQTT broker ("bridge").
        mqttc (paho.mqtt.client.Client): The MQTT client instance.

    Example:
        # Create an instance of AgnesMqttClient
        agnes_client = AgnesMqttClient()

        # Perform further configuration if needed

    """

    def __init__(self, topic, client_id='iMx8mn-evk') -> None:
        """
        Initialize an instance of AgnesMqttClient.

        This constructor sets up the MQTT client with predefined configurations,
        including the topic and client ID.

        """

        # Define the MQTT broker client ID and Topic to subscribe        
        self.TOPIC = topic
        self.CLIENT_ID = client_id

        # MQTT client setup
        self.mqttc = mqtt.Client(self.CLIENT_ID)
        self.mqttc.on_connect = self.__on_connect
        self.mqttc.on_message = self.__on_message


    def __on_connect(self, client, userdata, flags, rc) -> None:
        """
        MQTT on_connect callback function.

        This function is called when the MQTT client successfully connects to the broker
        or encounters an error during connection.

        Args:
            client (paho.mqtt.client.Client): The MQTT client instance.
            userdata (Any): User-defined data passed to the client.
            flags (dict): Flags indicating specific MQTT connection flags.
            rc (int): The connection result code.

        Returns:
            None

        Raises:
            Exception: If the MQTT connection fails (rc is not 0).

        Example:
            This function is typically set as the on_connect callback when configuring
            an MQTT client. It handles the logic for successful and failed connections.

        """

        if rc == 0:

            _ = client
            _ = userdata
            _ = flags

            logging.info("Connected to MQTT broker")
            client.subscribe(self.TOPIC)
        else:
            error_msg = f"Connection failed with code {rc}"
            logging.error(error_msg)
            raise MQTTConnectionError(error_msg)


    def __on_message(self, client, userdata, message) -> None:
        """
        Parse incoming messages from the MQTT broker.

        Parameters:
        - client: The MQTT client instance.
        - userdata: User data associated with the MQTT client.
        - message: The received message containing topic and payload.

        Returns:
        None

        Description:
        This method is called when a message is received on the MQTT topic.
        It extracts information about the topic and payload, logs the details,
        and sends the formatted message to the specified chat ID using the
        send_telegram_message function.

        Example:
        ```
        def on_message(client, userdata, message):
            instance.__on_message(client, userdata, message)
        ```
        """

        _ = client
        _ = userdata
        topic = message.topic

        # This method is called when a message is received on the MQTT topic
        message = message.payload.decode('utf-8')
        logging.info("Received MQTT message [%s]: %s", topic, message)


if __name__ == "__main__":
    # Script starting point.

    parser = argparse.ArgumentParser(description="Publish messages to Mqtt Topic.",
                                     usage="\n$%(prog)s -p topic-to-publish 'message to publish'")

    # Add required arguments for specifying the topic and message
    parser.add_argument("-p", "--topic", required=True, help="Specify the Mqtt Topic to publish to.")
    parser.add_argument("message", nargs='+', help="Message to publish to the Mqtt Topic.")

    args = parser.parse_args()

    # Log the provided topic and message
    logging.info("Topic: %s, Message: %s", args.topic, ' '.join(args.message))

    # Init the Mqtt class.
    BROKER_ADDRESS = "mqtt.eclipseprojects.io"
    PORT = 1883
    client = AgnesMqttClient(str(args.topic))

    try:
        # Start Mqtt client.
        client.mqttc.connect(BROKER_ADDRESS, PORT)
        client.mqttc.loop_start()

        # Publish the message and wait for the result
        logging.info('Publishing message to Topic.')
        result = client.mqttc.publish(args.topic, ' '.join(args.message), qos=1, retain=False)
        result.wait_for_publish()
        # Ensure the message is sent before proceeding
        time.sleep(2)

    except Exception as e:
        logging.error("Something went wrong -- %s", str(e), exc_info=False)

    finally:
        client.mqttc.disconnect()
        logging.info("Disconnected from MQTT broker.")
