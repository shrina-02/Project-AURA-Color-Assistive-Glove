# Project AURA – Color Assistive Glove

## Overview

Project AURA is an assistive wearable designed to help individuals with colour vision deficiency identify colours accurately and conveniently in real time.

The system integrates a TCS3200 colour sensor, ESP32 microcontroller, OLED display, and IoT-based web interface into a lightweight and portable glove. The sensor captures colour information from surrounding objects, while the ESP32 processes the detected RGB values and determines the closest human-readable colour name.

The identified colour is displayed on the OLED screen and simultaneously transmitted to a dedicated webpage, allowing the user to view the detected colour in real time.

The wearable is designed to provide hands-free and discreet colour identification without relying on audio feedback, making it suitable for everyday situations where silent visual feedback is preferred.

## Problem Statement

Colour vision deficiency can make everyday colour-dependent activities more challenging, including:

- Clothing selection
- Item sorting
- Recognition of colour-coded signals
- Navigation through colour-coded environments
- Identifying colours of objects in daily surroundings

Project AURA aims to provide a portable and accessible solution that assists users in identifying colours without requiring them to rely on another person for visual confirmation.

## Key Features

- Real-time colour identification
- TCS3200 colour sensor for RGB colour detection
- ESP32-based data processing
- Human-readable colour name identification
- OLED display for silent on-device feedback
- IoT connectivity for real-time webpage updates
- Lightweight glove-based wearable design
- Rechargeable battery-powered operation
- Hands-free and portable form factor
- Discreet colour feedback without audio output

## System Architecture

The system follows the following processing flow:

```text
Object
   │
   ▼
TCS3200 Colour Sensor
   │
   ▼
RGB Colour Data
   │
   ▼
ESP32 Microcontroller
   │
   ├──────────────► OLED Display
   │                  │
   │                  ▼
   │             Colour Name
   │
   └──────────────► IoT Webpage
                      │
                      ▼
                 Real-Time Display
