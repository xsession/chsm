"""
Test client for the CHSM debug channel.

Usage:
    1. Start the CHSM GUI (chsm_backend.py)
    2. In the GUI sidebar Debug panel, click "Connect" (default port 9999)
    3. Run this script: python test_debug_client.py

This will simulate a state machine sending live debug data.
"""
import socket
import json
import time

HOST = '127.0.0.1'
PORT = 9999


def send(sock, msg):
    sock.sendall((json.dumps(msg) + '\n').encode('utf-8'))


def main():
    print(f'Connecting to debug server at {HOST}:{PORT}...')
    with socket.create_connection((HOST, PORT)) as sock:
        print('Connected!')

        # Send initial state
        send(sock, {'type': 'state', 'active': ['idle']})
        send(sock, {'type': 'variables', 'vars': {'counter': 0, 'timer_ms': 1000}})
        send(sock, {'type': 'event', 'name': 'INIT', 'data': ''})
        time.sleep(2)

        # Simulate a transition
        send(sock, {'type': 'event', 'name': 'EVT_START'})
        send(sock, {'type': 'state', 'active': ['running']})
        send(sock, {'type': 'variables', 'vars': {'counter': 1, 'timer_ms': 500}})
        time.sleep(2)

        # Simulate nested states
        send(sock, {'type': 'event', 'name': 'EVT_TICK'})
        send(sock, {'type': 'variables', 'vars': {'counter': 2, 'timer_ms': 250}})
        time.sleep(2)

        # Simulate return to idle
        send(sock, {'type': 'event', 'name': 'EVT_STOP'})
        send(sock, {'type': 'state', 'active': ['idle']})
        send(sock, {'type': 'variables', 'vars': {'counter': 2, 'timer_ms': 0}})
        time.sleep(2)

        print('Test sequence complete.')


if __name__ == '__main__':
    main()
