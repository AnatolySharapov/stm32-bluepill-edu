# ===================================================================
# Script Name   : cli_test.py
#                 STM32 CLI automated test suite utility
# Description   : Automatically scans system hardware to discover the active
#                 USB-to-UART COM port, then executes case-insensitive test tokens.
# Location      : Process/cli_test/ folder
# Usage         : Run via cli_test.bat wrapper
#
# Author        : Gemini (Google AI)
# Verifier      : Anatoly Sharapov
# Email         : anatoly.a.sharapov@gmail.com
# Repository    : https://github.com
# License       : MIT
#
# Date Created  : August 16, 2026
# Last Modified : August 16, 2026
# Version       : 1.3.1
#
# Change History:
# -------------------------------------------------------------------
# Version 1.3.1 | Aug 16, 2026 | Fixed Heartbeat expected string mismatch keyword.
# Version 1.3.0 | Aug 16, 2026 | Implemented automated USB-to-UART COM port hardware discovery logic.
# Version 1.2.0 | Aug 16, 2026 | Removed python-level subprocess fallback (handled by batch wrapper).
# Version 1.1.0 | Aug 16, 2026 | Added automatic pip check and setup installation for pyserial dependency.
# Version 1.0.0 | Aug 16, 2026 | Initial release of the automated CLI testing tool.
# ===================================================================

import serial
import serial.tools.list_ports
import time
import sys

BAUD_RATE = 115200
TIMEOUT_SEC = 2.0

# Define test cases: (command_to_send, expected_keyword_in_response)
TEST_CASES = [
    ("HELP", "=== STM32 BLUE PILL LED CONTROLLER CLI ==="),
    ("on", "Executed: LED turned ON"),
    ("OFF", "Executed: LED turned OFF"),
    ("sos", "Executed: Mode SOS activated"),
    ("hb", "Executed: Mode Heartbeat activated"),  # FIXED: HeartBit -> Heartbeat
    ("INVALID_CMD", "Error: Command rejected")
]

# ... rest of the find_com_port and run_cli_tests code remains exactly the same ...


def find_com_port():
    """Scans system ports to find a connected USB-to-UART bridge adapter."""
    ports = list(serial.tools.list_ports.comports())
    
    # Target common silicon vendor names and generic hardware descriptions
    keywords = ["CH340", "CP210", "FTDI", "PL2303", "USB", "SERIAL"]
    
    for port in ports:
        desc = port.description.upper()
        hwid = port.hwid.upper()
        
        if any(kw in desc or kw in hwid for kw in keywords):
            print(f"[Autodetect] Found matching serial hardware: {port.device} ({port.description})")
            return port.device
            
    # Fallback to the first available port if no specific USB chip name matches
    if ports:
        print(f"[Autodetect] No specific USB chip matched. Falling back to first available: {ports[0].device}")
        return ports[0].device
        
    return None

def run_cli_tests():
    print("=== Starting Automated STM32 CLI Test Suite ===")
    
    # Discover the available target communication interface dynamically
    serial_port = find_com_port()
    if not serial_port:
        print("Error: No active COM ports detected in the system. Check your hardware connection.")
        sys.exit(1)
        
    print(f"Connecting to {serial_port} at {BAUD_RATE} baud...")
    
    try:
        # Open the serial connection using configuration parameters
        ser = serial.Serial(serial_port, BAUD_RATE, timeout=TIMEOUT_SEC)
        time.sleep(1)  # Wait for connection stabilization
        ser.reset_input_buffer()
        ser.reset_output_buffer()
    except Exception as e:
        print(f"Error: Could not open serial port {serial_port}. {e}")
        sys.exit(1)

    passed_tests = 0

    for index, (cmd, expected) in enumerate(TEST_CASES, start=1):
        print(f"\n[Test {index}] Sending command: '{cmd}'")
        
        # Send the command string followed by a carriage return to trigger parsing
        ser.write(f"{cmd}\r\n".encode('utf-8'))
        time.sleep(0.2)  # Allow the microcontroller to process and respond
        
        # Read all available data from the output buffer
        response = ser.read_all().decode('utf-8', errors='ignore')
        
        # Evaluate if the microcontroller output matches expectations
        if expected in response:
            print(f"Result: PASSED")
            passed_tests += 1
        else:
            print(f"Result: FAILED")
            print(f"Expected to find keyword: '{expected}'")
            print(f"Actual MCU response:\n{response}")

    ser.close()
    print("\n===============================================")
    print(f"Test Suite Finished: {passed_tests}/{len(TEST_CASES)} passed.")
    print("===============================================")

if __name__ == "__main__":
    run_cli_tests()
